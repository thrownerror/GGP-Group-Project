#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Enemy.h"
#include "Camera.h"
#include "Lights.h"
#include "CollisionManager.h"
class Game 
	: public DXCore
{

public:


	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);

private:

	CollisionManager collidingMaster = CollisionManager();
	bool testBox;
	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateMatrices();
	void CreateBasicGeometry();

	void BuildLevelGeometry();
	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	//Mesh objects
	Mesh** meshArray;
	Mesh* mesh1;
	Mesh* mesh2;
	Mesh* mesh3;
	Mesh* meshModel1;
	int meshArraySize;
//	char* meshModel1Path;

	//Material objects
	Material* mat1;
	ID3D11ShaderResourceView* srv;
	ID3D11ShaderResourceView* nrm;
	ID3D11SamplerState* sampState;

	//GameEntity objects
	GameEntity** entityArray;
	int entityArraySize;

	GameEntity* gePlayer;
	GameEntity* ge1;

	//walls - first room
	GameEntity* wall1; //front 
	GameEntity* wall2; //back
	GameEntity* wall3; //left
	GameEntity* wall4; //right
	GameEntity* wall5; //top
	GameEntity* wall6; //bottom

	//hallway1
	GameEntity* wall7; //top
	GameEntity* wall8; //bottom
	GameEntity* wall9; //left
	GameEntity* wall10; //right

	//Shaft1
	GameEntity* wall11; //bottom
	GameEntity* wall12; //left wall 1
	GameEntity* wall13; //right wall 1
	GameEntity* wall14; //left wall 2
	GameEntity* wall15; //right wall 2
	GameEntity* wall16; //back wall 1
	GameEntity* wall17; //back wall 2
	GameEntity* wall18; //left wall 3
	GameEntity* wall19; //right wall 3
	GameEntity* wall20; //top wall
	GameEntity* wall21; //front wall

	//GameEntity* wall11;

	//End cap 
	GameEntity* wallEnd;


	Enemy* e0;

	Camera* camera;

	//Lighting
	DirectionalLight dLight;
	DirectionalLight dLight2;

	// Shadow Map Data
	ID3D11Texture2D* shadowTexture;
	ID3D11DepthStencilView* shadowDepth;
	ID3D11ShaderResourceView* shadowSRV;
	ID3D11SamplerState* shadowSampler;
	ID3D11RasterizerState* shadowRasterizer;
	D3D11_VIEWPORT shadowViewport;
	SimpleVertexShader* shadowShader;
	XMFLOAT4X4 shadowView;
	XMFLOAT4X4 shadowProjection;

	XMFLOAT3 movementRight = XMFLOAT3(2.0f, 0.0f, 0.0f);
	XMFLOAT3 movementLeft = XMFLOAT3(-2.0f, 0.0f, 0.0f);
	XMFLOAT3 movementDown = XMFLOAT3(0.0f, -2.0f, 0.0f);
	XMFLOAT3 movementUp = XMFLOAT3(0.0f, 2.0f, 0.0f);
	XMFLOAT3 movementBackward = XMFLOAT3(0.0f, 0.0f, -2.0f);
	XMFLOAT3 movementForward = XMFLOAT3(0.0f, 0.0f, 2.0f);
	XMFLOAT3 rotation180AroundY = XMFLOAT3(0.0f, 180.0f, 0.0f);
	XMFLOAT3 rotate90CWAroundY = XMFLOAT3(0.0f, 90.0f, 0.0f);
	XMFLOAT3 rotate90CCWAroundY = XMFLOAT3(0.0f, -90.0f, 0.0f);
	XMFLOAT3 rotate90CWAroundX = XMFLOAT3(90.0f, 0.0f, 0.0f);
	XMFLOAT3 rotate90CCWAroundX = XMFLOAT3(-90.0f, 0.0f, 0.0f);
};

