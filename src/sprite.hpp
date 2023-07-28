#pragma once
#include<raylib.h>
#include<vector>

#define SPRITE_WIDTH 16
#define SPRITE_HEIGHT 16

class Sprite {
public:
	Sprite(const char* filename, unsigned int maxFrames);
	~Sprite();

	Texture* GetFrameTexture();
	void TickAnimation();

	inline unsigned int GetMaxFrames() {
		return this->frames.size();
	}

	unsigned int currentFrame = 0;
	double animationSpeed = 0.5;

private:
	void NextFrame();

	double lastTick = 0.0;
	std::vector<Texture> frames;
};
