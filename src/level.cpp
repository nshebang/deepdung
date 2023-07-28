#include"level.hpp"
#include<cstring>
#include<cassert>
#include<algorithm>
#include<iterator>

Level::Level(SpriteManager* sprMngr)
{
	this->sprMngr = sprMngr;
	this->currentLevel = 0;
	LoadNewLevel();
}

Level::~Level() {
	this->UnloadLevelObjects();
	this->UnloadLevelEntities();
	this->UnloadLevelParticles();
	this->UnloadLevelBullets();
	this->UnloadLevelResources();
}

bool operator==(const Color& color1, const Color& color2) {
	return (
		color1.r == color2.r &&
		color1.g == color2.g &&
		color1.b == color2.b &&
		color1.a == color2.a
	);
}

bool operator!=(const Color& color1, const Color& color2) {
	return (
		color1.r != color2.r ||
		color1.g != color2.g ||
		color1.b != color2.b ||
		color1.a != color2.a
	);
}

void
Level::SpawnParticle(Vector3 position) {
	Particle* particle = new Particle(this->sprMngr->GetSprite("attack"),
		position, false);
	this->sprMngr->GetSprite("attack")->animationSpeed = 0.15;
	this->particles.push_back(particle);
}

void
Level::UpdateParticles(Camera* camera, Inventory* inventory)
{
	for (unsigned i = 0; i < this->particles.size(); i++) {
		this->particles[i]->Update(false, camera,
			this->GetLevelMapImage(), inventory);
		if (this->particles[i]->queuedForDeletion) {
			delete this->particles[i];
			this->particles.erase(this->particles.begin() + i);
		}
	}
}

void
Level::SpawnBullet(Sprite* sprite, Vector3 position,
	bool playerIsOwner, Vector3 direction)
{
	Bullet* bullet = new Bullet(sprite, position, false, this->GetObjects());
	bullet->direction = Vector3Normalize(direction);
	this->bullets.push_back(bullet);
}

void
Level::UpdateBullets(bool isPlayerAttacking, Camera* camera,
	Inventory* inventory)
{
	for(unsigned i = 0; i < this->bullets.size(); i++) {
		this->bullets[i]->Update(isPlayerAttacking, camera, &this->map,
			inventory);
		if (this->bullets[i]->queuedForDeletion) {
			delete this->bullets[i];
			this->bullets.erase(this->bullets.begin() + i);
		}
	}
}

void
Level::LevelGoto(const char* newLevel)
{
	bool validLvl = false;
	for (int i = 0; i < this->MAX_LEVELS; i++)
		if (TextIsEqual(newLevel, this->LEVEL_NAMES[i])) {
			this->lastLevel = this->currentLevel;
			this->currentLevel = i;
			validLvl = true;
		}

	assert(validLvl);
	UnloadMesh(this->model.meshes[0]);
	LoadNewLevel();
}

void
Level::LoadNewLevel()
{
	TraceLog(LOG_INFO, "Loading level %s",
		this->LEVEL_NAMES[this->currentLevel]);

	this->model = {0};
	this->texture = {0};
	this->UnloadLevelObjects();
	this->UnloadLevelEntities();
	this->UnloadLevelParticles();
	this->UnloadLevelBullets();
	assert(this->lastLevel != this->currentLevel);

	this->LoadLevelModel();
	this->LoadLevelTexture();
	this->LoadLevelObjectsAndEntities();
}

void
Level::LoadLevelModel()
{
	char mapPath[256];
	TextCopy(mapPath, TextFormat("res/maps/%s.png",
		this->LEVEL_NAMES[this->currentLevel]));

	TraceLog(LOG_INFO, "Loading map model %s", mapPath);
	this->map = LoadImage(mapPath);
	this->levelWidth = this->map.width;
	this->levelHeight = this->map.height;
	this->mesh = GenMeshCubicmap(this->map, (Vector3) {1.f, 1.f, 1.f});
	this->model = LoadModelFromMesh(this->mesh);
	CenterModelTransform(this->model);
}

void
Level::LoadLevelTexture()
{
	char texturePath[256];
	TextCopy(texturePath, TextFormat("res/textures/%s-texture.png",
		this->LEVEL_NAMES[this->currentLevel]));
	
	TraceLog(LOG_INFO, "Loading texture %s", texturePath);
	this->texture = LoadTexture(texturePath);
	SetModelTexture(this->model, this->texture);	
}

