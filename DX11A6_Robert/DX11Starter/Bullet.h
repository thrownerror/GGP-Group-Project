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
	Emitter * emitter;
	ID3D11Device * device;
public:
	Bullet();
	Bullet(Mesh * mesh, Material * material, GameEntity * pl, ID3D11Device * d, SimpleVertexShader* pVert, SimplePixelShader* pPixel, ID3D11ShaderResourceView* pText);
	~Bullet();

	XMFLOAT3 GetDirection();
	void SetDirection(XMFLOAT3 dir);
	void SetPosition(XMFLOAT3 pos);
	float DistTravelled();
	float MaxDistTravelled();
	Emitter GetEmitter();

	void UpdateEntity(float deltaTime);
};