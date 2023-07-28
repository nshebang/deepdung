#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Chest : public StaticObject {
public:
	Chest(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Chest() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override;
};
