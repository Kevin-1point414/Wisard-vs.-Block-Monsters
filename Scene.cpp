#include "OpenWindow.h"
#include "Scene.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec2 _textCords;"
"out vec2 textCords;"
"uniform mat4 transformationMatrix;"
"void main()"
"{"
"   gl_Position = transformationMatrix * vec4(aPos.x, aPos.y, aPos.z, 1);"
"	textCords = _textCords;"
"}";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;"
"in vec2 textCords;"
"uniform sampler2D ourTexture;"
"void main()"
"{"
"FragColor = texture(ourTexture,textCords);"
"}";

void Scene::SetUpScene() {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	sceneList.push_back(this);

	transformLoc = glGetUniformLocation(shaderProgram, "transformationMatrix");
}

void Scene::UpdatePhysics(float time) {
	for (int i = 0; i < valueSpriteList.size(); i++) {

		valueSpriteList[i].SetIsGrounded(false);
		UpdatePhysicsForSprite(&valueSpriteList[i], time);
	}
	for (Sprite* sprite : pointerSpriteList) {

		sprite->SetIsGrounded(false);
		UpdatePhysicsForSprite(sprite, time);
	}

	int valueSpriteCount = (int)valueSpriteList.size();
	int pointerSpriteCount = (int)pointerSpriteList.size();

	for (int r = 0; r < 1; r++) {
		for (int i = 0; i < valueSpriteCount; i++) {
			for (int j = 0; j < valueSpriteCount; j++) {
				if (j == i) continue;
				ResolveCollisions(&valueSpriteList[i], &valueSpriteList[j], time);
				if (valueSpriteCount != valueSpriteList.size()
					|| pointerSpriteCount != pointerSpriteList.size()) {
					return;
				}
			}
			for (Sprite* spritePointer : pointerSpriteList) {
				ResolveCollisions(&valueSpriteList[i], spritePointer, time);
				if (valueSpriteCount != valueSpriteList.size()
					|| pointerSpriteCount != pointerSpriteList.size()) {
					return;
				}
			}
		}
		for (Sprite* sprite : pointerSpriteList) {
			for (int i = 0; i < valueSpriteCount; i++) {
				ResolveCollisions(sprite, &valueSpriteList[i], time);
				if (valueSpriteCount != valueSpriteList.size()
					|| pointerSpriteCount != pointerSpriteList.size()) {
					return;
				}
			}
			for (Sprite* spritePointer : pointerSpriteList) {
				if (sprite == spritePointer) continue;
				ResolveCollisions(sprite, spritePointer, time);
				if (valueSpriteCount != valueSpriteList.size()
					|| pointerSpriteCount != pointerSpriteList.size()) {
					return;
				}
			}
		}
	}
}

void Scene::UpdatePhysicsForSprite(Sprite* sprite, float time) {

	if (!sprite->GetIsPhysicsActive() || !sprite->GetIsMobile() || sprite->GetIsUI()) {
		sprite->ClearForces();
		return;
	}
	sprite->SetRotation(sprite->GetRotationVector() / sprite->GetRotationVector().Magnitude());

	sprite->SetPosition(sprite->GetPosition() + sprite->GetVelocity() * time);

	Vector2 changeInRotation(cos(sprite->GetAngluarVelocity() * time),
		sin(sprite->GetAngluarVelocity() * time));

	float rotationValues[4] = {
		changeInRotation.x, -changeInRotation.y,
		changeInRotation.y, changeInRotation.x
	};
	Matrix2x2 rotationMatrix(rotationValues);

	sprite->SetRotation(rotationMatrix * sprite->GetRotationVector());

	Vector2 netForce = Vector2(0, 0);

	float netTorque = 0;

	for (const Force& force : sprite->GetForces())
	{
		netForce += force.force;

		netTorque += (Vector3(force.relativePositionAppliedAt.x,
			force.relativePositionAppliedAt.y, 0) % Vector3(force.force.x, force.force.y, 0
			)).z;
	}
	netForce += Vector2(0, -sprite->GetMass() * sprite->GetGravity());

	sprite->SetAcceleration(netForce / sprite->GetMass());

	sprite->SetOldVelocity(sprite->GetVelocity());

	sprite->SetVelocity(sprite->GetVelocity() * powf(0.95f, time) + sprite->GetAcceleration()
		* time);
	sprite->SetAngluarVelocity(sprite->GetAngluarVelocity() * powf(0.95f, time)
		+ netTorque / sprite->GetMomentOfInertia() * time);

	sprite->ClearForces();
}

