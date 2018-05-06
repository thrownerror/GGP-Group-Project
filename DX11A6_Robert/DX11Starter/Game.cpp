#include "Game.h"
#include "Vertex.h"
#include "WICTextureLoader.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;


#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete mesh1;
	delete mesh2;
	delete mesh3;
	delete meshModel1;

	delete ge1;
	delete wall1;
	delete wall2;
	delete wall3;
	delete wall4;
	delete wall5;
	delete wall6;

	delete wall7;
	delete wall8;
	delete wall9;
	delete wall10;

	delete wall11;
	delete wall12;
	delete wall13;
	delete wall14;
	delete wall15;
	delete wall16;
	delete wall17;
	delete wall18;
	delete wall19;
	delete wall20;
	delete wall21;

	delete gePlayer;

	delete e0;

	delete[] entityArray;
	delete[] meshArray;

	delete mat1;
	delete camera;

	// Clean up shadow map
	shadowDepth->Release();
	shadowSRV->Release();
	shadowRasterizer->Release();
	shadowSampler->Release();
	delete shadowShader;

	srv->Release();
	sampState->Release();
	//mat1->GetSRV()->Release();
	//mat1->GetSamplerState()->Release();
	//commenting one of these will cause a memory leak
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	camera = new Camera();
	//ID3D11ShaderResourceView* srv;
	//ID3D11SamplerState* sampState;
	D3D11_SAMPLER_DESC samDesc = {};
	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT samplerTest = device->CreateSamplerState(&samDesc, &sampState);
	HRESULT h1 = CreateWICTextureFromFile(device, context, L"../../Assets/Textures/Rock1.png", 0, &srv);
	HRESULT h2 = CreateWICTextureFromFile(device, context, L"../../Assets/Textures/RockNorm.png", 0, &nrm);

	mat1 = new Material(vertexShader, pixelShader, srv, nrm, sampState);

	prevMousePos.x = 0;
	prevMousePos.y = 0;
	CreateMatrices();
	CreateBasicGeometry();

	dLight.AmbientColor = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	dLight.DiffuseColor = XMFLOAT4(0, 0, 1, 1);
	dLight.Direction = XMFLOAT3(1, -1, 0);

	dLight2.AmbientColor = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	dLight2.DiffuseColor = XMFLOAT4(.5f, .5f, 0, 1);
	dLight2.Direction = XMFLOAT3(-1, .5f, .1f);

	// Real-Time Shadow Data Descriptions

	// Create the actual texture that will be the shadow map
	D3D11_TEXTURE2D_DESC shadowTexDesc = {};
	shadowTexDesc.Width = shadowTexDesc.Height = 1024;
	shadowTexDesc.MipLevels = 1;
	shadowTexDesc.ArraySize = 1;
	shadowTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowTexDesc.CPUAccessFlags = 0;
	shadowTexDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowTexDesc.MipLevels = 1;
	shadowTexDesc.MiscFlags = 0;
	shadowTexDesc.SampleDesc.Count = 1;
	shadowTexDesc.SampleDesc.Quality = 0;
	shadowTexDesc.Usage = D3D11_USAGE_DEFAULT;
	device->CreateTexture2D(&shadowTexDesc, NULL, &shadowTexture);

	// Create the depth/stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = {};
	shadowDSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDSDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &shadowDepth);

	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC shadowSRVDesc = {};
	shadowSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shadowSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowSRVDesc.Texture2D.MipLevels = 1;
	shadowSRVDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowTexture, &shadowSRVDesc, &shadowSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();
	
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&shadowSampDesc, &shadowSampler);
		
	// Create a rasterizer state
	D3D11_RASTERIZER_DESC shadowRastDesc = {};
	shadowRastDesc.FillMode = D3D11_FILL_SOLID;
	shadowRastDesc.CullMode = D3D11_CULL_BACK;
	shadowRastDesc.DepthClipEnable = true;
	shadowRastDesc.DepthBias = 1000; // Multiplied by (smallest possible value > 0 in depth buffer)
	shadowRastDesc.DepthBiasClamp = 0.0f;
	shadowRastDesc.SlopeScaledDepthBias = 1.0f;
	device->CreateRasterizerState(&shadowRastDesc, &shadowRasterizer);

	// Create the shadow viewport
	shadowViewport = {};
	shadowViewport.TopLeftX = 0;
	shadowViewport.TopLeftY = 0;
	shadowViewport.Width = 1024;
	shadowViewport.Height = 1024;
	shadowViewport.MinDepth = 0.0f;
	shadowViewport.MaxDepth = 1.0f;

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//	srv->Release();
	//	sampState->Release();
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	shadowShader = new SimpleVertexShader(device, context);
	shadowShader->LoadShaderFile(L"ShadowShader.cso");
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	camera->UpdateProjectionMatrix((float)width, (float)height);

	// Calculate the shadow map view and projection for the light
	XMMATRIX shView = XMMatrixLookAtLH(
		XMVectorSet(0, 20, -20, 0),	// Start back and in the air
		XMVectorSet(0, 0, 0, 0),	// Look at the origin
		XMVectorSet(0, 1, 0, 0));	// Up is up
	XMStoreFloat4x4(&shadowView, XMMatrixTranspose(shView));

	XMMATRIX shProj = XMMatrixOrthographicLH(20.0f, 20.0f, 0.1f, 100.0f);
	XMStoreFloat4x4(&shadowProjection, XMMatrixTranspose(shProj));
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	testBox = true;
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT3 norm = XMFLOAT3(0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv = XMFLOAT2(0.0f, 1.0f);
	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices[] =
	{
		{ XMFLOAT3(+2.1f, +0.5f, +0.0f), norm, XMFLOAT2(0.5f, 1.0f)},
		{ XMFLOAT3(+3.6f, -1.5f, +0.0f), norm, XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(+0.6f, -1.5f, +0.0f), norm, XMFLOAT2(0.0f, 0.0f) },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int indices[] = { 0, 1, 2 };


	//mesh1 = new Mesh(vertices, 3, indices, 3, device);


	Vertex squareVertices[] =
	{
		//{ XMFLOAT3(+1.0f, +1.0f, +0.0f), red },
		//{ XMFLOAT3(+1.0f, -1.0f, +0.0f), blue },
		//{ XMFLOAT3(-1.0f, -1.0f, +0.0f), green },
		//{ XMFLOAT3(-1.0f, +1.0f, +0.0f), red }

		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), norm, XMFLOAT2(1.0f, 0.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, +1.0f), norm, XMFLOAT2(1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, +1.0f), norm, XMFLOAT2(0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, +1.0f), norm, XMFLOAT2(0.0f, 0.0f) }
	};
	unsigned int squareIndices[]{ 0, 1, 2,
								  2, 3, 0 };
	//mesh2 = new Mesh(squareVertices, 4, squareIndices, 6, device);


	Vertex trapezoidVertices[] =
	{
		{ XMFLOAT3(-1.5f, +0.5f, +1.0f), norm, XMFLOAT2(0.0f,	0.0f) },//0
		{ XMFLOAT3(-1.0f, +1.5f, +1.0f), norm, XMFLOAT2(0.165f, 1.0f) },//1
		{ XMFLOAT3(+1.0f, +1.5f, +1.0f), norm, XMFLOAT2(0.825f, 1.0f) },//2
		{ XMFLOAT3(+1.5f, +0.5f, +1.0f), norm, XMFLOAT2(1.0f,	0.0f) },//3
		{ XMFLOAT3(+1.0f, +0.5f, +1.0f), norm, XMFLOAT2(0.825f, 0.0f) },//4
		{ XMFLOAT3(-1.0f, +0.5f, +1.0f), norm, XMFLOAT2(0.165f, 0.0f) }//5

	};
	unsigned int trapezoidIndices[]{ 0,1,5,
									 5,1,2,
									 2,3,4,
									 4,5,2 };
	//mesh3 = new Mesh(trapezoidVertices, 6, trapezoidIndices, 12, device);

	mesh1 = new Mesh(squareVertices, 4, squareIndices, 6, device);
	mesh2 = new Mesh(squareVertices, 4, squareIndices, 6, device);
	mesh3 = new Mesh(squareVertices, 4, squareIndices, 6, device);


	char* meshModel1File = "../../Assets/Models/cone.obj";

	meshModel1 = new Mesh(meshModel1File, device);
	meshArraySize = 4;
	meshArray = new Mesh*[meshArraySize + 1];
	meshArray[0] = mesh1;
	meshArray[1] = mesh2;
	meshArray[2] = mesh3;

	ge1 = new GameEntity(meshArray[0], mat1);

	wall1 = new GameEntity(meshArray[0], mat1);

	wall2 = new GameEntity(meshArray[0], mat1);

	wall3 = new GameEntity(meshArray[0], mat1);

	wall4 = new GameEntity(meshArray[0], mat1);

	wall5 = new GameEntity(meshArray[0], mat1);

	wall6 = new GameEntity(meshArray[0], mat1);



	//hallway
	wall7	= new GameEntity(meshArray[0], mat1);
	wall8	= new GameEntity(meshArray[0], mat1);
	wall9	= new GameEntity(meshArray[0], mat1);
	wall10	= new GameEntity(meshArray[0], mat1);

	wall11	= new GameEntity(meshArray[0], mat1);
	wall12	= new GameEntity(meshArray[0], mat1);
	wall13	= new GameEntity(meshArray[0], mat1);
	wall14	= new GameEntity(meshArray[0], mat1);
	wall15	= new GameEntity(meshArray[0], mat1);
	wall16	= new GameEntity(meshArray[0], mat1);
	wall17	= new GameEntity(meshArray[0], mat1);
	wall18	= new GameEntity(meshArray[0], mat1);
	wall19	= new GameEntity(meshArray[0], mat1);
	wall20	= new GameEntity(meshArray[0], mat1);
	wall21 = new GameEntity(meshArray[0], mat1);


	wallEnd = new GameEntity(meshArray[0], mat1);

	//wall7 = new GameEntity(meshArray[0], mat1);

	gePlayer = new GameEntity(meshArray[0]);

	gePlayer->SetCollisionBox(.1f, .1f, .1f);


	e0 = new Enemy(meshModel1, mat1, meshModel1, mat1, gePlayer);
	e0->SetWanderPoints(XMFLOAT3(0, 0, 0), XMFLOAT3(5, 5, 5));
	e0->SetCollisionBox(.1f, .1f, .1f);

	if (testBox) {
		entityArraySize = 23;

		entityArray = new GameEntity*[entityArraySize + 1];
		entityArray[0] = ge1;
		entityArray[1] = wall1;
		entityArray[2] = wall2;
		entityArray[3] = wall3;
		entityArray[4] = wall4;
		entityArray[5] = wall5;
		entityArray[6] = wall6;
		//
		entityArray[7] = wall7;
		entityArray[8] = wall8;
		entityArray[9] = wall9;
		entityArray[10] = wall10;

		entityArray[11] = wall11;
		entityArray[12] = wall12;
		entityArray[13] = wall13;
		entityArray[14] = wall14;
		entityArray[15] = wall15;
		entityArray[16] = wall16;
		entityArray[17] = wall17;
		entityArray[18] = wall18;
		entityArray[19] = wall19;
		entityArray[20] = wall20;
		entityArray[21] = wall21;


		entityArray[22] = wallEnd;



		for (int i = 0; i < entityArraySize; i++) {
			entityArray[i]->SetCollisionBox(1.0f, 1.0f, .02f);
		}

		//XMFLOAT3 rotate
		//XMFLOAT3 scaleValue = XMFLOAT3(10.0f, 10.0f, 10.0f);
		//Reserved for any random entity


		wall5->SetCollisionBox(1.0f, 1.0f, 1.0f);
		wall6->SetCollisionBox(1.0f, 1.0f, 1.0f);
		//ge8->SetCollisionBox(1.0f, 1.0f, 1.0f);

		//Test wall/entity

		ge1->TransformTranslation(movementLeft);
		ge1->SetCollisionBox(1.0f, 1.0f, .01f);
		ge1->TransformTranslation(movementLeft);
		ge1->UpdateEntity();

		BuildLevelGeometry();
	}




}

