#pragma once
#include<raylib.h>
#include"staticobj.hpp"

class Prop : public StaticObject {
public:
	Prop(Sprite* sprite, Vector3 position,
		bool isWall, InteractionMode interactionMode) :
			StaticObject(sprite, position, isWall, interactionMode) {}
	~Prop() {}

	void Interact(std::map<std::string, ObjectData>* objInfo,
		int currentLevel, Inventory* inventory) override {}
};
