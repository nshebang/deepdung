#include"sprmngr.hpp"
#include<utility>

SpriteManager::SpriteManager()
{
	this->LoadSprites();	
}

SpriteManager::~SpriteManager()
{
	this->UnloadSprites();
}

void
SpriteManager::LoadSprites()
{
	TraceLog(LOG_INFO, "-- Begin loading game sprites (%d) --",
		this->SPRITE_ASSETS.size());

	for(auto& sprInfo: this->SPRITE_ASSETS) {
		Sprite* sprite = new Sprite(sprInfo.first.c_str(), sprInfo.second);
		std::string name(GetFileNameWithoutExt(sprInfo.first.c_str()));
		this->sprites.insert({
			name,
			sprite
		});
	}
}

Sprite*
SpriteManager::GetSprite(const char* name)
{
	std::string nameStr(name);
	return this->sprites.at(nameStr);
}

void
SpriteManager::TickSpriteAnimations()
{
	for (auto& sprite: this->sprites)
		sprite.second->TickAnimation();
}

void
SpriteManager::UnloadSprites()
{
	for (auto& sprite: this->sprites) {
		delete sprite.second;
		this->sprites.erase(sprite.first);
	}
}