void Game::BuildLevelGeometry() {


	//Forward and back walls
	wall1->TransformTranslation(movementForward);
	wall1->TransformTranslation(movementForward);
	wall2->TransformRotation(rotation180AroundY); //back wall
	wall1->UpdateEntity(); //moving wall1 back of hallway
	wall1->SetCollisionBox(1.0f, 1.0f, 0.2f);
	wall2->SetCollisionBox(1.0f, 1.0f, 0.2f);
	wall2->UpdateEntity();

	//Left and right walls
	wall3->TransformRotation(rotate90CCWAroundY);
	wall3->UpdateEntity(); //left
	wall3->SetCollisionBox(0.2f, 1.0f, 1.0f);
	wall4->TransformRotation(rotate90CWAroundY);
	wall4->SetCollisionBox(0.2f, 1.0f, 1.0f);
	wall4->UpdateEntity(); //right

						   //Top and Bottom walls
	wall5->TransformRotation(rotate90CCWAroundX);
	wall5->UpdateEntity();
	wall5->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall6->TransformRotation(rotate90CWAroundX);
	wall6->SetCollisionBox(1.0f, 0.2f, 1.0f);
	wall6->UpdateEntity();

	//set 2 initiation
	wall7->TransformTranslation(movementForward);
	wall7->TransformRotation(rotate90CCWAroundX);
	wall7->UpdateEntity();
	wall7->SetCollisionBox(1.0f, 0.2f, 1.0f);
	//wall7->SetCollisionBox(1.0f, 0.2f, 1.0f);

	wall8->TransformTranslation(movementForward);
	wall8->TransformRotation(rotate90CWAroundX);
	wall8->UpdateEntity();
	wall8->SetCollisionBox(1.0f, 0.2f, 1.0f);
	//wall8->SetCollisionBox(1.0f, 0.2f, 1.0f);

	wall9->TransformTranslation(movementForward);
	wall9->TransformRotation(rotate90CCWAroundY);
	wall9->UpdateEntity();
	wall9->SetCollisionBox(0.2f, 1.0f, 1.0f);


	wall10->TransformTranslation(movementForward);
	wall10->TransformRotation(rotate90CWAroundY);
	wall10->UpdateEntity();
	wall10->SetCollisionBox(0.2f, 1.0f, 1.0f);



	//shaft prep
	wall11->TransformTranslation(movementForward);
	wall11->TransformTranslation(movementForward);
	wall11->TransformRotation(rotate90CWAroundX);
	wall11->UpdateEntity(); //floor
	wall11->SetCollisionBox(1.0f, 0.2f, 1.0f);

	wall12->TransformTranslation(movementForward);
	wall12->TransformTranslation(movementForward);
	wall12->TransformRotation(rotate90CCWAroundY);
	wall12->UpdateEntity(); //left
	wall12->SetCollisionBox(0.2f, 1.0f, 0.2f);

	wall13->TransformTranslation(movementForward);
	wall13->TransformTranslation(movementForward);
	wall13->TransformRotation(rotate90CWAroundY);
	wall13->UpdateEntity(); //right
	wall13->SetCollisionBox(0.2f, 1.0f, 0.2f);

	wall14->TransformTranslation(movementForward);
	wall14->TransformTranslation(movementForward);
	wall14->TransformTranslation(movementUp);
	wall14->TransformRotation(rotate90CCWAroundY);
	wall14->UpdateEntity(); //left 2

	wall14->SetCollisionBox(0.2f, 1.0f, 0.2f);

	wall15->TransformTranslation(movementForward);
	wall15->TransformTranslation(movementForward);
	wall15->TransformTranslation(movementUp);
	wall15->TransformRotation(rotate90CWAroundY);
	wall15->UpdateEntity(); //right 2

	wall15->SetCollisionBox(0.2f, 1.0f, 0.2f);

	wall16->TransformTranslation(movementForward);
	wall16->TransformTranslation(movementForward);
	wall16->TransformTranslation(movementUp);
	wall16->TransformTranslation(movementUp);
	wall16->TransformRotation(rotate90CCWAroundY);
	wall16->UpdateEntity(); //left 3

	wall16->SetCollisionBox(0.2f, 1.0f, 0.2f);

	wall17->TransformTranslation(movementForward);
	wall17->TransformTranslation(movementForward);
	wall17->TransformTranslation(movementUp);
	wall17->TransformTranslation(movementUp);
	wall17->TransformRotation(rotate90CWAroundY);
	wall17->UpdateEntity(); //right 3

	wall17->SetCollisionBox(0.2f, 1.0f, 0.2f);

	wall18->TransformTranslation(movementForward);
	wall18->TransformTranslation(movementForward);
	wall18->TransformTranslation(movementUp);
	wall18->TransformTranslation(movementUp);
	wall18->TransformRotation(rotate90CCWAroundX);
	wall18->UpdateEntity(); //TOP

	wall18->SetCollisionBox(1.0f, 0.2f, 1.0f);

	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementForward);
	wall19->TransformTranslation(movementUp);
	wall19->TransformRotation(rotation180AroundY);
	wall19->UpdateEntity(); //back 1

	wall19->SetCollisionBox(1.0f, 1.0f, 0.2f);

	wall20->TransformTranslation(movementForward);
	wall20->TransformTranslation(movementForward);
	wall20->TransformTranslation(movementUp);
	wall20->TransformTranslation(movementUp);
	wall20->TransformRotation(rotation180AroundY);
	wall20->UpdateEntity(); // back 2

	wall20->SetCollisionBox(1.0f, 1.0f, 0.2f);

	wall21->TransformTranslation(movementForward);
	wall21->TransformTranslation(movementForward);
	wall21->TransformTranslation(movementUp);
	wall21->UpdateEntity(); // front 1

	wall21->SetCollisionBox(1.0f, 1.0f, 0.2f);

	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementForward);
	wallEnd->TransformTranslation(movementUp);
	wallEnd->TransformTranslation(movementUp);
	wallEnd->UpdateEntity(); // end cap - currently shaft front w

	wallEnd->SetCollisionBox(1.0f, 1.0f, 0.2f);
}
// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	camera->UpdateProjectionMatrix((float)width, (float)height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	XMFLOAT3 movementValue = XMFLOAT3(1.0f * deltaTime, 0.0f, 0.0f);
	XMFLOAT3 rotationValue = XMFLOAT3(0.0f, 20.0f * deltaTime, 0.0f);
	XMFLOAT3 scaleValue = XMFLOAT3(1.0f * deltaTime, 1.0f * deltaTime, 1.0f* deltaTime);
	//	printf("%d %d %d\n", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	gePlayer->SetPosition(camera->GetPosition());
	//	printf("\nPlayer x: %f, y: %f, z: %f", gePlayer->GetPosition().x, gePlayer->GetPosition().y, gePlayer->GetPosition().z);
		//ge1->TransformRotation(rotationValue);
		//ge1->UpdateEntity();
		//ge2->TransformTranslation(movementValue);
		//ge2->UpdateEntity();
		//ge3->TransformScale(scaleValue);
		//ge3->UpdateEntity();
		//ge4->TransformRotation(rotationValue);
		//ge5->TransformTranslation(movementValue);
		//ge4->UpdateEntity();
	gePlayer->PrintPosition();

	e0->UpdateEntity(deltaTime);

	/*
	printf("\nPlayer position.x: %f", gePlayer->GetPosition().x);
	printf("\nPlayer position.y: %f", gePlayer->GetPosition().y);
	printf("\nPlayer position.z: %f", gePlayer->GetPosition().z);

	printf("\nEnemy position.x: %f", e0->GetPosition().x);
	printf("\nEnemy position.y: %f", e0->GetPosition().y);
	printf("\nEnemy position.z: %f", e0->GetPosition().z);
	*/
	for (int i = 0; i <= entityArraySize - 1; i++) {
		//gePla
		//printf("\nObject: %f\n", i);
		entityArray[i]->UpdateEntity();
		//printf("\nPlayer position.x %f", gePlayer->GetColliderPosition().x);
		//printf("\nPlayer position.y %f", gePlayer->GetColliderPosition().y);
		//printf("\nPlayer position.z %f", gePlayer->GetColliderPosition().z);

		if (collidingMaster.isColliding(gePlayer, entityArray[i])) {
			collidingMaster.isColliding(gePlayer, entityArray[i]);
			printf("collision between player and %d\n", i);
		}
		else {
			//printf("no collision between player and %d\n", i);
		}

		//if(i != )
	}

	camera->Update(deltaTime);
	//ge5->Move(movementValue);