void
Level::LoadLevelObjectsAndEntities()
{
	char objmapPath[256];
	TextCopy(objmapPath, TextFormat("res/maps/%s-objs.png",
		this->LEVEL_NAMES[this->currentLevel]));	

	TraceLog(LOG_INFO, "Loading level objects");
	Image image = LoadImage(objmapPath);
	assert(image.width == this->levelWidth &&
		image.height == this->levelHeight); 

	for (int y = 0; y < this->levelHeight; y++)
	for (int x = 0; x < this->levelWidth; x++) {
		Color color = GetImageColor(image, x, y);
		if (color != BLACK && color != WHITE)
			this->LoadLevelObjAt(x, y, color);
	}

	UnloadImage(image);
}

void
Level::LoadLevelObjAt(int x, int y, Color color)
{
	x -= this->levelWidth / 2;
	y -= this->levelHeight / 2;
	
	std::string objId = TextFormat("%d,%d,%d",
		this->currentLevel, x, y);

	if (color == C_LIME) {
		this->objects.push_back(new Prop(
			this->sprMngr->GetSprite("tree"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			true, I_NONE
		));
	} else if (color == C_GRASS) {
		this->objects.push_back(new Prop(
			this->sprMngr->GetSprite("grass"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			false, I_NONE
		));
	} else if (color == C_VINES) {
		this->objects.push_back(new Prop(
			this->sprMngr->GetSprite("vines"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			false, I_NONE
		));
	} else if (color == C_EYESTATUE) {
		this->objects.push_back(new Prop(
			this->sprMngr->GetSprite("eye_statue"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			true, I_NONE
		));
	} else if (color == C_RED) {
		switch(this->currentLevel){
		case 0:
		case 3:
			this->entities.push_back(new Snake(
				this->sprMngr->GetSprite("snake"),
				(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
				true, &this->bullets
			));
			break;
		case 2:
			this->entities.push_back(new Bat(
				this->sprMngr->GetSprite("bat"),
				(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
				true, &this->bullets
			));
			this->sprMngr->GetSprite("bat")->animationSpeed = 0.25;
			break;
		default:
			break;
		}
	} else if (color == C_YELLOW) {
		this->RegisterLevelObjectIfNotExists(x, y, objId);

		this->objects.push_back(new Chest(
			this->sprMngr->GetSprite("chest"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			true, this->objInfo.at(objId).interactionMode
		));
	} else if (color == C_GRAY_FENCE) {
		this->objects.push_back(new Fence(
			this->sprMngr->GetSprite("fence"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			true, I_FENCE
		));
	} else if (color == C_CYAN) {
		this->RegisterLevelObjectIfNotExists(x, y, objId);
		
		char sprName[256];
		switch(this->objInfo.at(objId).warpType) {
		case LADDER_UP:
			TextCopy(sprName, "ladder_up");
			break;
		case LADDER_DOWN:
			TextCopy(sprName, "ladder_down");
			break;
		default:
			TextCopy(sprName, "empty");
			break;
		}

		this->objects.push_back(new Warp(
			this->sprMngr->GetSprite(sprName),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			true, this->objInfo.at(objId).interactionMode
		));
	} else if (color == C_PURPLE_DOOR) {
		this->RegisterLevelObjectIfNotExists(x, y, objId);

		this->objects.push_back(new Door(
			this->sprMngr->GetSprite("door"),
			(Vector3){(float)x + 0.5f, 0.5f, (float)y + 0.5f},
			this->objInfo.at(objId).interactionMode == I_DOOR?
			true : false, this->objInfo.at(objId).interactionMode
		));
	}
}

void
Level::RegisterLevelObjectIfNotExists(int x, int y, std::string objId)
{
	if (this->objInfo.find(objId) == this->objInfo.end())
		this->objInfo.insert({objId, GetLevelObjectData(x, y) });
}

ObjectData
Level::GetLevelObjectData(int x, int y) 
{
	ObjectData objData;
	int lx = x + (this->levelWidth / 2), // world coords to img coords
		ly = y + (this->levelHeight / 2);

	switch(this->currentLevel) {
	case 0:
		if ((lx == 7 && ly == 6) || (lx == 31 && ly == 32)) {
			objData.interactionMode = I_CHEST;
			objData.itemId = 0;
			objData.amount = 4;
		} else if (lx == 24 && ly == 3) {
			objData.interactionMode = I_CHEST;
			objData.itemId = 0;
			objData.amount = 2;
		} else if (lx == 26 && ly == 33) {
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 1;
			objData.warpTarget = (Vector2) {
				9.f - (18.f / 2.f),
				4.f - (22.f / 2.f) };
			objData.warpType = INVISIBLE;
		} else if (lx == 9 && ly == 9) { // Cirno reference
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 2;
			objData.warpTarget = (Vector2){
				38.f - (46.f / 2.f),
				29.f - (36.f / 2.f)};
			objData.warpType = LADDER_DOWN;
		} else if (lx == 2 && ly == 27) {
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 3;
			objData.warpTarget = (Vector2){
				27.f - (64.f / 2.f), 57.f - (64.f / 2.f)};
		}
		break;
	case 1:
		if (lx == 9 && ly == 2) {
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 0;
			objData.warpTarget = (Vector2){
				26.f - (36.f / 2.f),
				32.f - (36.f / 2.f)};
			objData.warpType = INVISIBLE;
		}
		break;
	case 2:
		if (lx == 38 && ly == 31) {
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 0;
			objData.warpTarget = (Vector2){
				9.f - (36.f / 2.f),
				11.f - (36.f / 2.f)};
			objData.warpType = LADDER_UP;
		} else if (lx == 18 && ly == 23) {
			objData.interactionMode = I_CHEST;
			objData.itemId = 1;
			objData.amount = 2;
		} else if (lx == 25 && ly == 15) {
			objData.interactionMode = I_CHEST;
			objData.itemId = 2;
			objData.amount = 5;
		} else if ((lx == 31 && ly == 15) || (lx == 34 && ly == 2)) {
			objData.interactionMode = I_CHEST;
			objData.itemId = 0;
			objData.amount = 2;
		} else if ((lx == 41 && ly == 18) || (lx == 5 && ly == 19) ||
			(lx == 39 && ly == 2) || (lx == 29 && ly == 8)) {
				objData.interactionMode = I_CHEST;
				objData.itemId = -2;
		} else if (lx == 17 && ly == 26) {
			objData.interactionMode = I_CHEST;
			objData.itemId = 2;
			objData.amount = 5;
		} else if ((lx == 22 && ly == 21) || (lx == 19 && ly == 17) ||
			(lx == 19 && ly == 14) || (lx == 15 && ly == 7)) {
				objData.interactionMode = I_DOOR;
		} else if (lx == 2 && ly == 11) {
			objData.warpType = LADDER_UP;
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 3;
			objData.warpTarget = (Vector2){
				39.f - (64.f / 2.f),
				48.f - (64.f / 2.f)};
		}
		break;
	case 3:
		if (lx == 40 && ly == 48) {
			objData.warpType = LADDER_DOWN;
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 2;
			objData.warpTarget = (Vector2){
				4.f - (46.f / 2.f),
				11.f - (36.f / 2.f)};
		} else if (lx == 27 && ly == 58) {
			objData.warpType = INVISIBLE;
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 0;
			objData.warpTarget = (Vector2){
				4.f - (36.f / 2.f), 27.f - (36.f / 2.f)};
		} else if (lx == 20 && ly == 21) {
			objData.warpType = LADDER_DOWN;
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 4;
			objData.warpTarget = (Vector2){
				2.f - (48.f / 2.f), 23.f - (36.f / 2.f)};
		}
		break;
	case 4:
		if (lx == 1 && ly == 24) {
			objData.warpType = LADDER_UP;
			objData.interactionMode = I_COLLISION;
			objData.warpLevel = 3;
			objData.warpTarget = (Vector2){
				20.f - (64.f / 2.f), 20.f - (64.f / 2.f)};
		}
		break;
	default:
		break;
	}

	return objData;
}

void
Level::UnloadLevelObjects()
{
	for (auto& obj : this->objects)
		delete obj;
	this->objects.clear();
}

void
Level::UnloadLevelEntities()
{
	for(auto& entity: this->entities)
		delete entity;
	this->entities.clear();
}

void
Level::UnloadLevelParticles()
{
	for (auto& particle : this->particles)
		delete particle;
	this->particles.clear();
}

void
Level::UnloadLevelBullets()
{
	for (auto& bullet : this->bullets)
		delete bullet;
	this->bullets.clear();
}

void
Level::UnloadLevelResources()
{
	if (IsModelReady(this->model))
		UnloadModel(this->model);
	if (IsTextureReady(this->texture))
		UnloadTexture(this->texture);
	if (IsImageReady(this->map))
		UnloadImage(this->map);
}
