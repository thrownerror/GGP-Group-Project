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

	bullets[numBullets - 1].TransformRotation(GetRotation());
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
		//float angle; //= atan2(GetPosition().x - player->GetPosition().x, GetPosition().z - player->GetPosition().z);
		//float angle = acos(dot(GetPosition(), player->GetPosition()) / mag(GetPosition()) * mag(player->GetPosition()));
		//angle = angle * (180 / 3.1415926f);
		float angle = atan2(GetPosition().z - player->GetPosition().z, GetPosition().x - player->GetPosition().x);
		angle *= -180 / (3.1415926 * 5);
		//SetRotation(XMStoreFloat3( , XMMatrixLookAtLH(XMLoadFloat3(&GetPosition()), XMLoadFloat3(&player->GetPosition()), XMLoadFloat3(&up))));
		//TransformRotation(player->GetPosition()); //<= change rotation based on player
		//SetRotation(XMFLOAT3(0, angle, 0));
		//TransformRotation(XMFLOAT3(0, angle, 0));
		SetRotation(XMFLOAT3(0, angle, 0));
		//printf("Rotation Angle: %f %f %f \n", GetRotation().x, GetRotation().y, GetRotation().z);
		//printf("Basic angle: %f \n", angle);
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