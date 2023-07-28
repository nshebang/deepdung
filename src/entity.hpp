#pragma once
#include<raylib.h>
#include<vector>
#include"sprite.hpp"
#include"inventory.hpp"

class Entity {
public:
	Entity(Sprite* sprite, Vector3 position, bool isWall, unsigned hpMax);
	virtual ~Entity();

	void Update(bool isPlayerAttacking, Camera* camera,
		Image* map, Inventory* inventory);
	virtual void Behavior(bool isPlayerAttacking, Camera* camera,
		Image* map, Inventory* inventory) = 0;

	inline Model* GetModel() {
		return &this->model;
	}

	inline void SetPosition(Vector3 position) {
		this->position = position;
	}

	inline Vector3 GetPosition() {
		return this->position;
	}	

	inline BoundingBox GetBoundingBox() {
		return this->boundingBox;
	}

	inline bool IsWall() {
		return this->isWall;
	}

	inline bool IsDead() {
		return this->dead;
	}

	bool tookDamage = false;

	// sfx
	std::vector<const char*> soundQueue;

private:
	Model model;
	Mesh mesh;
	Sprite* sprite;
	bool isWall;

protected:
	BoundingBox boundingBox;
	float boxSize = 0.4f;
	Vector3 oldPosition;
	Vector3 position;
	Vector3 velocity = (Vector3){0.f, 0.f, 0.f};
	unsigned state = 0;
	double lastAttack = 0;
	double lastDirChange = 0;
	int hp;
	int hpMax;

	double lastDamage = 0;

	bool dead = false; // entities created on level start won't be immediately
					   // deleted from memory, instead they "die"
};
