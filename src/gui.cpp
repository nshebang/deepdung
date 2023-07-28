#include"gui.hpp"
#include<raymath.h>
#include<cmath>
#include<ctime>
#include<algorithm>

GUI::GUI(SpriteManager* sprMngr,
	Inventory* playerInventory,
	int guiStartY, bool* gamePaused)
{
	this->sprMngr = sprMngr;
	this->inventory = playerInventory;
	this->guiStartY = guiStartY;
	this->initialHandItemPos = (Vector2){
		GetScreenWidth() - 360.f,
		this->guiStartY - 305.f
	};
	this->handItemPos = this->initialHandItemPos;
	this->gamePaused = gamePaused;

	TextCopy(this->messageSprite, "");
	this->font = LoadFont("res/fonts/romulus.png");
	SetRandomSeed(time(NULL));
}

GUI::~GUI()
{
	UnloadFont(this->font);
}

void
GUI::Input()
{
	unsigned int& selected = this->inventory->selectedItem;
	unsigned int keys[5] = {KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE};
	
	for(unsigned int i = 0; i < this->inventory->GetMaxItems(); i++)
		if(IsKeyPressed(keys[i]))
			selected = i;

	float scrolling = -GetMouseWheelMove();
	if (scrolling) {
		selected -= (scrolling > 0 &&
			selected == this->inventory->GetMaxItems() - 1)?
				this->inventory->GetMaxItems() : 0;
		selected += (scrolling < 0 && selected <= 0)?
				this->inventory->GetMaxItems() : 0;
		selected += scrolling < 0? -1 : 1;
	}
}

void
GUI::DrawGUI(bool isPlayerMoving, bool isPlayerAttacking)
{
	this->DrawHandItem(isPlayerMoving, isPlayerAttacking);

	DrawRectangleGradientV(
		0, this->guiStartY, GetScreenWidth(), GetScreenHeight(),
		(Color){20, 20, 20, 255}, BLACK);

	DrawTextEx(this->font,
		TextFormat("%02d\t\t\t%02d\n%02d",
			this->inventory->hp,
			this->inventory->soulParts,
			this->inventory->keys),
		(Vector2){ 48.f, this->guiStartY + 8.f },
		this->font.baseSize * 2.f, 3,
		this->damageScreenState? RED : WHITE);

	DrawTextureEx(*sprMngr->GetSprite("heart")->GetFrameTexture(),
		(Vector2){ 16.f, this->guiStartY + 10.f },
		0.f, 1.f, WHITE);

	DrawTextureEx(*sprMngr->GetSprite("key")->GetFrameTexture(),
		(Vector2){ 16.f, this->guiStartY + 48.f },
		0.f, 1.f, WHITE);

	DrawTextureEx(*sprMngr->GetSprite("soul")->GetFrameTexture(),
		(Vector2){ 96.f, this->guiStartY + 10.f },
		0.f, 1.f, WHITE);

	if (this->inventory->newItem > -1) {
		this->showMessage = true;
		
		TextCopy(this->message, TextFormat("You found %s.\n\"%s\"",
			this->inventory->GetItem(this->inventory->newItem)->name,
			this->inventory->GetItem(this->inventory->newItem)->desc));
		TextCopy(this->messageSprite,
			this->inventory->GetItem(this->inventory->newItem)->sprName);

		this->messageStartTime = GetTime();
		this->inventory->newItem = -1;
	}

	this->DrawHotbar();
	this->DrawDamage();	
	this->DrawMessage();
}

void
GUI::ShowMessage(const char* message, bool messageAutoClose, bool gotKey)
{
	this->showMessage = true;
	this->messageStartTime = GetTime();
	this->messageAutoClose = messageAutoClose;
	TextCopy(this->message, message);
	if (gotKey)
		TextCopy(this->messageSprite, "key");
}

void
GUI::DrawHotbar()
{
	for(unsigned i = 0; i < this->inventory->GetMaxItems(); i++) {
		int posX = ((GetScreenWidth() / 2) - 176) + (72 * i);
		
		if (this->inventory->selectedItem == i)
			DrawRectangle(posX - 2,	this->guiStartY + 6, 68, 68, WHITE);
		DrawRectangle(posX, this->guiStartY + 8, 64, 64, BLACK);

		if(this->inventory->GetItem(i)->amount < 0)
			continue;

		DrawTextureEx(*sprMngr
			->GetSprite(this->inventory->GetItem(i)->sprName)
			->GetFrameTexture(),
			(Vector2){
				(float)(((GetScreenWidth() / 2) - 176) + (72 * i)),
				(float)this->guiStartY + 8
			},
			0.f, 4.f, WHITE);
		
		if (this->inventory->GetItem(i)->maxAmount > 1)
			DrawTextEx(this->font,
				TextFormat("%d", this->inventory->GetItem(i)->amount),
				(Vector2){ (float)posX, this->guiStartY + 50.f },
				this->font.baseSize * 2.f, 3, WHITE);
	}
}

