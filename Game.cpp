#include "OpenWindow.h"
#include "Scene.h"
#include "Sprite.h"

Scene scene;
Camera camera;
Sprite player;
Sprite ground;

void Start() {

	scene.SetUpScene();

	player.SetTexture("player.png");
	ground.SetTexture("Ground.png");
	ground.position.y = -1;
	ground.scale.x = 2;
	scene.AddSpriteToScene(&ground);
	scene.AddSpriteToScene(&player);
	scene.AddCameraToScene(&camera);
}

void Update() {
	if (KeyPressed('W')) {
		camera.position.y += 0.0001f;
	}
	if (KeyPressed('S')) {
		camera.position.y -= 0.0001f;
	}
	if (KeyPressed('D')) {
		camera.position.x += 0.0001f;
	}
	if (KeyPressed('A')) {
		camera.position.x -= 0.0001f;
	}
	player.rotation = pi;

	scene.RenderScene();
}
