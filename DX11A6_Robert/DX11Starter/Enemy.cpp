#include "Enemy.h"



Enemy::Enemy() : Enemy(nullptr, nullptr, nullptr, nullptr, nullptr)
{
}

Enemy::Enemy(Mesh * enemyMesh, Material * enemyMaterial, Mesh * bullMesh, Material * bullMaterial, GameEntity* pl) : GameEntity(enemyMesh, enemyMaterial)
{
	bulletMesh = bullMesh;
	bulletMaterial = bullMaterial;

	bullets = std::vector<Bullet>();
	seesPlayer = false;
	attackInterval = 0;
	numBullets = 0;
	wp0 = XMFLOAT3(0, 0, 0);
	wp1 = XMFLOAT3(0, 0, 0);
	goingToOne = true;
	percentage = 0;

	player = player;
}


Enemy::~Enemy()
{
}

void Enemy::SetWanderPoints(XMFLOAT3 point0, XMFLOAT3 point1)
{
	wp0 = point0;
	wp1 = point1;
	SetPosition(wp0);
}

void Enemy::Attack()
{
	bullets.push_back(Bullet(bulletMesh, bulletMaterial, player));
	numBullets++;

	bullets[numBullets].TransformRotation(GetRotation());
}

void Enemy::UpdateEntity(float deltaTime)
{
	// If just wandering between two points
	if (!seesPlayer) {
		//if (wp0.x != wp1.x && wp0.y != wp1.y && wp0.z != wp1.z) {
		if (goingToOne) {
			percentage += 0.2f * deltaTime;
			if (percentage >= 1.0f) {
				percentage = 1.0f;
				goingToOne = false;
			}
		}
		else {
			percentage -= 0.2f * deltaTime;
			if (percentage <= 0) {
				percentage = 0;
				goingToOne = true;
			}
		}
		SetPosition(Lerp(wp0, wp1, percentage));
	}
	// If targeting the player
	else {
		XMFLOAT3 up = XMFLOAT3(0, 1, 0);
		float angle = atan2(GetPosition().x - player->GetPosition().x, GetPosition().z - player->GetPosition().z);
		angle = angle * (180 / 3.1415926f);
		//SetRotation(XMStoreFloat3( , XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()), XMLoadFloat3(&player->GetPosition()), XMLoadFloat3(&up))));
		//TransformRotation(player->GetPosition()); //<= change rotation based on player
		attackInterval -= deltaTime;
		if (attackInterval <= 0) {
			attackInterval = 3.0f;
			Attack();
		}
	}

	// If the enemy sees the player, change seesPlayer
	//if()

	for (int i = 0; i < numBullets; i++)
	{
		bullets[i].UpdateEntity(deltaTime);
	}
	GameEntity::UpdateEntity();
}