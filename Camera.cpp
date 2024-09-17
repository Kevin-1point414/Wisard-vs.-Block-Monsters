#include "Camera.h"
#include "OpenWindow.h"

Vector2 Camera::GetPosition() const {
	return position;
}

float Camera::GetViewSize() const {
	return viewSize;
}

void Camera::SetPosition(Vector2 _position) {
	position = _position;
}

void Camera::SetPosition(float _posX, float _posY) {
	position = Vector2(_posX, _posY);
}

void Camera::SetViewSize(float _size) {
	viewSize = _size;
}

Vector2 Camera::WorldToScreenCoordinates(Vector2 worldCoordinates) const {

	float screenScaleValues[16] = {
		screenRatio.x / viewSize, 0						  , 0, 0,
		0						, screenRatio.y / viewSize, 0, 0,
		0						, 0						  , 1, 0,
		0						, 0						  , 0, 1
	};
	Matrix4x4 screenScaleMatrix(screenScaleValues);

	float screenTranslationValues[16] = {
		1, 0, 0, -position.x,
		0, 1, 0, -position.y,
		0, 0, 1, 0			,
		0, 0, 0, 1
	};
	Matrix4x4 screenTranslationMatrix(screenTranslationValues);

	Matrix4x4 worldToScreenCoordinatesMatrix = screenScaleMatrix * screenTranslationMatrix;

	Vector4 screenCoordinates = worldToScreenCoordinatesMatrix *
		Vector4(worldCoordinates.x, worldCoordinates.y, 0, 1);

	return Vector2(screenCoordinates.x, screenCoordinates.y);
}

Vector2 Camera::ScreenToWorldCoordinates(Vector2 screenCoordinates) const {

	return Vector2((screenCoordinates.x - 0.5f * screenSize.x)/ screenSize.x,
		-(screenCoordinates.y - 0.5f * screenSize.y) / screenSize.y) * viewSize + position;
}
