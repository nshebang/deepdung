#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class EyeStatue : public StaticObject {
public:
	EyeStatue(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~EyeStatue() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override {}
};
