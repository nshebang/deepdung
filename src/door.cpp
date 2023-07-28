#include"door.hpp"

void
Door::Interact(std::map<std::string, ObjectData>* objInfo,
	int currentLevel,
	Inventory* inventory)
{
	if (inventory->keys <= 0)
		return;
	
	std::string objId = TextFormat("%d,%d,%d", currentLevel,
		static_cast<int>(this->position.x - 0.5f),
		static_cast<int>(this->position.z - 0.5f));

	inventory->keys--;
	this->ToggleWallMode();	

	this->interactionMode = I_DOOR_OPEN;
	objInfo->at(objId).interactionMode = I_DOOR_OPEN;
}
