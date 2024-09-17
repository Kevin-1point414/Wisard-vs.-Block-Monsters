#include "Scene.h"
#include <random>
#include <string>

Scene scene;
Camera camera;
Sprite player;
Sprite manaDisplayFill;
Sprite manaDisplayOutline;
float mana;
float maxMana;
std::vector<Sprite> enemies;
bool hasJustShot = false;
bool hasJustJumped = false;
bool hasJustDashed = false;
float timeSinceJumped = 1;
float timeSinceManaUsed = 0;
float manaCostCoefficient;
float bossHealth;

void EnemyCollided(Sprite* enemy, Sprite* spriteCollidedWith);
void FireBallCollided(Sprite* fireBall, Sprite* spriteCollidedWith);

void Start() {

	bossHealth = 10;
	maxMana = 100;
	mana = 100;
	manaCostCoefficient = 1;
	scene.SetUpScene();
	player.SetTexture("Player.png");
	camera.SetViewSize(12);
	player.SetSize(0.7f, 1);
	player.SetPosition(0, 1);
	scene.AddSpriteToScene(&player);

	manaDisplayOutline.SetIsUI(true);
	manaDisplayOutline.SetPosition(0.8f, 0.95f);
	manaDisplayOutline.SetSize(0.2f, 0.06f);
	manaDisplayOutline.SetTexture("ManaDisplayOutline.png");
	scene.AddSpriteToScene(&manaDisplayOutline);

	manaDisplayFill.SetIsUI(true);
	manaDisplayFill.SetPosition(0.8f, 0.95f);
	manaDisplayFill.SetSize(0.19f, 0.055f);
	manaDisplayFill.SetTexture("ManaDisplayFill.png");
	scene.AddSpriteToScene(&manaDisplayFill);

	Sprite ground;
	ground.SetTexture("Ground.png");
	ground.SetPosition(0, 0);
	ground.SetSize(5, 1);
	ground.SetIsMobile(false);

	scene.AddSpriteToScene(&ground);

	for (int i = 0; i < 30; i++) {
		Sprite platform;
		float distanceBetweenPlatforms = 2;
		float platformWidth = 6;
		Vector2 platformPosition((platformWidth + distanceBetweenPlatforms) * i + 7, rand() % 110 / 100.0f);
		platform.SetPosition(platformPosition);
		platform.SetSize(platformWidth, 1);
		platform.SetTexture("Ground.png");
		platform.SetIsMobile(false);
		scene.AddSpriteToScene(&platform);

		for (int j = 0; j < rand() % 4 + 2; j++) {
			if (i == 0) continue;
			Sprite enemy;
			enemy.SetTexture("Enemy" + std::to_string(rand() % 8 + 1) + ".png");
			enemy.SetPosition(platformPosition.x + (j + 1) * (rand() % 100 + enemy.GetSize().x * 0.5f)
				/ 100.0f, platformPosition.y + 1);
			enemy.SetSize(0.8f, 0.8f);
			enemy.Collided = EnemyCollided;
			enemy.SetName("Enemy");
			scene.AddSpriteToScene(&enemy);
			enemies.push_back(std::move(enemy));
		}
	}

	Sprite end;
	end.SetIsMobile(false);
	end.SetTexture("Ground.png");
	end.SetPosition(249, 0);
	end.SetSize(10, 1);
	scene.AddSpriteToScene(&end);

	Sprite boss;
	boss.SetPosition(251, 1);
	boss.SetSize(3, 3);
	boss.SetTexture("Enemy1.png");
	boss.SetName("Boss");
	boss.Collided = EnemyCollided;

	scene.AddSpriteToScene(&boss);
	enemies.push_back(std::move(boss));

	scene.AddCameraToScene(&camera);
}

