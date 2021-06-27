#pragma once
#include "basesystem.h"
#include <map>
#include <string>

typedef unsigned int texture_t;

enum class TextureFormat
{
	R8,
	RGB24,
	RGBA32,
};


DECLARE_SINGLETON_ACCESSOR(Textures);
class CTextures : public IBaseSingleton<CTextures>
{
	DECLARE_SINGLETON(Textures);
public:
	CTextures();
	~CTextures();

	texture_t LoadTexture(const char* path);

	texture_t ErrorTexture() { return m_errorTexture; }

	texture_t TextureFromPixels(TextureFormat format, int width, int height, void* pixels);
private:

	std::map<std::string, texture_t> m_textures;

	texture_t m_errorTexture;
};
