#pragma once

enum InteractionMode {
	I_NONE,
	I_CHEST,
	I_CHEST_OPEN,
	I_COLLISION,
	I_FENCE,
	I_FENCE_BROKEN,
	I_DOOR,
	I_DOOR_OPEN,
};

enum WarpType {
	INVISIBLE,
	LADDER_DOWN,
	LADDER_UP,
};

typedef struct ObjectData {
	InteractionMode interactionMode; 
	int itemId;
	int amount;
	int warpLevel;
	Vector2 warpTarget;
	WarpType warpType;
} ObjectData;

