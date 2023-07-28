#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Door : public StaticObject {
public:
	Door(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Door() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override;
};
