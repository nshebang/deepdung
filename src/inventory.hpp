#pragma once
#include<map>

class Inventory;

typedef struct InventoryItem {
	const char* name;
	const char* desc;
	const char* sprName;
	const char* handSprName;
	const char* meleeSprName;
	bool isMelee;
	int amount;
	int maxAmount;
	//void(Inventory::* useItem)(void);   old impl; might reconsider using it
} InventoryItem;


class Inventory {
public:
	Inventory() {}
	~Inventory() {}

	int hp = 30;
	int maxHp = 30;
	unsigned keys = 0;
	unsigned soulParts = 0;
	
	inline InventoryItem* GetItem(unsigned int id) {
		return &this->inventory.at(id);
	}

	//inline void UseItem(unsigned int id) {
	//	auto fnptr = this->inventory.at(id).useItem;
	//	(this->*fnptr)();
	//}

	inline const unsigned GetMaxItems() {
		return this->MAX_ITEMS;
	}

	unsigned int selectedItem = 0;
	bool tookDamage = false;
	bool warpInvulnerability = false;
	int newItem = -1;
	bool gotKey = false;
private:
	const unsigned MAX_ITEMS = 5;
	std::map<unsigned int, InventoryItem> inventory = {
		{0, {"potion", "This isn't over yet",
			"potion", "potion", "potion", false,
			-1, 10}},
		
		{1, {"the knife", "Don't get too close to me",
			"knife", "knife_melee", "knife_melee_attacking", true,
			-1, 1}},
		
		{2, {"fire gun", "No more fences. Burn it all.",
			"fire_gun", "fire_gun", "fire_gun", false,
			25, 25}},
		
		{3, {"the force glove", "I will push the heaviest rocks",
			"glove", "glove_melee", "glove_melee_attacking",
			true, 1, 1}},
		
		{4, {"the shrine key", "Do I really want to?",
			"shrine_key", "shrine_key", "shrine_key", false,
			-1, 1}}
	};
};

