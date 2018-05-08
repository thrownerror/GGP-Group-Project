#pragma once
#include "Bullet.h"
#include "SimpleShader.h"
#include <cmath>

class Enemy :
	public GameEntity
{
private:
	// List of the bullets this enemy has fired for bullet updates
	std::vector<Bullet> bullets;
	int numBullets;

	// Bullet mesh and material
	Mesh* bulletMesh;
	Material* bulletMaterial;

	// Whether this enemy "sees" the player to shoot at it
	bool seesPlayer;
	float attackInterval;
	GameEntity* player;

	// Points the enemy wanders between
	XMFLOAT3 wp0, wp1;
	bool goingToOne;
	float percentage;

	// Calculates dot product / magnitude
	float dot(XMFLOAT3 a, XMFLOAT3 b);
	float mag(XMFLOAT3 a);

public:
	Enemy();
	Enemy(Mesh * enemyMesh, Material * enemyMaterial, Mesh * bullMesh, Material * bullMaterial, GameEntity * pl);
	~Enemy();
	void SetWanderPoints(XMFLOAT3 point0, XMFLOAT3 point1);
	void Attack();
	void UpdateEntity(float deltaTime);

	int NumBullets();
	std::vector<Bullet> Bullets();
};