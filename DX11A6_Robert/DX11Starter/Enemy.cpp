#include "Enemy.h"



float Enemy::dot(XMFLOAT3 a, XMFLOAT3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

float Enemy::mag(XMFLOAT3 a)
{
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

Enemy::Enemy() : Enemy(nullptr, nullptr, nullptr, nullptr, nullptr)
{
}

Enemy::Enemy(Mesh * enemyMesh, Material * enemyMaterial, Mesh * bullMesh, Material * bullMaterial, GameEntity* pl) : GameEntity(enemyMesh, enemyMaterial)
{
	bulletMesh = bullMesh;
	bulletMaterial = bullMaterial;

	bullets = std::vector<Bullet>();
	seesPlayer = false;
	attackInterval = 10;
	numBullets = 0;
	wp0 = XMFLOAT3(0, 0, 0);
	wp1 = XMFLOAT3(0, 0, 0);
	goingToOne = true;
	percentage = 0;

	player = pl;
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

	XMVECTOR pos = XMLoadFloat3(&GetPosition());
	XMVECTOR playerPos = XMLoadFloat3(&player->GetPosition());
	XMFLOAT3 result;
	XMStoreFloat3(&result, playerPos - pos);
	
	bullets[numBullets - 1].SetScale(XMFLOAT3(5, 5, 5));
	bullets[numBullets - 1].SetPosition(GetPosition());
	bullets[numBullets - 1].SetDirection(result);

	printf("Firing bullet \n");
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
		XMVECTOR pos = XMLoadFloat3(&GetPosition());
		XMVECTOR playerPos = XMLoadFloat3(&player->GetPosition());
		XMFLOAT3 result;
		XMStoreFloat3(&result, playerPos - pos);
		
		if (result.x < 0 && result.z < 0) {
			result.x *= -1;
		}
		//result.x = sin(result.x);

		SetRotation(XMFLOAT3(0, result.x, 0));

		attackInterval -= deltaTime;
		if (attackInterval <= 0) {
			attackInterval = 2.0f;
			Attack();
		}
	}

	// If the enemy sees the player, change seesPlayer

	XMFLOAT3 pos = GetPosition();
	XMFLOAT3 ePos = player->GetPosition();

	float dist = sqrt(pow(pos.x - ePos.x, 2) +
					  pow(pos.y - ePos.y, 2) +
					  pow(pos.z - ePos.z, 2));

	if (dist < 5.0f)
		seesPlayer = true;
	else
		seesPlayer = false;

	// Update all bullets
	for (int i = 0; i < numBullets; i++)
	{
		bullets[i].UpdateEntity(deltaTime);
	}
	GameEntity::UpdateEntity();
}

int Enemy::NumBullets()
{
	return numBullets;
}

std::vector<Bullet> Enemy::Bullets()
{
	return bullets;
}
