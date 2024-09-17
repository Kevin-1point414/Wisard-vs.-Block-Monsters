#pragma once
#include "Force.h"
#include "OpenWindow.h"
#include "Utility.h"
#include <vector>
#include <wincodec.h>

#pragma comment(lib, "windowscodecs.lib")

class Sprite
{
public:
	Sprite();

	~Sprite();

	Sprite(const Sprite& sprite) = default;

	Sprite(Sprite&& sprite) noexcept;
	Sprite& operator=(const Sprite&) = default;

	unsigned int VAO = NULL, VBO = NULL, EBO = NULL, textureID = NULL;
	WICInProcPointer texture = NULL;

	void SetTexture(std::string imageName);
	void SetRotation(float _rotation);
	void SetRotation(Vector2 _rotation);
	void SetPosition(Vector2 _position);
	void SetPosition(float _posX, float _posY);
	void SetSize(Vector2 _size);
	void SetSize(float _sizeX, float _sizeY);
	void SetMass(float _mass);
	void SetCoefficientOfRestitution(float _coefficientOfRestitution);
	void SetGravity(float _gravity);
	void SetFriction(float _friction);
	void SetVelocity(float _velocityX, float _velocityY);
	void SetVelocity(Vector2 _velocity);
	void SetOldVelocity(Vector2 _oldVelocity);
	void SetAcceleration(Vector2 _acceleration);
	void SetAngluarVelocity(float _angluarVelocity);
	void UpdateMomentOfInertia();
	void SetIsPhysicsActive(bool _isPhysicsActive);
	void SetIsMobile(bool _isMobile);
	void SetIsGrounded(bool _isGrounded);
	void SetIsUI(bool _isUI);
	void SetCanCollide(bool _canCollide);
	void ApplyForce(Force _force);
	void ApplyForce(Vector2 _force, Vector2 _relativePositionAppliedAt = Vector2(0, 0));
	void ApplyForce(float _forceX, float _forceY, Vector2 _relativePositionAppliedAt = Vector2(0, 0));
	void ApplyForce(float _forceX, float _forceY, float _relativePositionAppliedAtX,
		float _relativePositionAppliedAtY);
	void ApplyForce(Vector2 _force, float _relativePositionAppliedAtX, float _relativePositionAppliedAtY);
	void SetName(std::string _name);
	void Reset();

	void ClearForces();

	float GetRotation() const;
	Vector2 GetRotationVector() const;
	Vector2 GetPosition() const;
	Vector2 GetSize() const;
	Vector2 GetVelocity() const;
	Vector2 GetOldVelocity() const;
	Vector2 GetAcceleration() const;
	std::vector<Force> GetForces() const;
	float GetGravity() const;
	float GetMass() const;
	float GetMomentOfInertia() const;
	float GetAngluarVelocity() const;
	float GetFriction() const;
	float GetCoefficientOfRestitution() const;
	bool GetIsPhysicsActive() const;
	bool GetIsMobile() const;
	bool GetIsGrounded() const;
	bool GetIsUI() const;
	bool GetCanCollide() const;
	std::string GetImageName() const;
	std::string GetName() const;
	
	std::vector<int> scenesIncludedIn;

	void (*Collided) (Sprite* self, Sprite* spriteCollidedWith) = NULL;

private:

	Vector2 position = Vector2(0, 0);
	Vector2 velocity = Vector2(0, 0);
	Vector2 acceleration = Vector2(0, 0);
	Vector2 oldVelocity = Vector2(0, 0);
	Vector2 size = Vector2(1, 1);
	Vector2 rotation = Vector2(1, 0);
	float mass = 1;
	float coefficientOfRestitution = 0;
	float friction = 0.7f;
	float gravity = 2;
	float angluarVelocity = 0;
	float momentOfInertia;
	bool isPhysicsActive = true;
	bool isUI = false;
	bool isMobile = true;
	bool isGrounded = false;
	bool canCollide = true;
	std::string name;
	std::string imageName;
	WICRect imageRectangle = {};
	std::vector<Force> forces;
};
