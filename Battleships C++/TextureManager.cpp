#pragma warning(disable:26812) // Disable 'prefer enum class over enum' from imported SDL library

#include <iostream>
#include <SDL_image.h>

#include "TextureManager.h"

TextureManager* TextureManager::s_pInstance = 0;

bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* pRenderer)
{
	// Create a temporary surface to turn into a texture
	SDL_Surface* pTempSurface = IMG_Load(fileName.c_str());

	if (pTempSurface == 0) 
	{
		std::cout << "Surface couldn't be created from " << fileName.c_str() << ".\n"; // DEBUG
		return false; // Something went wrong: Surface wasn't created.
	}

	// Create the texture from the temporary surface
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);

	// We don't need the surface anymore
	SDL_FreeSurface(pTempSurface);

	// If the texture was created, add it to the list of textures
	if (pTexture != 0) 
	{
		m_textureMap[id] = pTexture;
		return true; // Success!
	}

	std::cout << "Texture couldn't be created from " << fileName.c_str() << ", after succesfully creating surface.\n"; // DEBUG
	return false; // Something went wrong: Texture wasn't created.
}

void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = width * currentFrame;
	srcRect.y = height * (currentRow - 1);
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;

	SDL_RenderCopyEx(pRenderer, m_textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

void TextureManager::clearFromTextureMap(std::string id)
{
	m_textureMap.erase(id);
}
