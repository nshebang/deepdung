#include"player.hpp"
#include"utils.hpp"
#include<raymath.h>
#include<cmath>
#include<algorithm>
#include<string>

Player::Player(std::vector<StaticObject*>* objects,
	std::vector<Entity*>* entities,
	std::map<std::string, ObjectData>* objInfo)
{
	this->InitCamera();
	this->objects = objects;
	this->entities = entities;
	this->objInfo = objInfo;
}

Player::~Player() {}

void
Player::UpdatePlayer(int currentLevel, Image* map)
{
	this->currentLevel = currentLevel;

	this->inventory.hp = std::clamp(this->inventory.hp,
		0, this->inventory.maxHp);

	double currentTime = GetTime();
	if (this->attacking &&
		(currentTime - this->attackStart) > this->attackDuration) {
			this->attackEnd = currentTime;
			this->attacking = false;
	}

	this->UpdateCamera();
	this->CheckUseObjects();
	this->CheckCollisions(map);
}

void
Player::InitCamera()
{
	this->camera.position = (Vector3){-3.f, 0.65f, 0.f};
	this->oldPosition = this->camera.position;
	this->camera.target = (Vector3){0.f, 0.f, -32.f};
	this->camera.up = (Vector3){0.f, 1.f, 0.f};
	this->camera.projection = CAMERA_PERSPECTIVE;
	this->camera.fovy = 60.f;
}

void
Player::UpdateCamera()
{
	float dt = GetFrameTime();
	float speedBoost = IsKeyDown(KEY_LEFT_CONTROL)? this->speed / 4.f : 0.f;

	float bobbingVelocity = this->Moving()?
		sinf(GetTime() * 10.f) / 3.25f : 0.f;

	this->oldPosition = this->camera.position;

	UpdateCameraPro(this->GetCamera(),
    	(Vector3){
            IsKeyDown(KEY_W) * (this->speed + speedBoost) * dt -
            IsKeyDown(KEY_S) * (this->speed + speedBoost) * dt,    
            IsKeyDown(KEY_D) * (this->speed + speedBoost) * dt -
            IsKeyDown(KEY_A) * (this->speed + speedBoost) * dt,
			bobbingVelocity * dt
        },
        (Vector3){
            GetMouseDelta().x * 0.1f,
            GetMouseDelta().y * 0.1f,
            0.f
        },
        0.f
	);

	this->GetCamera()->position.y = std::clamp(
		this->GetCamera()->position.y, .58f, .7f);
}

void
Player::CheckUseObjects()
{
	unsigned int selected = this->inventory.selectedItem;
	if (!IsMouseButtonPressed(0) ||
		this->inventory.GetItem(selected)->amount <= 0)
			return;

	double currentTime = GetTime();
	switch(selected) {
	case 0:
		if (this->inventory.hp < this->inventory.maxHp) {
			this->inventory.hp += 6;
			this->soundQueue.push_back("potion");
			this->inventory.GetItem(selected)->amount--;
		}
		break;
	case 1:
		if (!this->attacking && (currentTime - this->attackEnd) >
			this->attackDelay) {
				this->attacking = true;
				this->soundQueue.push_back("hit_enemy");
				this->attackStart = currentTime;
		}
		break;
	case 2:
		if (!this->shooting && (currentTime - this->lastShot) >
			this->shootDelay) {
				this->shooting = true;
				this->lastShot = currentTime;
				this->soundQueue.push_back("fire_gun");
				this->inventory.GetItem(selected)->amount--;
		}
		break;
	case 3:
		if (!this->attacking && (currentTime - this->attackEnd) >
			this->attackDelay) {
				this->attacking = true;
				this->soundQueue.push_back("hit_enemy");
				this->attackStart = currentTime;
		}
		break;
	default:
		TraceLog(LOG_INFO, "you will regret this");
		break;
	}
}

