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
#include <iostream>

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
		LoadSurfaceIntoTexture(pSurface);  // Now does flipping too!
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
	static bool ttfInitialized = false;
	if (!ttfInitialized)
	{
		if (TTF_Init() == -1)
		{
			LogManager::GetInstance().Log("TTF_Init failed!");
			return;
		}
		ttfInitialized = true;
	}

	TTF_Font* pFont = TTF_OpenFont(fontname, pointsize);
	if (!pFont)
	{
		LogManager::GetInstance().Log("Failed to open font!");
		return;
	}

	SDL_Color color = { 255, 255, 255, 255 };
	SDL_Surface* pSurface = TTF_RenderText_Blended(pFont, text, color);
	if (!pSurface)
	{
		LogManager::GetInstance().Log("Failed to render text surface!");
		TTF_CloseFont(pFont);
		return;
	}

	glPixelStorei(GL_UNPACK_ROW_LENGTH, pSurface->pitch / pSurface->format->BytesPerPixel);
	LoadSurfaceIntoTexture(pSurface); // This frees the surface

	TTF_CloseFont(pFont);
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

		//manually flipping surface
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

