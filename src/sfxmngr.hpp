#pragma once
#include<raylib.h>
#include<map>
#include<string>

class SFXManager {
public:
	SFXManager();
	~SFXManager();

	void PlaySFX(const char* soundName);
	void StopSFX(const char* soundName);

private:
	std::map<std::string, Sound> sounds;
	static constexpr const char* SFX_RESOURCES[] = {
		"res/sfx/buzz.ogg",
		"res/sfx/enemy_death.ogg",
		"res/sfx/fire_gun.ogg",
		"res/sfx/game_over.ogg",
		"res/sfx/hit_final_boss.ogg",
		"res/sfx/mage_skeleton.ogg",
		"res/sfx/rock_push.ogg",
		"res/sfx/warp.ogg",
		"res/sfx/door_opened.ogg",
		"res/sfx/enemy_shoot.ogg",
		"res/sfx/found_item.ogg",
		"res/sfx/hit_enemy.ogg",
		"res/sfx/hurt.ogg",
		"res/sfx/potion.ogg",
		"res/sfx/vessel_activation.ogg",
		nullptr
	};
};

