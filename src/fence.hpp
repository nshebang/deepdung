#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Fence : public StaticObject {
public:
	Fence(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Fence() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override {}
};
