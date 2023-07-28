#include"bullet.hpp"
#include<raymath.h>

void
Bullet::Behavior(bool isPlayerAttacking, Camera* camera,
	Image* map, Inventory* inventory)
{
	float dt = GetFrameTime();
	
	this->boxSize = 0.25f;

	this->CheckCollisions(map);
	this->velocity = (Vector3){
		this->direction.x * 18.f * dt,
		0.f,
		this->direction.z * 18.f * dt
	};
}

void
Bullet::CheckCollisions(Image* map)
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

		if (CheckCollisionBoxes(this->boundingBox, wallBox)) {
			this->queuedForDeletion = true;
		}
	}
	
	for (auto& obj : *this->objects) {
		if (CheckCollisionBoxes(this->boundingBox, obj->GetBoundingBox()) &&
			obj->IsWall()) {
				if (obj->GetInteractionMode() == I_FENCE) {
					obj->ToggleWallMode();
					obj->SetInteractionMode(I_FENCE_BROKEN);
				}
				this->queuedForDeletion = true;
		}
	}
}

