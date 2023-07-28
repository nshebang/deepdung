#include"game.hpp"
#include<rlgl.h>

#define RLIGHTS_IMPLEMENTATION
#include"rextras/rlights.h"

Game::Game()
{
	this->InitGame();
	TraceLog(LOG_INFO, "Game started.");
	this->MainLoop();
}

Game::~Game()
{}

void
Game::InitGame()
{
	InitWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, this->GAME_NAME);
	SetTargetFPS(60);
	//SetConfigFlags(FLAG_MSAA_4X_HINT);
	ChangeDirectory(GetApplicationDirectory());
	TraceLog(LOG_INFO, "Working dir is %s", GetApplicationDirectory());

	this->InitShaders();

	this->sprMngr = new SpriteManager();
	this->sfxMngr = new SFXManager();
	this->level = new Level(this->sprMngr);
	this->player = new Player(this->level->GetObjects(),
		this->level->GetEntities(),
		this->level->GetObjsInfo());
	this->gui = new GUI(this->sprMngr,
		this->player->GetInventory(),
		RENDERTEX_HEIGHT, &this->paused);
	this->skybox = new Skybox();

	this->levelKeys.assign(this->level->MAX_LEVELS, 0);
}

void
Game::InitShaders()
{
	this->renderTexture = LoadRenderTexture(this->RENDERTEX_WIDTH,
		this->RENDERTEX_HEIGHT);
	
	this->pixelizerShader = LoadShader(0, "res/shaders/pixelizer.fs");
	this->InitFog();	
}

void
Game::InitFog()
{
	this->fogShader = LoadShader(
		"res/shaders/lighting.vs",
		"res/shaders/fog.fs"
	);
	this->fogShader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(
		this->fogShader,
		"matModel"
	);
    this->fogShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(
		this->fogShader,
		"viewPos"
	);
    
	int fogAmbientLoc = GetShaderLocation(this->fogShader, "ambient");
    int fogColorLoc = GetShaderLocation(this->fogShader, "fogColor");
	int fogDensityLoc = GetShaderLocation(this->fogShader, "fogDensity");

	float fogColor[4] = {0.f, 0.f, 0.f, 0.5f};
	float fogBrightness[4] = { .25f, .25f, .25f, 0.f };
	float fogDensity = .5f;
	SetShaderValue(this->fogShader, fogAmbientLoc,
		fogBrightness, SHADER_UNIFORM_VEC4);
	SetShaderValue(this->fogShader, fogColorLoc,
		fogColor, SHADER_UNIFORM_VEC4);
	SetShaderValue(this->fogShader, fogDensityLoc,
		&fogDensity, SHADER_UNIFORM_FLOAT);
}

