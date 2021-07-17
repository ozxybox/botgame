#pragma once
#include "basesystem.h"
#include <map>
#include <string>
#include <glm/glm/ext/vector_float4.hpp>

// Handle to a texture
typedef size_t texture_t;

typedef size_t tileset_t;

typedef size_t framebuffer_t;


struct rendertarget_t
{
	texture_t texture;
	framebuffer_t framebuffer;
};

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

	texture_t NamedTextureFromPixels(const char* name, TextureFormat format, unsigned int width, unsigned int height, void* pixels);

	tileset_t LoadTileset(const char* path, unsigned int tilewidth, unsigned int tileheight);

	// Returns the postion of a tile within a tileset, from 0-1, for use within a shader
	glm::vec4 TileRect(tileset_t tileset, int id);

	// Use this if you know exactly which tile you want
	glm::vec4 TileRect(tileset_t tileset, int x, int y);

	// Returns's the texture's OpenGL handle. Used for rendering
	unsigned int RenderHandle(texture_t t);

	texture_t TilesetTexture(tileset_t tileset);

	rendertarget_t CreateRenderTarget(const char* name, unsigned int width, unsigned int height, bool depthBuffer = false);
private:

	std::map<std::string, texture_t> m_textures;
	std::map<std::string, tileset_t> m_tilesets;

	texture_t m_errorTexture;
};