void Scene::ResolveCollisions(Sprite* sprite, Sprite *secondSprite, float time) {

	if (!sprite->GetIsMobile() && !secondSprite->GetIsMobile()) {
		return;
	}
	if ((sprite->GetPosition() - secondSprite->GetPosition()).MagnitudeSquared()
			> (sprite->GetSize() + secondSprite->GetSize()).MagnitudeSquared()) {
		return;
	}

	Vector2 position = secondSprite->GetPosition();
	Vector2 rotation = secondSprite->GetRotationVector();

	ConvertToLocalCoordinates(sprite, secondSprite);

	float pointRotationValues[4] = {
	   sprite->GetRotationVector().x, -sprite->GetRotationVector().y,
	   sprite->GetRotationVector().y, sprite->GetRotationVector().x
	};
	Matrix2x2 pointRotationMatrix(pointRotationValues);

	Vector2 points[4];

	points[0] = pointRotationMatrix * sprite->GetSize() * 1 / 2 + sprite->GetPosition();

	points[1] = pointRotationMatrix * Vector2(sprite->GetSize().x, -sprite->GetSize().y)
		* 1 / 2 + sprite->GetPosition();

	points[2] = pointRotationMatrix * Vector2(-sprite->GetSize().x, sprite->GetSize().y)
		* 1 / 2 + sprite->GetPosition();

	points[3] = pointRotationMatrix * Vector2(-sprite->GetSize().x, -sprite->GetSize().y)
		* 1 / 2 + sprite->GetPosition();

	int pointsGreaterThanY = 0;
	int pointsGreaterThanX = 0;
	int pointsLessThanX = 0;
	int pointsLessThanY = 0;
	Vector2 pointsCollided[4];
	int numberOfPointsCollided = 0;

	for (int i = 0; i < 4; i++) {
		if (points[i].x >= -secondSprite->GetSize().x * 1 / 2 &&
			points[i].x <= secondSprite->GetSize().x * 1 / 2 &&
			points[i].y >= -secondSprite->GetSize().y * 1 / 2 &&
			points[i].y <= secondSprite->GetSize().y * 1 / 2) {
			pointsCollided[numberOfPointsCollided] = points[i];
			numberOfPointsCollided++;
		}
		if (points[i].x < -secondSprite->GetSize().x * 1 / 2) {
			pointsLessThanX++;
		}
		if (points[i].y < -secondSprite->GetSize().y * 1 / 2) {
			pointsLessThanY++;
		}
		if (points[i].y > secondSprite->GetSize().y * 1 / 2) {
			pointsGreaterThanY++;
			if (points[i].y - 0.5f * secondSprite->GetSize().y < 0.001f) {
				sprite->SetIsGrounded(true);
			}
		}
		if (points[i].x > secondSprite->GetSize().x * 1 / 2) {
			pointsGreaterThanX++;
		}
	}
	if (numberOfPointsCollided == 0) {
		ConvertToWorldCoordinates(sprite, secondSprite, position, rotation);
		return;
	}

	if (!sprite->GetIsPhysicsActive() || !secondSprite->GetIsPhysicsActive()
		|| sprite->GetIsUI() || secondSprite->GetIsUI()) {
		ConvertToWorldCoordinates(sprite, secondSprite, position, rotation);
		if (sprite->Collided != NULL) {
			sprite->Collided(sprite, secondSprite);
		}
		if (secondSprite->Collided != NULL) {
			secondSprite->Collided(secondSprite, sprite);
		}
		return;
	}
	if (!sprite->GetCanCollide() || !secondSprite->GetCanCollide()) {
		ConvertToWorldCoordinates(sprite, secondSprite, position, rotation);
		if (sprite->Collided != NULL) {
			sprite->Collided(sprite, secondSprite);
		}
		if (secondSprite->Collided != NULL) {
			secondSprite->Collided(secondSprite, sprite);
		}
		return;
	}

	bool hitX = false;

	float minY = INFINITY;
	float maxY = -INFINITY;
	float displacementY = 0;

	for (int i = 0; i < 4; i++) {

		if (points[i].x >= -secondSprite->GetSize().x * 1 / 2 &&
			points[i].x <= secondSprite->GetSize().x * 1 / 2 &&
			points[i].y >= -secondSprite->GetSize().y * 1 / 2 &&
			points[i].y <= secondSprite->GetSize().y * 1 / 2) {
			if (pointsGreaterThanY > 0) {
				displacementY = secondSprite->GetSize().y * 1 / 2 - points[i].y;
			}
			else {
				displacementY = -secondSprite->GetSize().y * 1 / 2 - points[i].y;
			}
			continue;
		}
		if (points[i].y < minY) {
			minY = points[i].y;
		}
		if (points[i].y > maxY) {
			maxY = points[i].y;
		}
	}
	float changeInHeight = sprite->GetVelocity().y * time;
	minY -= changeInHeight;
	maxY -= changeInHeight;

	Vector2 totalChangeLinear(0, 0);
	Vector2 secondTotalChangeLinear(0, 0);

	for (int i = 0; i < numberOfPointsCollided; i++) {
		Vector2 penetration(0, 0);
		if (pointsGreaterThanY > pointsGreaterThanX && pointsGreaterThanY > pointsLessThanY
			&& pointsGreaterThanY > pointsLessThanX) {
			penetration = Vector2(0, secondSprite->GetSize().y * 1 / 2 - pointsCollided[i].y);
			sprite->SetIsGrounded(true);
		}
		else if (pointsGreaterThanX > pointsGreaterThanY
			&& pointsGreaterThanX > pointsLessThanY && pointsGreaterThanX > pointsLessThanX) {
			penetration = Vector2(secondSprite->GetSize().x * 1 / 2 - pointsCollided[i].x, 0);
			hitX = true;
		}
		else if (pointsLessThanY > pointsGreaterThanY
			&& pointsLessThanY > pointsGreaterThanX && pointsLessThanY > pointsLessThanX) {
			penetration = Vector2(0, -secondSprite->GetSize().y * 1 / 2 - pointsCollided[i].y);
		}
		else if (pointsLessThanX > pointsGreaterThanY
			&& pointsLessThanX > pointsGreaterThanX && pointsLessThanX > pointsLessThanY) {
			penetration = Vector2(-secondSprite->GetSize().x * 1 / 2 - pointsCollided[i].x, 0);
			hitX = true;
		}
		else if (sprite->GetIsMobile()) {
			if (pointsGreaterThanX == pointsGreaterThanY && pointsGreaterThanX != 0) {
				if (minY >= secondSprite->GetSize().y * 1 / 2) {
					penetration = Vector2(0, secondSprite->GetSize().y * 1 / 2
						- pointsCollided[i].y);
					sprite->SetIsGrounded(true);
				}
				else {
					penetration = Vector2(secondSprite->GetSize().x * 1 / 2
						- pointsCollided[i].x, 0);
					hitX = true;
				}
			}
			else if (pointsLessThanX == pointsGreaterThanY && pointsGreaterThanY != 0) {
				if (minY >= secondSprite->GetSize().y * 1 / 2) {
					penetration = Vector2(0, secondSprite->GetSize().y * 1 / 2
						- pointsCollided[i].y);
					sprite->SetIsGrounded(true);
				}
				else {
					penetration = Vector2(-secondSprite->GetSize().x * 1 / 2
						- pointsCollided[i].x, 0);
					hitX = true;
				}
			}
			else if (pointsLessThanX == pointsLessThanY && pointsLessThanY != 0) {
				if (maxY <= -secondSprite->GetSize().y * 1 / 2) {
					penetration = Vector2(0, -secondSprite->GetSize().y * 1 / 2
						- pointsCollided[i].y);
				}
				else {
					penetration = Vector2(-secondSprite->GetSize().x * 1 / 2
						- pointsCollided[i].x, 0);
					hitX = true;
				}
			}
			else if (pointsGreaterThanX == pointsLessThanY && pointsLessThanY != 0) {
				if (maxY <= -secondSprite->GetSize().y * 1 / 2) {
					penetration = Vector2(0, -secondSprite->GetSize().y * 1 / 2
						- pointsCollided[i].y);
				}
				else {
					penetration = Vector2(secondSprite->GetSize().x * 1 / 2
						- pointsCollided[i].x, 0);
					hitX = true;
				}
			}
		}

		float averageCoefficientOfRestitution = (secondSprite->GetCoefficientOfRestitution() +
			sprite->GetCoefficientOfRestitution()) / 2.0f;

		float averageFriction = (secondSprite->GetFriction() + sprite->GetFriction()) / 2.0f;

		if (!sprite->GetIsMobile()) {

			Vector2 relativePointCollided = (pointsCollided[i] - penetration / 2);

			Vector3 angluarVelocityAtPoint = Vector3(0, 0, secondSprite->GetAngluarVelocity())
				% Vector3((relativePointCollided).x, (relativePointCollided).y, 0);

			Vector2 velocityAtPointOfCollision = secondSprite->GetVelocity()
				+ Vector2(angluarVelocityAtPoint.x, angluarVelocityAtPoint.y);

			Vector2 changeInVelocity;
			Vector2 changeInLinearVelocity;
			Vector2 impulse;
			Vector2 friction;

			if (!hitX) {

				changeInVelocity = Vector2(0, -(1 + averageCoefficientOfRestitution)
					* velocityAtPointOfCollision.y);

				impulse = changeInVelocity * secondSprite->GetMass();
				if (secondSprite->GetVelocity().x > 0) {
					friction = Vector2(-std::abs(impulse.y) * averageFriction, 0);
					secondSprite->SetVelocity(secondSprite->GetVelocity()
						+ friction / secondSprite->GetMass());
					if (secondSprite->GetVelocity().x < 0) {
						secondSprite->SetVelocity(0, secondSprite->GetVelocity().y);
					}
				}
				else if (secondSprite->GetVelocity().x < 0) {
					friction = Vector2(std::abs(impulse.y) * averageFriction, 0);
					secondSprite->SetVelocity(secondSprite->GetVelocity()
						+ friction / secondSprite->GetMass());
					if (secondSprite->GetVelocity().x > 0) {
						secondSprite->SetVelocity(0, secondSprite->GetVelocity().y);
					}
				}
			}
			else {
				if (velocityAtPointOfCollision.x > 0 && -penetration.x > 0 ||
					velocityAtPointOfCollision.x < 0 && -penetration.x < 0) {
					changeInVelocity = Vector2(-(1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.x, 0);
				}
				else {
					changeInVelocity = Vector2((1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.x, 0);
				}

				impulse = changeInVelocity * secondSprite->GetMass();
			}
			totalChangeLinear = changeInVelocity;

			secondSprite->SetPosition(penetration * -1);
		}
		else if (!secondSprite->GetIsMobile()) {

			Vector2 relativePointCollided = (pointsCollided[i] - penetration / 2)
				- sprite->GetPosition();

			Vector3 angluarVelocityAtPoint = Vector3(0, 0, sprite->GetAngluarVelocity())
				% Vector3((relativePointCollided).x, (relativePointCollided).y, 0);

			Vector2 velocityAtPointOfCollision = sprite->GetVelocity()
				+ Vector2(angluarVelocityAtPoint.x, angluarVelocityAtPoint.y);

			Vector2 changeInVelocity;
			Vector2 changeInLinearVelocity;
			Vector2 friction;
			Vector2 impulse;

			if (!hitX) {

				changeInVelocity = Vector2(0, -(1 + averageCoefficientOfRestitution)
					* velocityAtPointOfCollision.y);

				impulse = changeInVelocity * sprite->GetMass();

				if (sprite->GetVelocity().x > 0) {
					friction = Vector2(-std::abs(impulse.y) * averageFriction, 0);
					sprite->SetVelocity(sprite->GetVelocity() + friction);
					if (sprite->GetVelocity().x < 0) {
						sprite->SetVelocity(0, sprite->GetVelocity().y);
					}
				}
				else if (sprite->GetVelocity().x < 0) {
					friction = Vector2(std::abs(impulse.y) * averageFriction, 0);
					sprite->SetVelocity(sprite->GetVelocity() + friction);
					if (sprite->GetVelocity().x > 0) {
						sprite->SetVelocity(0, sprite->GetVelocity().y);
					}
				}
			}
			else {
				if (velocityAtPointOfCollision.x > 0 && -penetration.x > 0 || 
					velocityAtPointOfCollision.x < 0 && -penetration.x < 0) {
					changeInVelocity = Vector2(-(1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.x, 0);
				}
				else {
					changeInVelocity = Vector2((1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.x, 0);
				}

				impulse = changeInVelocity * sprite->GetMass();
			}
			totalChangeLinear = changeInVelocity;

			sprite->SetPosition(sprite->GetPosition() + penetration);
		}
		else {
			
			float proportionOfMass = sprite->GetMass() /
				(sprite->GetMass() + secondSprite->GetMass());

			float proportionOfMomentOfInertia = sprite->GetMomentOfInertia() /
				(sprite->GetMomentOfInertia() + secondSprite->GetMomentOfInertia());

			Vector2 relativePointCollided = (pointsCollided[i] - penetration / 2)
				- sprite->GetPosition();
			Vector2 secondRelativePointCollided = (pointsCollided[i] - penetration / 2);

			Vector3 angluarVelocityAtPoint = Vector3(0, 0, sprite->GetAngluarVelocity())
				% Vector3((relativePointCollided).x, (relativePointCollided).y, 0)
				+ Vector3(0, 0, secondSprite->GetAngluarVelocity())
				% Vector3((secondRelativePointCollided).x, (secondRelativePointCollided).y, 0);

			Vector2 velocityAtPointOfCollision
				= sprite->GetVelocity() - secondSprite->GetVelocity();

			Vector2 changeInVelocity;

			if (!hitX) {
				if (velocityAtPointOfCollision.y > 0 && -penetration.y > 0 ||
					velocityAtPointOfCollision.y < 0 && -penetration.y < 0) {
					changeInVelocity = Vector2(0, -(1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.y);
				}
				else {
					changeInVelocity = Vector2(0, (1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.y);
				}
			}
			else {
				if (velocityAtPointOfCollision.x > 0 && -penetration.x > 0 ||
					velocityAtPointOfCollision.x < 0 && -penetration.x < 0) {
					changeInVelocity = Vector2(-(1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.x, 0);
				}
				else {
					changeInVelocity = Vector2((1 + averageCoefficientOfRestitution)
						* velocityAtPointOfCollision.x, 0);
				}
			}
			totalChangeLinear = changeInVelocity * (1 - proportionOfMass);
			secondTotalChangeLinear = changeInVelocity * -proportionOfMass;

			sprite->SetPosition(sprite->GetPosition() + penetration * (1 - proportionOfMass));
			secondSprite->SetPosition(penetration * -proportionOfMass);
		}
	}
	sprite->SetVelocity(sprite->GetVelocity() + totalChangeLinear);
	secondSprite->SetVelocity(secondSprite->GetVelocity() + secondTotalChangeLinear);
	ConvertToWorldCoordinates(sprite, secondSprite, position, rotation);

	if (sprite->Collided != NULL) {
		sprite->Collided(sprite, secondSprite);
	}
	if (secondSprite->Collided != NULL) {
		secondSprite->Collided(secondSprite, sprite);
	}
}

void Scene::ConvertToLocalCoordinates(Sprite* sprite, Sprite* secondSprite) {
	Vector2 position = secondSprite->GetPosition();
	Vector2 rotation = secondSprite->GetRotationVector();
	secondSprite->SetPosition(Vector2(0, 0));

	sprite->SetPosition(sprite->GetPosition() - position);

	secondSprite->SetRotation(Vector2(1, 0));

	float rotationValues[4] = {   // negative rotation matrix is used to apply the same  
		rotation.x , rotation.y,  // rotation to sprite as was applied to secondSprite when   
		-rotation.y, rotation.x   // setting secondSprite's rotation to zero
	};
	Matrix2x2 rotationMatrix(rotationValues);

	sprite->SetRotation(rotationMatrix * sprite->GetRotationVector());
	sprite->SetPosition(rotationMatrix * sprite->GetPosition());
	sprite->SetVelocity(rotationMatrix * sprite->GetVelocity());
	secondSprite->SetVelocity(rotationMatrix * secondSprite->GetVelocity());
}

void Scene::ConvertToWorldCoordinates(Sprite* sprite, Sprite* secondSprite,
	Vector2 position, Vector2 rotation) {
	float inverseRotationValues[4] = {
		rotation.x , -rotation.y,
		rotation.y, rotation.x
	};
	Matrix2x2 inverseRotationMatrix(inverseRotationValues);

	sprite->SetPosition(inverseRotationMatrix * sprite->GetPosition() + position);
	sprite->SetRotation(inverseRotationMatrix * sprite->GetRotationVector());
	sprite->SetVelocity(inverseRotationMatrix * sprite->GetVelocity());

	secondSprite->SetPosition(position + inverseRotationMatrix * secondSprite->GetPosition());
	secondSprite->SetRotation(inverseRotationMatrix * secondSprite->GetRotationVector());
	secondSprite->SetVelocity(inverseRotationMatrix * secondSprite->GetVelocity());
}

void Scene::RenderScene() {
	glClearColor(135.0 / 256.0, 206.0 / 256.0, 235.0 / 256.0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	for (int i = 0; i < pointerSpriteList.size(); i++)
	{
		RenderSprite(pointerSpriteList[i]);
	}
	for (int i = 0; i < valueSpriteList.size(); i++)
	{
		RenderSprite(&valueSpriteList[i]);
	}
}

void Scene::RenderSprite(Sprite* sprite) {

	Matrix4x4 finalMatrix;

	float scaleValues[16] = {
		sprite->GetSize().x, 0				    , 0, 0,
		0				   , sprite->GetSize().y, 0, 0,
		0				   , 0				    , 1, 0,
		0				   , 0				    , 0, 1
	};
	Matrix4x4 scaleMatrix(scaleValues);

	float rotationValues[16] = {
		sprite->GetRotationVector().x, -sprite->GetRotationVector().y, 0, 0,
		sprite->GetRotationVector().y, sprite->GetRotationVector().x , 0, 0,
		0							 , 0							 , 1, 0,
		0							 , 0							 , 0, 1
	};
	Matrix4x4 rotationMatrix(rotationValues);

	float translationValues[16] = {
		1, 0, 0, sprite->GetPosition().x,
		0, 1, 0, sprite->GetPosition().y,
		0, 0, 1, 0					    ,
		0, 0, 0, 1
	};
	Matrix4x4 translationMatrix(translationValues);

	Matrix4x4 worldCoordinatesMatrix = translationMatrix * rotationMatrix * scaleMatrix;

	if (!sprite->GetIsUI()) {

		float screenScaleValues[16] = {
			screenRatio.x / camera->GetViewSize(), 0									, 0, 0,
			0									 , screenRatio.y / camera->GetViewSize(), 0, 0,
			0									 , 0									, 1, 0,
			0									 , 0									, 0, 1
		};
		Matrix4x4 screenScaleMatrix(screenScaleValues);

		float screenTranslationValues[16] = {
			1, 0, 0, -camera->GetPosition().x,
			0, 1, 0, -camera->GetPosition().y,
			0, 0, 1, 0						 ,
			0, 0, 0, 1
		};
		Matrix4x4 screenTranslationMatrix(screenTranslationValues);
		Matrix4x4 worldToScreenCoordinatesMatrix = screenScaleMatrix * screenTranslationMatrix;
		finalMatrix = worldToScreenCoordinatesMatrix * worldCoordinatesMatrix;
	}
	else {
		finalMatrix = worldCoordinatesMatrix;
	}

	glUniformMatrix4fv(transformLoc, 1, true, *finalMatrix.values);
	glBindVertexArray(sprite->VAO);
	glBindTexture(GL_TEXTURE_2D, sprite->textureID);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Scene::AddSpriteToScene(Sprite* _sprite) {
	pointerSpriteList.push_back(_sprite);

	_sprite->scenesIncludedIn.push_back((int)(std::find(sceneList.begin(),
		sceneList.end(), this) - sceneList.begin()));
}

void Scene::AddCameraToScene(Camera* _camera) {
	camera = _camera;
}

void Scene::Reset() {
	camera = NULL;
	shaderProgram = NULL;
	for (int i = 0; i < pointerSpriteList.size(); i++) {
		pointerSpriteList[i]->Reset();
	}
	pointerSpriteList.clear();
	valueSpriteList.clear();
	transformLoc = 0;
}

void Scene::RemoveSprite(std::string spriteName) {

	for (int i = 0; i < pointerSpriteList.size(); i++) {
		if (pointerSpriteList[i]->GetName() == spriteName) {
			pointerSpriteList.erase(pointerSpriteList.begin() + i);
		}
	}
	for (int i = 0; i < valueSpriteList.size(); i++) {
		if (valueSpriteList[i].GetName() == spriteName) {
			valueSpriteList.erase(valueSpriteList.begin() + i);
		}
	}
}

void Scene::RemoveSprite(Sprite* spritePointer) {

	std::vector<Sprite*>::iterator iterator = std::find(pointerSpriteList.begin(), 
		pointerSpriteList.end(), spritePointer);
	if (iterator != pointerSpriteList.end()) {
		pointerSpriteList.erase(iterator);
	}
	else {
		for (int i = 0; i < valueSpriteList.size(); i++) {
			if (spritePointer == &valueSpriteList[i]) {
				valueSpriteList.erase(valueSpriteList.begin() + i);
			}
		}
	}
}

std::vector<Scene*> sceneList;
