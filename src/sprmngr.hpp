#pragma once
#include<raylib.h>
#include<map>
#include<string>
#include"sprite.hpp"

#define FRAME_WIDTH 16
#define FRAME_HEIGHT 16
#define MAX_SPRITES 3

class SpriteManager {
public:
	SpriteManager();
	~SpriteManager();

	Sprite* GetSprite(const char* name);
	void TickSpriteAnimations();

private:
	void LoadSprites();
	void UnloadSprites();

	const std::map<std::string, const unsigned int> SPRITE_ASSETS = {
		{"res/textures/icons/potion.png", 1},
		{"res/textures/icons/knife.png", 1},
		{"res/textures/icons/fire_gun.png", 1},
		{"res/textures/icons/glove.png", 1},
		{"res/textures/icons/shrine_key.png", 1},
		{"res/textures/icons/heart.png", 1},
		{"res/textures/icons/key.png", 1},
		{"res/textures/icons/soul.png", 1},
		{"res/textures/tree.png", 1},
		{"res/textures/vines.png", 1},
		{"res/textures/grass.png", 1},
		{"res/textures/eye_statue.png", 1},
		{"res/textures/chest.png", 1},
		{"res/textures/chest_open.png", 1},
		{"res/textures/fence.png", 1},
		{"res/textures/fence_destroyed.png", 1},
		{"res/textures/ladder_down.png", 1},
		{"res/textures/ladder_up.png", 1},
		{"res/textures/door.png", 1},
		{"res/textures/door_boss.png", 1},
		{"res/textures/hand.png", 1},
		{"res/textures/knife_melee.png", 1},
		{"res/textures/knife_melee_attacking.png", 1},
		{"res/textures/glove_melee.png", 1},
		{"res/textures/glove_melee_attacking.png", 1},
		{"res/textures/hand.png", 1},
		{"res/textures/snake.png", 2},
		{"res/textures/bat.png", 2},
		{"res/textures/fire_ball.png", 3},
		{"res/textures/hornet_bullet.png", 1},
		{"res/textures/attack.png", 3},
		{"res/textures/empty.png", 1}
	};
	std::map<std::string, Sprite*> sprites;
};
