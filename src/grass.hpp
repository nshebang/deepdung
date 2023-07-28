#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Grass : public StaticObject {
public:
	Grass(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Grass() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override {}
};
