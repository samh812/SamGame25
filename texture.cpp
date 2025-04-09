// COMP710 GP Framework 2025
// This include:
#include "texture.h"
// Local includes:
#include "logmanager.h"
// Library include:
#include <SDL_image.h>
#include <cassert>
#include "glew.h"
#include <SDL.h>
#include <cstring>

#include <SDL_ttf.h>
Texture::Texture()
	: m_uiTextureId(0)
	, m_iHeight(0)
	, m_iWidth(0)
{
}
Texture::~Texture()
{
	glDeleteTextures(1, &m_uiTextureId);
}
bool Texture::Initialise(const char* pcFilename)
{
	SDL_Surface* pSurface = IMG_Load(pcFilename);
	if (pSurface)
	{
		m_iWidth = pSurface->w;
		m_iHeight = pSurface->h;
		int bytesPerPixel = pSurface->format->BytesPerPixel;
		unsigned int format = 0;
		if (bytesPerPixel == 3)
		{
			format = GL_RGB;
		}
		else if (bytesPerPixel == 4)
		{
			format = GL_RGBA;
		}
		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format, GL_UNSIGNED_BYTE, pSurface->pixels);
		SDL_FreeSurface(pSurface);
		pSurface = 0;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		LogManager::GetInstance().Log("Texture failed to load!");
		assert(0);
		return false;
	}
	return true;
}
void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
}
int Texture::GetWidth() const
{
	assert(m_iWidth);
	return (m_iWidth);
}
int Texture::GetHeight() const
{
		assert(m_iHeight);
		return (m_iHeight);
}

void
Texture::LoadTextTexture(const char* text, const char* fontname, int pointsize)
{
	TTF_Font* pFont = 0;
	TTF_Init();
	if (pFont == 0)
	{
		pFont = TTF_OpenFont(fontname, pointsize);
	}
	SDL_Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	SDL_Surface* pSurface = TTF_RenderText_Blended(pFont, text, color);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);
	LoadSurfaceIntoTexture(pSurface);
	TTF_CloseFont(pFont);
	pFont = 0;
}
void
Texture::LoadSurfaceIntoTexture(SDL_Surface* pSurface)
{
	if (pSurface)
	{
		m_iWidth = pSurface->w;
		m_iHeight = pSurface->h;
		int bytesPerPixel = pSurface->format->BytesPerPixel;
		unsigned int format = (bytesPerPixel == 3) ? GL_RGB : GL_RGBA;

		// Flip the surface manually (row-wise swap)
		int pitch = pSurface->pitch;
		uint8_t* pixels = (uint8_t*)pSurface->pixels;
		uint8_t* temp = new uint8_t[pitch];

		for (int i = 0; i < m_iHeight / 2; ++i)
		{
			uint8_t* row1 = pixels + (i * pitch);
			uint8_t* row2 = pixels + ((m_iHeight - i - 1) * pitch);
			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}
		delete[] temp;

		glGenTextures(1, &m_uiTextureId);
		glBindTexture(GL_TEXTURE_2D, m_uiTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_iWidth, m_iHeight, 0, format,
			GL_UNSIGNED_BYTE, pSurface->pixels);
		SDL_FreeSurface(pSurface);
		pSurface = 0;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

