#pragma once
#include <d3d11.h>
#include "DXCore.h"
#include "Vertex.h"
#include "DirectXMath.h"
#include "Mesh.h"
#include "Material.h"
#include "Collider.h"
using namespace DirectX;

class GameEntity
{
public:
	//get-set matrix
	XMFLOAT4X4 GetWorldMatrix();
	void SetWorldMatrix(XMFLOAT4X4 setMatrix);

	//get-set vectors
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetScale();
	XMFLOAT3 GetColliderPosition();

	Collider* GetCollider();
	//these allow things to "jump" to positions
	void SetPosition(XMFLOAT3 setPos);
	void SetRotation(XMFLOAT3 setRot);
	void SetScale(XMFLOAT3 setSca);
	
	void SetCollisionBox(float xDim, float yDim, float zDim);
	void ColliderBoxMatrix(bool safeRotation);
	//offset methods
	void Move(XMFLOAT3 value); 
	void MoveForward(XMFLOAT3 value);

	//Draw
	ID3D11Buffer* GetVertBuffer();
	ID3D11Buffer* GetIndBuffer();
	int GetIndCount();

	//void Draw();
	void UpdateEntity();

	//transformation methods 
	void TransformTranslation(XMFLOAT3 value);
	void TransformScale(XMFLOAT3 value);
	void TransformRotation(XMFLOAT3 value);

	//Material methods
	void PrepareMaterial(XMFLOAT4X4 camView, XMFLOAT4X4 camProj);

	//Constructor/Deconstructor
	GameEntity();
	GameEntity(Mesh* mesh);
	GameEntity(Mesh* mesh, Material* material);
	~GameEntity();

	float GetColliderXWidth();
	//float getColliderXMin();
	float GetColliderYHeight();
	//float getColliderYMin();
	float GetColliderZDepth();
	//float getColliderZMin();
	void PrintPosition();
private:

	Material*	entityMaterial;
	Mesh*		entityMesh;
	XMFLOAT4X4  worldMatrix;
	XMFLOAT3	position;
	XMFLOAT3	collisionPosition;
	XMFLOAT3	rotation;
	XMFLOAT3	scale;
	//collision stuff - treated as radii
	Collider*	collisionBox;

/*	//float colliderXWidth;
	//float colliderYHeight;
	//float colliderZDepth;*/

	//collision detection notes
	//Bullet will be a box. AxisAligned 

	bool recalculateWorldMatrix;
	void calcWorldMatrix();
};

