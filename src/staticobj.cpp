#include"staticobj.hpp"
#include"utils.hpp"
#include<raymath.h>
#include<cmath>
#include<algorithm>

StaticObject::StaticObject(Sprite* sprite, Vector3 position,
	bool isWall, InteractionMode interactionMode)
{
	this->sprite = sprite;
	this->position = position;
	this->mesh = GenMeshPlane(1.f, 1.f, 5, 5);
	this->model = LoadModelFromMesh(this->mesh);
	CenterModelTransform(this->model);
	SetModelTexture(this->model, *sprite->GetFrameTexture());

	this->isWall = isWall;
	if (this->isWall)
		this->boundingBox = (BoundingBox) {
			Vector3SubtractValue(this->position, .4f),
			Vector3AddValue(this->position, .4f)
		};
	this->interactionMode = interactionMode;
}

StaticObject::~StaticObject()
{
	UnloadModel(this->model);
}

void
StaticObject::Update(Camera* camera)
{
	Matrix camMatrix = GetCameraMatrix(*camera);
	float yaw = GetCameraMatrixYaw(camMatrix);
	
	this->model.transform = MatrixRotateXYZ((Vector3){
		PI / 2.f, 2.f * PI, -yaw - (PI / 2.f)
	});
	
	SetModelTexture(this->model, *this->sprite->GetFrameTexture());
}

