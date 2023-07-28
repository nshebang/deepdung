#include"chest.hpp"

void
Chest::Interact(std::map<std::string, ObjectData>* objInfo,
	int currentLevel,
	Inventory* inventory)
{
	std::string objId = TextFormat("%d,%d,%d", currentLevel,
		static_cast<int>(this->position.x - 0.5f),
		static_cast<int>(this->position.z - 0.5f));

	if (objInfo->at(objId).itemId == -2) {
		inventory->keys++;
		inventory->gotKey = true;
	} else {
		inventory
			->GetItem(objInfo->at(objId).itemId)
			->amount += objInfo->at(objId).amount;
		inventory->newItem = objInfo->at(objId).itemId;
	}


	this->interactionMode = I_CHEST_OPEN;
	objInfo->at(objId).interactionMode = I_CHEST_OPEN;
}
