#pragma once
#include<raylib.h>

class Skybox {
public:
	Skybox();
	~Skybox();

	inline Model* GetModel() {
		return &this->model;
	}

private:
	Mesh mesh;
	Model model;
};

