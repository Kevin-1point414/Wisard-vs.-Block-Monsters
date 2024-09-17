#pragma once
#include "Utility.h"

class Force
{
public:

	Force(Vector2 _force, Vector2 _relativePositionAppliedAt = Vector2(0, 0));
	Force(float _forceX, float _forceY, Vector2 _relativePositionAppliedAt = Vector2(0, 0));
	Force(Vector2 _force, float _relativePositionAppliedAtX, float _relativePositionAppliedAtY);
	Force(float _forceX, float _forceY, float _relativePositionAppliedAtX, float _relativePositionAppliedAtY);
	Force();

	Vector2 force;
	Vector2 relativePositionAppliedAt;
};