void Update(float time) {

	if (mana < maxMana) {
		if (timeSinceManaUsed < 2) {
			mana += time * 10;
			timeSinceManaUsed += time;
		}
		else {
			mana += time * 40;
		}
	}
	else {
		mana = maxMana;
	}
	manaDisplayFill.SetSize(mana / 100.0f * 0.19f, 0.055f);
	manaDisplayFill.SetPosition(0.8f + 0.5f * mana / 100.0f * 0.19f - 0.19f / 2, 0.95f);
	if (KeyPressed('W')) {
		if (player.GetIsGrounded()) {
			player.SetVelocity(player.GetVelocity().x, 2.3f);
			hasJustJumped = true;
			timeSinceJumped = 0;
		}
		else if (mana > 25 * manaCostCoefficient && !hasJustJumped && timeSinceJumped > 0.1) {
			player.SetVelocity(player.GetVelocity().x, 2.3f);
			mana -= 25 * manaCostCoefficient;
			timeSinceManaUsed = 0;
			hasJustJumped = true;
		}
	}
	else {
		hasJustJumped = false;
		timeSinceJumped += time;
	}
	if (KeyPressed(Spacebar) && mana > 15 * manaCostCoefficient && !hasJustDashed) {
		hasJustDashed = true;
		mana -= 15 * manaCostCoefficient;
		timeSinceManaUsed = 0;
		if (player.GetVelocity().x > 0) {
			player.SetVelocity(7, player.GetVelocity().y);
		}
		else {
			player.SetVelocity(-7, player.GetVelocity().y);
		}
	}
	if (!KeyPressed(Spacebar)) {
		hasJustDashed = false;
	}
	if (KeyPressed('D') && player.GetVelocity().x < 2.3f) {
		player.ApplyForce(40, 0);
		player.SetSize(-0.7f, 1);
	}
	if (KeyPressed('A') && player.GetVelocity().x > -2.3f) {
		player.ApplyForce(-40, 0);
		player.SetSize(0.7f, 1);
	}
	if (!KeyPressed(LeftMouseButton)) {
		hasJustShot = false;
	}
	if (KeyPressed(LeftMouseButton) && mana > 20 * manaCostCoefficient && !hasJustShot) {
		mana -= 20 * manaCostCoefficient;
		timeSinceManaUsed = 0;
		hasJustShot = true;
		Vector2 positionClicked = camera.ScreenToWorldCoordinates(mousePosition);
		Sprite fireBall;
		fireBall.SetSize(0.4f, 0.4f);
		fireBall.SetTexture("Fire" + std::to_string(rand() % 2 + 1) + ".png");
		fireBall.SetName("FireBall");
		fireBall.Collided = FireBallCollided;
		fireBall.SetCanCollide(false);
		if (positionClicked.x > player.GetPosition().x) {
			player.SetSize(-0.7f, 1);
			fireBall.SetPosition(player.GetPosition().x + (player.GetSize().x + fireBall.GetSize().x) / 2
				+ 0.5f, player.GetPosition().y + 0.3f);
		}
		else {
			player.SetSize(0.7f, 1);
			fireBall.SetPosition(player.GetPosition().x - (player.GetSize().x + fireBall.GetSize().x) / 2
				+ 0.2f, player.GetPosition().y + 0.3f);
		}
		Vector2 distance = positionClicked - player.GetPosition();
		fireBall.SetVelocity(distance / distance.Magnitude() * 4 + player.GetVelocity() * 0.5f);
		scene.AddSpriteToScene(&fireBall);
	}
	if (player.GetPosition().y < -10) {
		scene.Reset();
		enemies.clear();
		Start();
	}

	for (int i = 0; i < enemies.size(); i++) {

		Vector2 distance = player.GetPosition() - enemies[i].GetPosition();
		float distanceMagnitudeSquared = distance.MagnitudeSquared();
		
		if (distanceMagnitudeSquared < 100) {

			float distanceMagnitude = std::sqrt(distanceMagnitudeSquared);

			enemies[i].ApplyForce(distance.x / distanceMagnitude * 45, 0);

			if (enemies[i].GetVelocity().x < -3.5f) {
				enemies[i].SetVelocity(-3.5f, enemies[i].GetVelocity().y);
			}
			if (enemies[i].GetVelocity().x > 3.5f) {
				enemies[i].SetVelocity(3.5f, enemies[i].GetVelocity().y);
			}
			if (enemies[i].GetIsGrounded()) {
				if (player.GetPosition().y - (player.GetSize().y + enemies[i].GetSize().y) / 2.0f
					> enemies[i].GetPosition().y && distanceMagnitude < 2) {
					enemies[i].SetVelocity(enemies[i].GetVelocity().x, 3);
				}
				if ((int)std::round(enemies[i].GetPosition().x - 3 - 0.5f * enemies[i].GetSize().x - 7) % 8
					== 0 || (int)std::round(enemies[i].GetPosition().x + 3 + 0.5f * enemies[i].GetSize().x - 7)
					% 8 == 0) {
					enemies[i].SetVelocity(enemies[i].GetVelocity().x, 3);
				}	
			}
		}
	}

	camera.SetPosition(player.GetPosition());

	if (std::abs(player.GetVelocity().x) > 7) {
		player.SetVelocity(0, player.GetVelocity().y);
	}

	scene.UpdatePhysics(time);
	scene.RenderScene();
}

void EnemyCollided(Sprite* enemy, Sprite* spriteCollidedWith) {

	if (spriteCollidedWith->GetName() == "Player") {
		scene.Reset();
		enemies.clear();	
		Start();
	}
	if (spriteCollidedWith->GetName() == "FireBall" && enemy->GetName() == "Enemy") {
		scene.RemoveSprite(enemy);
		scene.RemoveSprite(spriteCollidedWith);
		manaCostCoefficient *= 0.996f;
	}
	if (spriteCollidedWith->GetName() == "FireBall" && enemy->GetName() == "Boss") {
		bossHealth--;
		scene.RemoveSprite(spriteCollidedWith);
		if (bossHealth == 0) {
			scene.RemoveSprite(enemy);
			manaCostCoefficient *= 0.95f;
		}
	}
}

void FireBallCollided(Sprite* fireBall, Sprite* spriteCollidedWith) {

	if (spriteCollidedWith->GetName() == "Ground") {
		scene.RemoveSprite(fireBall);
	}
}
