#pragma once
#include "Camera.h"
#include "Sprite.h"
#include <vector>

class Scene {
public:
	void RenderScene();
	void UpdatePhysics(float time);
	void SetUpScene();
	void AddSpriteToScene(Sprite* _sprite);
	void AddCameraToScene(Camera* _camera);
	void Reset();
	void RemoveSprite(std::string spriteName);
	void RemoveSprite(Sprite* spritePointer);

	Camera* camera = NULL;
	unsigned int shaderProgram = NULL;
	std::vector<Sprite*> pointerSpriteList;
	std::vector<Sprite> valueSpriteList;

private:

	Vector3 backgroundColor;
	void RenderSprite(Sprite* sprite);
	void UpdatePhysicsForSprite(Sprite* sprite, float time);
	void ResolveCollisions(Sprite* sprite, Sprite* secondSprite, float time);
	void ConvertToLocalCoordinates(Sprite* sprite, Sprite* secondSprite);
	void ConvertToWorldCoordinates(Sprite* sprite, Sprite* secondSprite,
		Vector2 position, Vector2 rotation);

	unsigned int transformLoc;
};
extern std::vector<Scene*> sceneList;