void
GUI::DrawDamage()
{
	// this function is just a simple damage screen effect
	// i think it's too complex for a simple effect and i don't like that but
	// it works for now

	if (this->inventory->tookDamage) {
		this->damageScreenState = 20;	
		this->inventory->tookDamage = false;
	}

	Color colors[4] = {BLACK, RED, MAROON, BLACK};
	if (this->damageScreenState) {
		Vector2 center = { GetScreenWidth() / 2.f,
			(GetScreenHeight() - 80.f) / 2};
		float maxDist = Vector2Distance(center, (Vector2){ 0.f, 0.f });

		for (int y = 0; y < guiStartY; y += 2)
		for (int x = 0; x < GetScreenWidth(); x += 2) {
			float distToCenter = Vector2Distance(center, (Vector2){
				(float)x, (float)y }) + this->damageScreenState * 10.f;
			float factor = Normalize(distToCenter, 0.f, maxDist);
			int randNum = GetRandomValue(0, (int)(factor * 100));
			int randCol = GetRandomValue(0, 3);
			if (randNum > 45)
				DrawRectangle(x, y, 2, 2, colors[randCol]);
		}

		this->damageScreenState--;
	}
}

void
GUI::DrawHandItem(bool isPlayerMoving, bool isPlayerAttacking)
{
	this->handItemPos.x = std::clamp(this->handItemPos.x - 
		(sinf(GetMouseDelta().x * 18.f) / 2.f) * 2.f,
			GetScreenWidth() - 400.f,
			GetScreenWidth() + 160.f);
	if (isPlayerMoving)
		this->handItemPos.y = std::clamp(
			this->handItemPos.y + sinf(GetTime() * 12.f) / 2.f,
			this->initialHandItemPos.y - 20.f,
			this->initialHandItemPos.y + 25.f);

	unsigned int selected = this->inventory->selectedItem;
	if (this->inventory->GetItem(selected)->amount < 0) {
		DrawTextureEx(*sprMngr->GetSprite("hand")->GetFrameTexture(),
			this->handItemPos, 0.f, 20.f, WHITE);
		return;
	}

	Vector2 offset = (Vector2){ 0.f, 0.f };
	float rotation = 0.f, scale = 20.f;
	switch(selected) {
	case 0:
		scale = 14.f;
		offset.x = 72.f;
		offset.y = 156.f;
		rotation = -15.f;
		break;
	case 1:
		break;
	case 2:
		offset.x = 16.f;
		offset.y = 48.f;
		break;
	case 3:
		break;
	case 4:
		scale = 10.f;
		offset.x = 272.f;
		offset.y = 332.f;
		rotation = -180.f;
		break;
	default:
		break;
	}

	char handSpr[256];
	TextCopy(handSpr, !isPlayerAttacking?
		this->inventory->GetItem(this->inventory->selectedItem)->handSprName :
		this->inventory->GetItem(this->inventory->selectedItem)->meleeSprName);
	DrawTextureEx(*sprMngr->GetSprite(handSpr)->GetFrameTexture(),
		Vector2Add(this->handItemPos, offset), rotation, scale, WHITE);
}

void
GUI::DrawMessage()
{
	if (!this->showMessage)
		return;
	if (!*this->gamePaused)
		*this->gamePaused = true;

	DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight() - 80,
		(Color){0, 0, 0, 90});

	Vector2 textSize = MeasureTextEx(this->font, this->message,
		this->font.baseSize * 4.f, 3);
	DrawTextEx(this->font, this->message,
		(Vector2){ (GetScreenWidth() - textSize.x) / 2.f, 180.f },
		this->font.baseSize * 4.f, 3, WHITE);

	if (!TextIsEqual(this->messageSprite, ""))
		DrawTextureEx(*this->sprMngr
			->GetSprite(this->messageSprite)
			->GetFrameTexture(),
			(Vector2){ (GetScreenWidth() / 2.f) - 64.f, 32.f },
			0.f, 8.f, WHITE);

	double currentTime = GetTime();
	if ((currentTime - messageStartTime) > 0.5 && (GetKeyPressed() != 0 ||
		IsMouseButtonPressed(0) || IsMouseButtonPressed(1) ||
		messageAutoClose)) {
			this->showMessage = false;
			this->messageAutoClose = false;
			TextCopy(this->messageSprite, "");
			*this->gamePaused = false;
	}
}


