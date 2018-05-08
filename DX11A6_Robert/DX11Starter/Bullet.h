#pragma once
#include "GameEntity.h"
#include "Emitter.h"
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
	~Bullet();

	XMFLOAT3 GetDirection();
	void SetDirection(XMFLOAT3 dir);
	float DistTravelled();
	float MaxDistTravelled();

	void UpdateEntity(float deltaTime);
};