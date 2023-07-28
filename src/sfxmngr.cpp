#include"sfxmngr.hpp"
#include<cassert>

SFXManager::SFXManager()
{
	InitAudioDevice();
	TraceLog(LOG_INFO, "SFXManager: begin loading game sounds");
	unsigned i = 0;
	while(SFX_RESOURCES[i] != nullptr) {
		std::string name(GetFileNameWithoutExt(SFX_RESOURCES[i]));
		Sound sound = LoadSound(SFX_RESOURCES[i]);
		this->sounds.insert({ name, sound });
		i++;
	}
}

SFXManager::~SFXManager()
{
	for (auto& sound: this->sounds) {
		TraceLog(LOG_INFO, "SFXManager: unload %s", sound.first.c_str());
		UnloadSound(sound.second);
		this->sounds.erase(sound.first);
	}
	CloseAudioDevice();
}

void
SFXManager::PlaySFX(const char* soundName)
{
	assert(this->sounds.find(soundName) != this->sounds.end());
	PlaySound(this->sounds.at(soundName));
}

void
SFXManager::StopSFX(const char* soundName)
{
	StopSound(this->sounds.at(soundName));
}

