#pragma once
#include<raylib.h>

float GetCameraMatrixYaw(Matrix x);
float GetCameraMatrixPitch(Matrix x);
float GetCameraMatrixRoll(Matrix x);
float GetModelMatrixYaw(Matrix x);
bool IsEqualApprox(float x, float y);
void SetModelTexture(Model& model, Texture2D texture);
void SetModelCubemapTexture(Model& model, TextureCubemap texture);
void CenterModelTransform(Model& model);
