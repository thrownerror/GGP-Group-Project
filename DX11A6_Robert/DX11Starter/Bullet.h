#pragma once
#include "GameEntity.h"
class Bullet :
	public GameEntity
{
private:
	float distTravelled;
	float maxDistTravelled;
	XMFLOAT3 direction;
public:
	Bullet();
	Bullet(Mesh * mesh, Material * material, GameEntity * pl);
	XMFLOAT3 GetDirection();
	void SetDirection(XMFLOAT3 dir);
	~Bullet();

	void UpdateEntity(float deltaTime);
};

