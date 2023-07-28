#include"entity.hpp"
#include"utils.hpp"
#include<raymath.h>

Entity::Entity(Sprite* sprite, Vector3 position, bool isWall, unsigned hpMax)
{
	this->sprite = sprite;
	this->position = position;
	this->mesh = GenMeshPlane(1.f, 1.f, 5, 5);
	this->model = LoadModelFromMesh(this->mesh);
	CenterModelTransform(this->model);
	SetModelTexture(this->model, *sprite->GetFrameTexture());

	this->boundingBox = (BoundingBox) {
		Vector3SubtractValue(this->position, .4f),
		Vector3AddValue(this->position, .4f)
	};
	this->isWall = isWall;
	this->hpMax = hpMax;
	this->hp = hpMax;
}

Entity::~Entity()
{
	UnloadModel(this->model);
}

void
Entity::Update(bool isPlayerAttacking, Camera* camera,
	Image* map, Inventory* inventory)
{
	if (this->dead) {
		this->boundingBox.min = (Vector3){0.f, 0.f, 0.f};
		this->boundingBox.max = (Vector3){0.f, 0.f, 0.f};
		return;
	}

	this->Behavior(isPlayerAttacking, camera, map, inventory);

	this->oldPosition = this->position;
	this->position = Vector3Add(this->position, this->velocity);

	this->boundingBox = (BoundingBox) {
		Vector3SubtractValue(this->position, this->boxSize),
		Vector3AddValue(this->position, this->boxSize)
	};
	
	Matrix camMatrix = GetCameraMatrix(*camera);
	float yaw = GetCameraMatrixYaw(camMatrix);

	this->model.transform = MatrixRotateXYZ((Vector3){
		PI / 2.f, 2.f * PI, -yaw - (PI / 2.f)
	});

	SetModelTexture(this->model, *this->sprite->GetFrameTexture());
}

