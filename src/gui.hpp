#pragma once
#include<raylib.h>
#include"sprmngr.hpp"
#include"inventory.hpp"

class GUI {
public:
	GUI(SpriteManager* sprMngr,
		Inventory* playerInventory,
		const int guiStartY,
		bool* gamePaused);
	~GUI();

	void Input();
	void DrawGUI(bool isPlayerMoving, bool isPlayerAttacking);
	void ShowMessage(const char* message, bool messageAutoClose,
		bool gotKey);
private:
	SpriteManager* sprMngr;
	Inventory* inventory;
	int guiStartY;
	Font font;
	Vector2 initialHandItemPos;
	Vector2 handItemPos;
	int damageScreenState = 0;
	
	bool* gamePaused;

	bool showMessage = false;
	char message[256];
	char messageSprite[256];
	double messageStartTime;
	bool messageAutoClose = false;

	void DrawHotbar();
	void DrawDamage();
	void DrawMessage();
	void DrawHandItem(bool isPlayerMoving, bool isPlayerAttacking);
};
