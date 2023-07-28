#pragma once
#include<raylib.h>
#include<map>
#include<string>
#include"objectdata.hpp"
#include"inventory.hpp"
#include"sprite.hpp"

class StaticObject {
public:
	StaticObject(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode);
	virtual ~StaticObject();

	void Update(Camera* camera);
	virtual void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel,
		Inventory* inventory) = 0;

	inline Model* GetModel() {
		return &this->model;
	}

	inline void SetSprite(Sprite* sprite) {
		this->sprite = sprite;
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

	inline InteractionMode GetInteractionMode() {
		return this->interactionMode;
	}

	inline void ToggleWallMode() {
		this->isWall = !this->isWall;
	}

	inline void SetInteractionMode(InteractionMode interactionMode) {
		this->interactionMode = interactionMode;
	}

private:
	Model model;
	Mesh mesh;
	bool isWall;

protected:
	Vector3 position;
	Sprite* sprite;
	InteractionMode interactionMode;
	BoundingBox boundingBox;
};
