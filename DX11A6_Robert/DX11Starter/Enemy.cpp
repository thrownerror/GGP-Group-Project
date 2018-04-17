#include "Enemy.h"



Enemy::Enemy()
{
	bullets = std::vector<Bullet>();
	seesPlayer = false;
	numBullets = 0;
	wp0 = XMFLOAT3(0, 0, 0);
	wp1 = XMFLOAT3(0, 0, 0);
	goingToOne = true;
	percentage = 0;
}

Enemy::Enemy(Mesh * enemyMesh, Material * enemyMaterial, Mesh * bullMesh, Material * bullMaterial) : GameEntity(enemyMesh, enemyMaterial)
{
	bulletMesh = bullMesh;
	bulletMaterial = bullMaterial;
	Enemy();
}


Enemy::~Enemy()
{
}

void Enemy::SetWanderPoints(XMFLOAT3 point0, XMFLOAT3 point1)
{
	wp0 = point0;
	wp1 = point1;
}

void Enemy::Attack()
{
	bullets.push_back(Bullet(bulletMesh, bulletMaterial));
	numBullets++;
}

void Enemy::UpdateEntity(float deltaTime)
{
	// If just wandering between two points
	if (!seesPlayer) {
		//if (wp0.x != wp1.x && wp0.y != wp1.y && wp0.z != wp1.z) {
		if (goingToOne) {
			percentage += 0.1f;
			if (percentage >= 1.0f) {
				percentage = 1.0f;
				goingToOne = false;
			}
		}
		else {
			percentage -= 0.1f;
			if (percentage <= 0) {
				percentage = 0;
				goingToOne = true;
			}
		}
		TransformTranslation(Lerp(wp0, wp1, percentage));
	}
	// If trageting the player
	else {

	}

	for (int i = 0; i < numBullets; i++)
	{
		bullets[i].UpdateEntity(deltaTime);
	}
	GameEntity::UpdateEntity();
}

// Lerps between two points, returning the position at the percent between the two
XMFLOAT3 Enemy::Lerp(XMFLOAT3 start, XMFLOAT3 end, float percent)
{
	XMVECTOR s = XMLoadFloat3(&start);
	XMVECTOR e = XMLoadFloat3(&end);
	XMFLOAT3 result;

	XMStoreFloat3(&result, s + percent * (e - s));

	return result;
}
