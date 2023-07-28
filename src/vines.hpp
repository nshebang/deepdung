#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Vines : public StaticObject {
public:
	Vines(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Vines() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override {}
};
