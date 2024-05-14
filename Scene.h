#pragma once
#include <vector>
#include "Sprite.h"
#include "Camera.h"

class Scene {
public:
	void RenderScene();
	void SetUpScene();
	void AddSpriteToScene(Sprite* _sprite);
	void AddCameraToScene(Camera* _camera);

	Camera* camera;
	unsigned int shaderProgram = NULL;
	std::vector<Sprite*> spriteList;
};