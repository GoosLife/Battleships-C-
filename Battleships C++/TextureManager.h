#pragma once
#include <map>
#include <SDL.h>
#include <string>

class TextureManager
{
private:
	TextureManager() {}
	~TextureManager() {}
	static TextureManager* s_pInstance;

public:
	static TextureManager* Instance() {
		// Create texture manager if one doesn't exist
		if (s_pInstance == 0) {
			s_pInstance = new TextureManager();
		}

		// return texture manager
		return s_pInstance;
	}

	bool load(std::string filename, std::string id, SDL_Renderer* pRenderer);

	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void clearFromTextureMap(std::string id);

	std::map<std::string, SDL_Texture*> m_textureMap;

};
	typedef TextureManager TheTextureManager;

