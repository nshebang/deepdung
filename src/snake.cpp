#include"snake.hpp"
#include<raymath.h>
#include<cmath>

void
Snake::Behavior(bool isPlayerAttacking, Camera* camera,
	Image* map, Inventory* inventory)
{
	float dt = GetFrameTime();

	this->CheckCollisions(map);

	float distanceToPlayer = Vector3Distance(this->position,
		camera->position);
	double attackSpeed = 3;
	double currentTime = GetTime();
	if ((currentTime - this->lastAttack) > attackSpeed &&
		distanceToPlayer < 1.4f && !inventory->warpInvulnerability) {
			inventory->hp -= 2;
			inventory->tookDamage = true;
			this->soundQueue.push_back("hurt");
			this->lastAttack = currentTime;
	}
	if ((currentTime - this->lastDamage) > 0.5 &&
		distanceToPlayer < 1.8f && isPlayerAttacking) {
			this->lastDamage = currentTime;
			this->tookDamage = true;
			this->soundQueue.push_back("hit_enemy");
			this->hp--;
	}
	if (this->hp <= 0) {
		this->soundQueue.push_back("enemy_death");
		this->dead = true;
	}

	if ((currentTime - this->lastDirChange) > 6) {
		this->moveFactor = (float)GetRandomValue(1, 3);
		this->lastDirChange = currentTime;
	}

	this->velocity = (Vector3){
		cosf(GetTime() * this->moveFactor) * 2.f * dt,
		0.f,
		sinf(GetTime() * this->moveFactor) * 2.f * dt
	};
}

void
Snake::CheckCollisions(Image* map)
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

		if (CheckCollisionBoxes(this->boundingBox, wallBox))
			this->position = this->oldPosition;
	}

	for (auto& bullet : *this->bullets)
		if (CheckCollisionBoxes(this->boundingBox, bullet->GetBoundingBox())) {
			this->hp -= 4;
			this->tookDamage = true;
			this->soundQueue.push_back("hit_enemy");
			bullet->queuedForDeletion = true;
		}
}