void
Game::MainLoop()
{
	DisableCursor();

	while(!WindowShouldClose()) {
		this->sprMngr->TickSpriteAnimations();
		this->gui->Input();

		if (!this->paused)
			this->Render();

		this->levelKeys[this->level->GetCurrentLevel()] = this->player
			->GetInventory()
			->keys;

		if (this->player->wantsToChangeLevel != -1) {
			this->sfxMngr->PlaySFX("warp");
			this->level->LevelGoto(this->level->LEVEL_NAMES[
				this->player->wantsToChangeLevel]);	
			
			char enteringMsg[256];
			TextCopy(enteringMsg, TextFormat("Entering %s", 
				this->level->LEVEL_NAMES[this->player->wantsToChangeLevel]));
			this->gui->ShowMessage(TextReplace(enteringMsg, "_", " "),
				true, false);
			
			this->player->GetInventory()->keys = this->levelKeys[
				this->player->wantsToChangeLevel];
			this->player->wantsToChangeLevel = -1;
			this->player->GetInventory()->warpInvulnerability = false;
		}

		if (this->player->GetInventory()->gotKey) {
			this->gui->ShowMessage("You got a key.\nYou can open the doors.",
				false, true);
			this->player->GetInventory()->gotKey = false;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		BeginShaderMode(this->pixelizerShader);
			DrawTextureRec(this->renderTexture.texture, 
				(Rectangle){ 0, 0,
					(float)this->renderTexture.texture.width,
					(float)-this->renderTexture.texture.height },
				(Vector2){ 0, 0 }, WHITE);
		EndShaderMode();

		this->gui->DrawGUI(this->player->Moving(), this->player->Attacking());

		EndDrawing();
	}
	
	this->UnloadGame();
	CloseWindow();
}


void
Game::Render()
{
	BeginTextureMode(this->renderTexture);
	ClearBackground(BLACK);

	BeginMode3D(*this->player->GetCamera());

	this->RenderSkybox();

	SetShaderValue(this->fogShader,
		this->fogShader.locs[SHADER_LOC_VECTOR_VIEW],
		&this->player->GetCamera()->position,
		SHADER_UNIFORM_VEC3);

	if (this->level->IsReady()) {
		this->level
			->GetCurrentLevelModel()
			->materials[0].shader = this->fogShader;
			
		DrawModel(*this->level->GetCurrentLevelModel(),
			(Vector3){0.f, 0.f, 0.f},
			1.f, WHITE);

		this->player->UpdatePlayer(this->level->GetCurrentLevel(),
			this->level->GetLevelMapImage());

		for (auto& sndName : this->player->soundQueue) 
			this->sfxMngr->PlaySFX(sndName);
		this->player->soundQueue.clear();

		if (this->player->shooting) {
			this->level->SpawnBullet(this->sprMngr->GetSprite("fire_ball"),
				this->player->GetCamera()->position,
				true, this->player->GetCamera()->target);
			this->player->shooting = false;
		}

		rlDisableBackfaceCulling();
		this->ProcessObjects();
		this->ProcessEntities();
		this->ProcessParticles();
		this->ProcessBullets();
		rlEnableBackfaceCulling();
		
	}	
		
	EndMode3D();
	EndTextureMode();
}

void
Game::RenderSkybox()
{
	rlDisableBackfaceCulling();
	rlDisableDepthMask();
	DrawModel(*this->skybox->GetModel(), (Vector3){ 0.f, 0.f, 0.f },
		1.f, WHITE);
	rlEnableBackfaceCulling();
	rlEnableDepthMask();
}

void
Game::ProcessObjects()
{
	auto objects = this->level->GetObjects();
	for (auto& obj: *objects) {
		obj->Update(this->player->GetCamera());
		obj->GetModel()->materials[0].shader = this->fogShader;

		switch (obj->GetInteractionMode()) {
		case I_FENCE_BROKEN:
			obj->SetSprite(this->sprMngr->GetSprite("fence_destroyed"));
			break;
		case I_CHEST_OPEN:
			obj->SetSprite(this->sprMngr->GetSprite("chest_open"));
			break;
		case I_DOOR_OPEN:
			obj->SetSprite(this->sprMngr->GetSprite("empty"));
			break;
		default:
			break;
		}

		DrawModel(*obj->GetModel(),
			obj->GetPosition(),
			1.f, WHITE);
	}
}

void
Game::ProcessEntities()
{
	auto entities = this->level->GetEntities();
	for(auto& entity: *entities) {
		entity->Update(this->player->Attacking(),
			this->player->GetCamera(),
			this->level->GetLevelMapImage(),
			this->player->GetInventory());
		entity->GetModel()->materials[0].shader = this->fogShader;

		for(auto& sndName : entity->soundQueue)
			this->sfxMngr->PlaySFX(sndName);
		entity->soundQueue.clear();

		if (entity->tookDamage) {
			entity->tookDamage = false;
			this->level->SpawnParticle(entity->GetPosition());
		}

		if (!entity->IsDead())
			DrawModel(*entity->GetModel(), entity->GetPosition(), 1.f, WHITE);
		}
}

void
Game::ProcessParticles()
{
	this->level->UpdateParticles(this->player->GetCamera(),
		this->player->GetInventory());
	for(auto& particle : *this->level->GetParticles()) {
		DrawModel(*particle->GetModel(), particle->GetPosition(), 1.f, WHITE);
	}
}

void
Game::ProcessBullets()
{
	this->level->UpdateBullets(this->player->Attacking(),
		this->player->GetCamera(), this->player->GetInventory());
	for (auto& bullet : *this->level->GetBullets()) {
		DrawModel(*bullet->GetModel(), bullet->GetPosition(), 1.f, WHITE);
	}
}

void
Game::UnloadGame()
{
	UnloadShader(this->fogShader);
	UnloadShader(this->pixelizerShader);
	UnloadRenderTexture(this->renderTexture);
	delete this->gui;
	delete this->sprMngr;
	delete this->sfxMngr;
	delete this->player;
	delete this->level;
	delete this->skybox;
}
