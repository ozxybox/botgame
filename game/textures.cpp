#include "textures.h"
#include "log.h"

#include <glad/glad.h>

#include <vector>
#include <lodepng/lodepng.h>


// == NOTICE == 
//  - At some point in time, texture_t and tileset_t should STOP BEING POINTERS!
//    DO NOT WRITE SWATHS OF CODE OUTSIDE OF TEXTURES.CPP THAT EXPECTS THAT


DEFINE_SINGLETON(Textures);

struct textureData_t
{
	GLuint id;
	unsigned int width, height;
};

struct tilesetData_t
{
	unsigned int tilew, tileh;
	unsigned int cols, rows;
	textureData_t* texure;
};


textureData_t* TextureFromPixels(TextureFormat format, unsigned int width, unsigned int height, void* pixels)
{
	GLenum glFormat = GL_R;
	GLenum glSize = GL_UNSIGNED_BYTE;
	switch (format)
	{
	case TextureFormat::R8:
		glFormat = GL_R;
		glSize = GL_UNSIGNED_BYTE;
		break;
	case TextureFormat::RGB24:
		glFormat = GL_RGB;
		glSize = GL_UNSIGNED_BYTE;
		break;
	case TextureFormat::RGBA32:
		glFormat = GL_RGBA;
		glSize = GL_UNSIGNED_BYTE;
		break;
	default:
		break;
	}

	GLuint textureid;
	glGenTextures(1, &textureid);
	glBindTexture(GL_TEXTURE_2D, textureid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, glFormat, glSize, pixels);
	return new textureData_t{ textureid, width, height };
}

textureData_t* loadPng(const char* path)
{
	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, path);

	if (error)
	{
		Log::Fault("Error loading png %s!\n %d: %s\n", path, error, lodepng_error_text(error));
		return 0;
	}
	
	return TextureFromPixels(TextureFormat::RGBA32, width, height, image.data());
}


CTextures::CTextures()
{
	// Create a texture to return when we fail to load one
	 
	unsigned int errorPixels[] = { 0xFF0000FF, 0xFFFFFF00, 0xFFFFFF00, 0xFF0000FF };
	m_errorTexture = NamedTextureFromPixels("__|error", TextureFormat::RGBA32, 2, 2, errorPixels);
}

CTextures::~CTextures()
{
	for (auto p : m_textures)
	{
		glDeleteTextures(1, &((textureData_t*)p.second)->id);
	}
}

texture_t CTextures::LoadTexture(const char* path)
{
	std::string str = path;
	if (m_textures.contains(str))
		return m_textures[str];
	
	texture_t txt = (texture_t)loadPng(path);
	if (txt)
		return txt;
	return m_errorTexture;
}

texture_t CTextures::NamedTextureFromPixels(const char* name, TextureFormat format, unsigned int width, unsigned int height, void* pixels)
{
	std::string str = name;
	if (m_textures.contains(str))
		return m_textures[str];

	texture_t txt = (texture_t)TextureFromPixels(format, width, height, pixels);
	m_textures.emplace(str, txt);
	return txt;
}

tileset_t CTextures::LoadTileset(const char* path, unsigned int tilewidth, unsigned int tileheight)
{
	std::string str = path;
	if (m_tilesets.contains(str))
		return m_tilesets[str];

	texture_t t = LoadTexture(path);
	textureData_t* xd = (textureData_t*)t;
	tilesetData_t* td = new tilesetData_t{ tilewidth, tileheight, xd->width / tilewidth, xd->height / tileheight, xd };
	m_tilesets.emplace(str, (tileset_t)td);
	return (tileset_t)td;
}

glm::vec4 CTextures::TileRect(tileset_t tileset, int id)
{
	tilesetData_t* td = (tilesetData_t*)tileset;
	
	int x = id % td->cols;
	int y = id / td->cols;

	return { x / (float)td->cols, y / (float)td->rows, td->tilew / (float)td->texure->width, td->tileh / (float)td->texure->height};
}

glm::vec4 CTextures::TileRect(tileset_t tileset, int x, int y)
{
	tilesetData_t* td = (tilesetData_t*)tileset;

	return { x / (float)td->cols, y / (float)td->rows, td->tilew / (float)td->texure->width, td->tileh / (float)td->texure->height };
}

unsigned int CTextures::RenderHandle(texture_t t) 
{
	return ((textureData_t*)t)->id;
}

texture_t CTextures::TilesetTexture(tileset_t tileset)
{
	return (texture_t)((tilesetData_t*)tileset)->texure;
}

rendertarget_t CTextures::CreateRenderTarget(const char* name, unsigned int width, unsigned int height, bool depthBuffer)
{
	GLuint framebufferid = 0;
	glGenFramebuffers(0, &framebufferid);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferid);

	texture_t texture = NamedTextureFromPixels(name, TextureFormat::RGBA32, width, height, 0);
	GLuint txid = RenderHandle(texture);

	// Attach the texture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, txid, 0);



	if (depthBuffer)
	{
		// The depth buffer
		GLuint depthrenderbuffer;
		glGenRenderbuffers(1, &depthrenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	}

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, txid, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Check if our framebuffer is feeling well
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return {0,0};

}
