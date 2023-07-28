#include"sprite.hpp"

Sprite::Sprite(const char* filename, unsigned int maxFrames)
{
	Image image = LoadImage(filename);

	for(unsigned int i = 0; i < maxFrames; i++) {
		Image tmpImg = ImageCopy(image);
		ImageCrop(&tmpImg, (Rectangle){
			(float)(i * SPRITE_WIDTH), 0.f,
			SPRITE_WIDTH, SPRITE_HEIGHT
		});
		this->frames.push_back(LoadTextureFromImage(tmpImg));
		UnloadImage(tmpImg);
	}

	UnloadImage(image);
}

Texture*
Sprite::GetFrameTexture()
{
	return &this->frames[this->currentFrame];
}

void
Sprite::TickAnimation()
{
	if (this->GetMaxFrames() == 1 || this->animationSpeed == 0)
		return;

	double currentTime = GetTime();

	if ((currentTime - this->lastTick) > this->animationSpeed) {
		this->NextFrame();
		this->lastTick = currentTime;
	}
}

void
Sprite::NextFrame()
{
	if (this->currentFrame < (this->GetMaxFrames() - 1))
		this->currentFrame++;
	else
		this->currentFrame = 0;
}

Sprite::~Sprite()
{
	for (Texture tex : this->frames) {
		UnloadTexture(tex);
	}
}
