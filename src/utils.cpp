#include"utils.hpp"
#include<raymath.h>
#include<cmath>

float
GetCameraMatrixYaw(Matrix x)
{
	return atan2f(x.m0, x.m8);
}

float
GetCameraMatrixPitch(Matrix x)
{
	return acosf(-x.m8);
}

float
GetCameraMatrixRoll(Matrix x)
{
	return atan2f(x.m9, x.m10);
}

float
GetModelMatrixYaw(Matrix x)
{
	return atan2f(x.m0, x.m4);
}

bool
IsEqualApprox(float x, float y)
{
	return fabs(x - y) <= (
		(fabs(x) < fabs(y)? fabs(y) : fabs(x)) * __FLT_EPSILON__);
}

void
SetModelTexture(Model& model, Texture2D texture)
{
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
}

void
SetModelCubemapTexture(Model& model, TextureCubemap texture)
{
	model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = texture;	
}

void

CenterModelTransform(Model& model)
{
	BoundingBox bb = GetModelBoundingBox(model);
    Vector3 center = { 0 };
    center.x = bb.min.x  + (((bb.max.x - bb.min.x)/2));
    center.z = bb.min.z  + (((bb.max.z - bb.min.z)/2));

    Matrix matTranslate = MatrixTranslate(-center.x, 0.f, -center.z);
	
    model.transform = matTranslate;
}