//	ge5->UpdateEntity();
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

#pragma region Render Shadow Map

	// Set shadow map render target
	context->OMSetRenderTargets(0, 0, shadowDepth);
	// Set the shadow map depth view
	context->ClearDepthStencilView(shadowDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);
	// Set the shadow rasterizer
	context->RSSetState(shadowRasterizer);
	
	// Get the existing viewport
	D3D11_VIEWPORT viewport;
	UINT num = 1;
	context->RSGetViewports(&num, &viewport);
	// Set the viewport to the shadow map
	context->RSSetViewports(num, &shadowViewport);

	// Set the shadow vertex shader
	shadowShader->SetShader();
	shadowShader->SetMatrix4x4("view", shadowView);
	shadowShader->SetMatrix4x4("projection", shadowProjection);

	// Turn OFF the pixel shader
	context->PSSetShader(0, 0, 0);

	for (int i = 0; i <= entityArraySize - 1; i++) {
		if (entityArray[i] != '\0') {
			// Set the world position data
			shadowShader->SetMatrix4x4("world", entityArray[i]->GetWorldMatrix());
			shadowShader->CopyAllBufferData();

			// Set buffers in the input assembler
			ID3D11Buffer* vertexHolder = entityArray[i]->GetVertBuffer();
			context->IASetVertexBuffers(0, 1, &vertexHolder, &stride, &offset);
			context->IASetIndexBuffer(entityArray[i]->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// Finally do the actual drawing
			context->DrawIndexed(entityArray[i]->GetIndCount(),	0, 0);
		}
	}

	// Reset it to the default render target
	context->OMSetRenderTargets(1, &backBufferRTV, depthStencilView);
	// Set it to the standard rasterizer
	context->RSSetState(0);
	// Set it back to the default viewport
	context->RSSetViewports(num, &viewport);
#pragma endregion

#pragma region Render Full Scene

	pixelShader->SetData(
		"light",
		&dLight,
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"light2",
		&dLight2,
		sizeof(DirectionalLight));


	// Linear Fog
	// vertexShader->SetFloat("fogStart", 0.5f);
	// vertexShader->SetFloat("fogEnd", 3.0f);

	// Exponential Fog
	vertexShader->SetFloat("fogDensity", 0.66f);

	// Fog Color
	pixelShader->SetFloat4("fogColor", { 0.5f, 0.5f, 0.5f, 1.0f });

	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	//vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("shadowView", shadowView);
	vertexShader->SetMatrix4x4("shadowProjection", shadowProjection);

	// Draw the enemy
	pixelShader->SetShaderResourceView("shadowSRV", shadowSRV);
	pixelShader->SetSamplerState("shadowSampler", shadowSampler);
	e0->PrepareMaterial(camera->GetCamMatrix(), camera->GetProjectionMatrix());
	ID3D11Buffer* vert = e0->GetVertBuffer();
	context->IASetVertexBuffers(0, 1, &vert, &stride, &offset);
	context->IASetIndexBuffer(e0->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(e0->GetIndCount(), 0, 0);

	// Draw the entity array
	for (int i = 0; i <= entityArraySize - 1; i++) {
		if (entityArray[i] != '\0') {

			// Pass in the shadow data
			pixelShader->SetShaderResourceView("shadowSRV", shadowSRV);
			pixelShader->SetSamplerState("shadowSampler", shadowSampler);

			// Prepare all the mateiral data for the shaders
			entityArray[i]->PrepareMaterial(camera->GetCamMatrix(), camera->GetProjectionMatrix());

			// Set buffers in the input assembler
			//  - Do this ONCE PER OBJECT you're drawing, since each object might
			//    have different geometry.
			ID3D11Buffer* vertexHolder = entityArray[i]->GetVertBuffer();
			context->IASetVertexBuffers(0, 1, &vertexHolder, &stride, &offset);
			context->IASetIndexBuffer(entityArray[i]->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);

			// Finally do the actual drawing
			//  - Do this ONCE PER OBJECT you intend to draw
			//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
			//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
			//     vertices in the currently set VERTEX BUFFER
			context->DrawIndexed(
				entityArray[i]->GetIndCount(),
				0,
				0);
		}
	}

#pragma endregion

	// Reset any states we've changed for the next frame!
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

	// Be sure to unbind any shader resources for the next frame
	ID3D11ShaderResourceView* nullSRV[16] = {};
	context->PSSetShaderResources(0, 16, nullSRV);

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	//camera->MouseRotateX(x - (float)prevMousePos.x);
	//camera->MouseRotateY(y - (float)prevMousePos.y);
	// Save the previous mouse position, so we have it for the future
	//prevMousePos.x = x;
	//prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}

#pragma endregion