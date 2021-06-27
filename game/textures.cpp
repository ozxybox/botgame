#include "textures.h"
#include "log.h"

#include <glad/glad.h>

#include <vector>
#include <lodepng/lodepng.h>

DEFINE_SINGLETON(Textures);



GLuint loadPng(const char* path)
{
	std::vector<unsigned char> image;
	unsigned int width, height;
	unsigned int error = lodepng::decode(image, width, height, path);

	if (error)
	{
		Log::Fault("Error loading png %s!\n %d: %s\n", path, error, lodepng_error_text(error));
		return GL_INVALID_INDEX;
	}
	
	return Textures().TextureFromPixels(TextureFormat::RGBA32, width, height, image.data());
}


CTextures::CTextures()
{
	// Create a texture to return when we fail to load one
	 
	unsigned int errorPixels[] = { 0xFF0000FF, 0xFFFFFF00, 0xFFFFFF00, 0xFF0000FF };
	m_errorTexture = TextureFromPixels(TextureFormat::RGBA32, 2, 2, errorPixels);
}

CTextures::~CTextures()
{
	for (auto p : m_textures)
	{
		if (p.second != m_errorTexture)
			glDeleteTextures(1, &p.second);
	}
	glDeleteTextures(1, &m_errorTexture);
}

texture_t CTextures::LoadTexture(const char* path)
{
	std::string str = path;
	if (m_textures.contains(str))
		return m_textures[str];
	
	texture_t txt = loadPng(path);
	if (txt == GL_INVALID_INDEX)
		txt = m_errorTexture;

	m_textures.emplace(str, txt);
	return txt;
}

texture_t CTextures::TextureFromPixels(TextureFormat format, int width, int height, void* pixels)
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
	return textureid;
}
