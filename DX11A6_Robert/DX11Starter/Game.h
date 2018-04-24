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
	GameEntity* ge2;
	GameEntity* ge3;
	GameEntity* ge4;
	GameEntity* ge5;
	GameEntity* ge6;
	GameEntity* ge7;

	Enemy* e0;

	Camera* camera;

	//Lighting
	DirectionalLight dLight;
	DirectionalLight dLight2;

	// Shadow Map Data
	ID3D11Texture2D* shadowTexture;
	ID3D11DepthStencilView* shadowDepth;
	ID3D11ShaderResourceView* shadowSRV;
	D3D11_VIEWPORT shadowViewport;
};

