#pragma once
#include "Bullet.h"

class Enemy :
	public GameEntity
{
private:
	std::vector<Bullet> bullets;
	bool seesPlayer;
	Mesh* bulletMesh;
	Material* bulletMaterial;

public:
	Enemy();
	Enemy(Mesh* enemyMesh, Material* enemyMaterial, Mesh* bullMesh, Material* bullMaterial);
	~Enemy();
	void Attack();
	void UpdateEntity();
};