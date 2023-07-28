#pragma once
#include<raylib.h>
#include<vector>
#include<map>
#include<string>
#include"inventory.hpp"
#include"staticobj.hpp"
#include"entity.hpp"
#include"objectdata.hpp"

class Player {
public:
	Player(std::vector<StaticObject*>* objects,
		std::vector<Entity*>* entities,
		std::map<std::string, ObjectData>* objInfo);
	~Player();

	inline Camera* GetCamera() {
		return &this->camera;
	}

	inline Inventory* GetInventory() {
		return &this->inventory;
	}

	inline bool Colliding() {
		return this->colliding;
	}

	inline bool Attacking() {
		return this->attacking;
	}

	void UpdatePlayer(int currentLevel, Image* map);
	bool Moving();

	bool shooting = false;
	int wantsToChangeLevel = -1;

	//sfx
	std::vector<const char*> soundQueue;
private:
	const Vector3 PLAYER_SIZE = (Vector3){.25f, .65f, .25f};
	Camera camera = {0};
	CameraMode cameraMode = CAMERA_FIRST_PERSON;
	int speed = 6.f;
	Vector3 oldPosition;
	bool colliding = false;
	Inventory inventory;

	bool attacking = false;
	double attackStart = 0;
	double attackEnd = 0;
	double attackDuration = 0.25;
	double attackDelay = 0.35;

	double lastShot = 0;
	double shootDelay = 0.35;

	// level related stuff
	int currentLevel;
	std::vector<StaticObject*>* objects;
	std::vector<Entity*>* entities;
	std::map<std::string, ObjectData>* objInfo;

	void InitCamera();
	void UpdateCamera();
	void CheckUseObjects();
	void CheckCollisions(Image* map);
	void CheckWallCollisions(BoundingBox& playerBox, Image* map);
	void CheckObjectCollisions(BoundingBox& playerBox);
	void CheckEntityCollisions(BoundingBox& playerBox);
};
