#pragma once
#include<raylib.h>
#include<vector>
#include"sprmngr.hpp"
#include"sfxmngr.hpp"
#include"player.hpp"
#include"level.hpp"
#include"gui.hpp"
#include"skybox.hpp"

class Game {
public:
	Game();
	~Game();

	void InitGame();
	void InitShaders();
	void InitFog();
	void MainLoop();
	void Render();
	void RenderSkybox();
	void ProcessObjects();
	void ProcessEntities();
	void ProcessParticles();
	void ProcessBullets();
	void UnloadGame();

private:
	static const int WINDOW_WIDTH = 960;
	static const int WINDOW_HEIGHT = 540;
	static const int RENDERTEX_WIDTH = 960;
	static const int RENDERTEX_HEIGHT = 460;
	static constexpr const char* GAME_NAME = "DeepDung";

	bool paused = false;

	SpriteManager* sprMngr = nullptr;
	SFXManager* sfxMngr = nullptr;
	Player* player = nullptr;
	Level* level = nullptr;
	GUI* gui = nullptr;
	Skybox* skybox = nullptr;

	Shader fogShader;
	Shader pixelizerShader;
	RenderTexture2D renderTexture;

	std::vector<int> levelKeys;
};
