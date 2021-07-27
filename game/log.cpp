#include "log.h"
#include <vector>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#define VC_EXTRALEAN 1
#include <windows.h>
#include <process.h>
#endif


void defaultSink(Log::MessageType type, const char* message);
static Log::messageSink_t s_messageSink = &defaultSink;

// Windows colors
// Maybe change their values on Linux?
enum class ConsoleColor
{
    BLACK = 0,
    DARKBLUE = 1,
    DARKGREEN = 2,
    DARKRED = 4,
    DARKGRAY = 8,
    GRAY = DARKRED | DARKGREEN | DARKBLUE,
    DARKCYAN = DARKBLUE | DARKGREEN,
    DARKMAGENTA = DARKRED | DARKBLUE,
    DARKYELLOW = DARKRED | DARKGREEN,
    WHITE = DARKGRAY | GRAY,
    RED = DARKGRAY | DARKRED,
    BLUE = DARKGRAY | DARKBLUE,
    GREEN = DARKGRAY | DARKGREEN,
    MAGENTA = DARKGRAY | DARKMAGENTA,
    YELLOW = DARKGRAY | DARKYELLOW,
    CYAN = DARKGRAY | DARKCYAN,
};


void SetConsoleTextForegroundColor(ConsoleColor color)
{
#ifdef _WIN32
    HANDLE Con;
    Con = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(Con, (WORD)color);
#endif
}


// Cheesy move to strip off the path from the file name 
// Note: This does require log.cpp to exist at the base of the file tree!
//consteval
constexpr int findInFILE(char s)
{
    constexpr const char path[] = __FILE__;
    constexpr const int len = sizeof(path);
    for (int i = len - 1; i != 0; i--)
        if (path[i] == s)
            return i;
    return -1;
}

//consteval
constexpr int baseFilePathLength()
{
    int 
#ifdef _WIN32
    slash = findInFILE('\\');
    if (slash < 0)
        slash = findInFILE('/');
#else
    slash = findInFILE('/');
    if (slash < 0)
        slash = findInFILE('\\');
#endif
    if (slash > 0)
        return slash + 1;
    return 0;
}

static constexpr int BASE_FILE_PATH_LENGTH = baseFilePathLength();


// Use this so that all assertions look consistent
void formatAssertion(char* message, size_t length, const char* expression, int line, const char* file, const char* function)
{
    snprintf(message, 1024, "-= Assertion failed! =-\n%s\n%s in %s: Line %d\n\n", expression, function, file + BASE_FILE_PATH_LENGTH, line);
}

bool Log::Assert(bool condition, const char* expression, int line, const char* file, const char* function)
{
    if (!condition)
    {
        char* message = LogBuffer(1024);
        formatAssertion(message, 1024, expression, line, file, function);
        Drain(MessageType::FAULT, message);

#ifdef _WIN32
        int ret = MessageBoxA(nullptr, message, "Assertion failed!", MB_ABORTRETRYIGNORE);

        // This is counterintuitive to the buttons...
        switch (ret)
        {
        case IDABORT:
            exit(1);
            break;
        case IDRETRY:
            DebugBreak();
            break;
        }
#else
#endif
       
    }
    return condition;
}


bool Log::AssertSilent(bool condition, const char* expression, int line, const char* file, const char* function)
{
    if (!condition)
    {
        char* buf = LogBuffer(1024);
        formatAssertion(buf, 1024, expression, line, file, function);
        Drain(MessageType::FAULT, buf);
    }
    return condition;
}


bool Log::AssertFatal(bool condition, const char* expression, int line, const char* file, const char* function)
{
    if (!condition)
    {
        char message[1024];
        formatAssertion(message, 1024, expression, line, file, function);
        Drain(MessageType::FATAL, message);
    }
    return condition;
}

void defaultSink(Log::MessageType type, const char* message)
{

    switch (type)
    {
    case Log::MessageType::NONE:
        SetConsoleTextForegroundColor(ConsoleColor::GRAY);
        break;
    case Log::MessageType::DEBUG:
        SetConsoleTextForegroundColor(ConsoleColor::GREEN);
        break;
    case Log::MessageType::MESSAGE:
        SetConsoleTextForegroundColor(ConsoleColor::CYAN);
        break;
    case Log::MessageType::WARNING:
        SetConsoleTextForegroundColor(ConsoleColor::YELLOW);
        break;
    case Log::MessageType::FAULT:
        SetConsoleTextForegroundColor(ConsoleColor::RED);
        break;
    case Log::MessageType::FATAL:
    {
        SetConsoleTextForegroundColor(ConsoleColor::MAGENTA);
        printf(message);

        // Incase anything wants to print after we're done running
        SetConsoleTextForegroundColor(ConsoleColor::GRAY);

        // pfd::message("Fatal Error!", message, pfd::choice::abort_retry_ignore).result();

        exit(1);
        return;
    }
    break;
    default:
        SetConsoleTextForegroundColor(ConsoleColor::GRAY);
        break;
    }

    printf(message);
    SetConsoleTextForegroundColor(ConsoleColor::GRAY);
}

void Log::SetSink(messageSink_t sink)
{
    s_messageSink = sink;
}

void Log::Drain(MessageType type, char* str)
{
    if (s_messageSink)
        s_messageSink(type, str);
}

char* Log::LogBuffer(size_t reserve)
{
    static thread_local std::vector<char> s_messageBuffer;
    s_messageBuffer.reserve(reserve);
    return s_messageBuffer.data();
}