void
Player::CheckCollisions(Image* map)
{
	Vector3 playerPos = this->camera.position;
	BoundingBox playerBox = (BoundingBox){
		(Vector3){	playerPos.x - this->PLAYER_SIZE.x / 2.f,
					playerPos.y - this->PLAYER_SIZE.y / 2.f,
					playerPos.z - this->PLAYER_SIZE.z / 2.f, },
		(Vector3){	playerPos.x + this->PLAYER_SIZE.x / 2.f,
					playerPos.y + this->PLAYER_SIZE.y / 2.f,
					playerPos.z + this->PLAYER_SIZE.z / 2.f, }
	};

	this->CheckWallCollisions(playerBox, map);	
	this->CheckObjectCollisions(playerBox);
	this->CheckEntityCollisions(playerBox);
}

void
Player::CheckWallCollisions(BoundingBox& playerBox, Image* map)
{
	for (int z = 0; z < map->height; z++)
	for (int x = 0; x < map->width; x++) {
		if (GetImageColor(*map, x, z).r != 255)
			continue;

		float xx = (x - map->width / 2.f) + .5f;
		float zz = (z - map->height / 2.f) + .5f;
		BoundingBox wallBox = (BoundingBox){
			(Vector3){ xx - .5f, 0.f, zz - .5f },
			(Vector3){ xx + .5f, 1.f, zz + .5f }
		};

		//DrawBoundingBox(wallBox, RED);

		if (CheckCollisionBoxes(playerBox, wallBox))
			this->camera.position = oldPosition;
	}
}

void
Player::CheckObjectCollisions(BoundingBox& playerBox)
{
	for (auto& obj: *this->objects) {
		if (CheckCollisionBoxes(playerBox, obj->GetBoundingBox()) &&
			obj->IsWall()) 
				this->camera.position = oldPosition;
		
		switch (obj->GetInteractionMode()) {
		case I_CHEST: {
			Vector3 dirVector = Vector3Normalize(Vector3Subtract(
				obj->GetPosition(), this->camera.position));
			float dot = Vector3DotProduct(dirVector, Vector3Normalize(
				this->camera.target));
			float distance = Vector3Distance(obj->GetPosition(),
				this->camera.position);

			if (dot > 0.85f && distance < 1.35f && IsMouseButtonPressed(1)) {
				obj->Interact(this->objInfo, this->currentLevel,
					&this->inventory);
				this->soundQueue.push_back("found_item");
			}
			break;
		}
		case I_COLLISION: {
			if (!CheckCollisionBoxes(playerBox, obj->GetBoundingBox()))
				break;
			this->inventory.warpInvulnerability = true;
			std::string objId = TextFormat("%d,%d,%d", currentLevel,
				static_cast<int>(obj->GetPosition().x - 0.5f),
				static_cast<int>(obj->GetPosition().z - 0.5f));
			this->wantsToChangeLevel = this->objInfo->at(objId).warpLevel;
			this->camera.position.x = this->objInfo->at(objId).warpTarget.x;
			this->camera.position.z = this->objInfo->at(objId).warpTarget.y;
			break;
		}
		case I_DOOR: {
			Vector3 dirVector = Vector3Normalize(Vector3Subtract(
				obj->GetPosition(), this->camera.position));
			float dot = Vector3DotProduct(dirVector, Vector3Normalize(
				this->camera.target));
			float distance = Vector3Distance(obj->GetPosition(),
				this->camera.position);

			if (dot > 0.85f && distance < 1.35f && IsMouseButtonPressed(1)) {
				obj->Interact(this->objInfo, this->currentLevel,
					&this->inventory);
				this->soundQueue.push_back("door_opened");
			}
			break;
		}
		default:
			break;
		}
		//DrawBoundingBox(obj->GetBoundingBox(), RED);
	}
}

void
Player::CheckEntityCollisions(BoundingBox& playerBox)
{
	for(auto& entity: *this->entities)
		if (CheckCollisionBoxes(playerBox, entity->GetBoundingBox())
			&& entity->IsWall())
				this->camera.position = oldPosition;
}

bool
Player::Moving()
{
	return IsKeyDown(KEY_W) || IsKeyDown(KEY_S) ||
		IsKeyDown(KEY_D) || IsKeyDown(KEY_A);
}

