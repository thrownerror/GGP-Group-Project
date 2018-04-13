#include "Enemy.h"



Enemy::Enemy()
{
	bullets = std::vector<Bullet>();
	seesPlayer = false;
}

Enemy::Enemy(Mesh * enemyMesh, Material * enemyMaterial, Mesh * bullMesh, Material * bullMaterial) : GameEntity(enemyMesh, enemyMaterial)
{
	bulletMesh = bullMesh;
	bulletMaterial = bullMaterial;
}


Enemy::~Enemy()
{
}

void Enemy::Attack()
{
	bullets.push_back(Bullet(bulletMesh, bulletMaterial));
}

void Enemy::UpdateEntity()
{

	GameEntity::UpdateEntity();
}
