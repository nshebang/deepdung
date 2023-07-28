#include"skybox.hpp"
#include"utils.hpp"

Skybox::Skybox()
{
	this->mesh = GenMeshCube(1.f, 1.f, 1.f);
	this->model = LoadModelFromMesh(this->mesh);

	this->model.materials[0].shader = LoadShader("res/shaders/skybox.vs",
		"res/shaders/skybox.fs");

	int envMap[1] = { MATERIAL_MAP_CUBEMAP };
	int doGamma[1] = {0};
	int vFlipped[1] = {0};
	SetShaderValue(this->model.materials[0].shader,
		GetShaderLocation(this->model.materials[0].shader, "environmentMap"),
			envMap, SHADER_UNIFORM_INT);
    SetShaderValue(this->model.materials[0].shader,
		GetShaderLocation(this->model.materials[0].shader, "doGamma"),
			doGamma, SHADER_UNIFORM_INT);
    SetShaderValue(this->model.materials[0].shader,
		GetShaderLocation(this->model.materials[0].shader, "vflipped"),
			vFlipped, SHADER_UNIFORM_INT);

	Image img = LoadImage("res/textures/sky.png");
	SetModelCubemapTexture(this->model, LoadTextureCubemap(
		img, CUBEMAP_LAYOUT_AUTO_DETECT));
	SetTextureFilter(
		this->model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture,
		TEXTURE_FILTER_POINT); // TODO fix this shit (blurry skybox)
	UnloadImage(img);	
}

Skybox::~Skybox()
{
	UnloadShader(this->model.materials[0].shader);
	UnloadTexture(this->model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
	UnloadModel(this->model);
}

