#pragma once
#include "Utility.h"
class Camera {

public:

	void SetPosition(Vector2 _position);
	void SetPosition(float _posX, float _posY);
	void SetViewSize(float _size);

	Vector2 ScreenToWorldCoordinates(Vector2 screenCoordinates) const;
	Vector2 WorldToScreenCoordinates(Vector2 worldCoordinates) const;

	Vector2 GetPosition() const;
	float GetViewSize() const;

private:
	Vector2 position = Vector2(0, 0);
	float viewSize = 5;
};
