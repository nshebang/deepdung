#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Warp : public StaticObject {
public:
	Warp(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Warp() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override;
};
