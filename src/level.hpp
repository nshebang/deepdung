#pragma once
#include<raylib.h>
#include<raymath.h>
#include<vector>
#include<map>
#include<string>
#include"sprmngr.hpp"
#include"utils.hpp"

#include"staticobj.hpp"
#include"prop.hpp"
#include"chest.hpp"
#include"warp.hpp"
#include"fence.hpp"
#include"door.hpp"

#include"entity.hpp"
#include"snake.hpp"
#include"bat.hpp"

#include"particle.hpp"

#include"bullet.hpp"

#include"objectdata.hpp"

#define C_LIME (Color){0, 255, 0, 255}
#define C_YELLOW (Color){255, 255, 0, 255}
#define C_CYAN (Color){0, 255, 255, 255}
#define C_RED (Color){255, 0, 0, 255}
#define C_GRAY_FENCE (Color){150, 150, 150, 255}
#define C_RED_EYE (Color){38, 0, 0, 255}
#define C_PURPLE_DOOR (Color){150, 50, 255, 255}
#define C_PINK_DOOR (Color){255, 0, 150, 255}
#define C_GRASS (Color){0, 150, 0, 255}
#define C_VINES (Color){0, 100, 0, 255}
#define C_EYESTATUE (Color){38, 0 , 25, 255}

class Level {
public:
	Level(SpriteManager* sprMngr);
	~Level();

	inline int GetCurrentLevel() {
		return this->currentLevel;
	}

	inline const char* GetCurrentLevelName() {
		return this->LEVEL_NAMES[this->currentLevel];
	}

	inline bool IsReady() {
		return IsModelReady(this->model) && IsTextureReady(this->texture);
	}

	inline Model* GetCurrentLevelModel() {
		return &this->model;
	}

	inline std::vector<StaticObject*>* GetObjects() {
		return &this->objects;
	}

	inline std::vector<Entity*>* GetEntities() {
		return &this->entities;
	}	

	inline std::map<std::string, ObjectData>* GetObjsInfo() {
		return &this->objInfo;
	}

	inline std::vector<Particle*>* GetParticles() {
		return &this->particles;
	}

	inline std::vector<Bullet*>* GetBullets() {
		return &this->bullets;
	}

	inline Vector2 GetLevelDimensions() {
		return (Vector2) {
			(float)this->levelWidth,
			(float)this->levelHeight
		};
	}

	inline Image* GetLevelMapImage() {
		return &this->map;
	}
	
	void SpawnParticle(Vector3 position);
	void UpdateParticles(Camera* camera, Inventory* inventory);
	void SpawnBullet(Sprite* sprite, Vector3 position, bool playerIsOwner,
		Vector3 direction);
	void UpdateBullets(bool isPlayerAttacking, Camera* camera,
		Inventory* inventory);
	void LevelGoto(const char* newLevel);

	friend bool operator==(const Color& color1, const Color& color2);
	friend bool operator!=(const Color& color1, const Color& color2);

	static const int MAX_LEVELS = 5;
	const char* LEVEL_NAMES[MAX_LEVELS] = {
		"the_nexus",
		"the_shrine",
		"bat_cave",
		"dark_jungle",
		"holenest",
	};
private:
	int currentLevel;

	int lastLevel = -1;
	int levelWidth = 0;
	int levelHeight = 0;

	Image map;
	Model model;
	Mesh mesh;
	Texture2D texture;
	SpriteManager* sprMngr;

	std::vector<StaticObject*> objects;
	std::vector<Entity*> entities;

	std::vector<Particle*> particles;
	std::vector<Bullet*> bullets;

	// key is level,imgX,imgY ("0,5,4": (ObjectInfo){})
	std::map<std::string, ObjectData> objInfo;	

	void LoadNewLevel();
	void LoadLevelModel();
	void LoadLevelTexture();
	void LoadLevelObjectsAndEntities();
	void LoadLevelObjAt(int x, int y, Color color);
	void RegisterLevelObjectIfNotExists(int x, int y, std::string objId);
	ObjectData GetLevelObjectData(int x, int y);
	void UnloadLevelObjects();
	void UnloadLevelEntities();
	void UnloadLevelParticles();
	void UnloadLevelBullets();
	void UnloadLevelResources();
};
