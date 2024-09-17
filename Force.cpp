#include "Force.h"

Force::Force(Vector2 _force, Vector2 _relativePositionAppliedAt) {
	force = _force;
	relativePositionAppliedAt = _relativePositionAppliedAt;
}

Force::Force(float _forceX, float _forceY, Vector2 _relativePositionAppliedAt) {
	force = Vector2(_forceX, _forceY);
	relativePositionAppliedAt = _relativePositionAppliedAt;
}

Force::Force(float _forceX, float _forceY, float _relativePositionAppliedAtX,
	float _relativePositionAppliedAtY) {
	force = Vector2(_forceX, _forceY);
	relativePositionAppliedAt = Vector2(_relativePositionAppliedAtX, _relativePositionAppliedAtY);
}

Force::Force(Vector2 _force, float _relativePositionAppliedAtX, float _relativePositionAppliedAtY) {
	force = _force;
	relativePositionAppliedAt = Vector2(_relativePositionAppliedAtX, _relativePositionAppliedAtY);
}

Force::Force() {}