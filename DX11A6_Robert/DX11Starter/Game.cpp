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
	delete ge2;
	delete ge3;
	delete ge4;
	delete ge5;
	delete ge6;
	delete ge7;
	delete gePlayer;

	delete[] entityArray;
	delete[] meshArray;

	delete mat1;
	delete camera;

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

	dLight.AmbientColor = XMFLOAT4(0.1, 0.1, 0.1, 1.0);
	dLight.DiffuseColor = XMFLOAT4(0, 0, 1, 1);
	dLight.Direction = XMFLOAT3(1, -1, 0);

	dLight2.AmbientColor = XMFLOAT4(0.3, 0.3, 0.3, 1.0);
	dLight2.DiffuseColor = XMFLOAT4(.5, .5, 0, 1);
	dLight2.Direction = XMFLOAT3(-1, .5, .1);

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
	meshArray = new Mesh*[meshArraySize+1];
	meshArray[0] = mesh1;
	meshArray[1] = mesh2;
	meshArray[2] = mesh3;

	ge1 = new GameEntity(meshArray[0], mat1);

	ge2 = new GameEntity(meshArray[0], mat1);

	ge3 = new GameEntity(meshArray[0], mat1);

	ge4 = new GameEntity(meshArray[0], mat1);

	ge5 = new GameEntity(meshArray[0], mat1);

	ge6 = new GameEntity(meshArray[0], mat1);

	ge7 = new GameEntity(meshArray[0], mat1);

	gePlayer = new GameEntity(meshArray[0]);
	gePlayer->SetCollisionBox(.1f, .1f, .1f);
	if (testBox) {
		entityArraySize = 7;

		entityArray = new GameEntity*[entityArraySize + 1];
		entityArray[0] = ge1;
		entityArray[1] = ge2;
		entityArray[2] = ge3;
		entityArray[3] = ge4;
		entityArray[4] = ge5;
		entityArray[5] = ge6;
		entityArray[6] = ge7;

		for (int i = 0; i < entityArraySize; i++) {
			entityArray[i]->SetCollisionBox(1.0f, 1.0f, .02);
		}
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
		//XMFLOAT3 scaleValue = XMFLOAT3(10.0f, 10.0f, 10.0f);
		//Reserved for any random entity
		ge1->TransformTranslation(movementLeft);
		
		ge1->TransformTranslation(movementLeft);
		ge1->UpdateEntity();
		//Forward and back walls
		ge2->UpdateEntity();


		ge3->TransformRotation(rotation180AroundY); //back wall
		ge3->UpdateEntity();

		//Left and right walls
		ge4->TransformRotation(rotate90CCWAroundY);
		ge4->UpdateEntity(); //left

		ge5->TransformRotation(rotate90CWAroundY);
		ge5->UpdateEntity(); //right

		//Top and Bottom walls
		ge6->TransformRotation(rotate90CCWAroundX);
		ge6->UpdateEntity();
		ge7->TransformRotation(rotate90CWAroundX);
		ge7->UpdateEntity();
	}



	
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
	for (int i = 0; i <= entityArraySize - 1; i++) {
		//gePla
		entityArray[i]->UpdateEntity();
		if (collidingMaster.isColliding(gePlayer, entityArray[i])) {
		//	printf("collision between player and %d\n",i);
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

	pixelShader->SetData(
		"light",
		&dLight,
		sizeof(DirectionalLight));
	pixelShader->SetData(
		"light2",
		&dLight2,
		sizeof(DirectionalLight));
		
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
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for (int i = 0; i <= entityArraySize - 1; i++) {
		if (entityArray[i] != '\0') {
			entityArray[i]->PrepareMaterial(camera->GetCamMatrix(), camera->GetProjectionMatrix());
			//pixelShader->CopyAllBufferData();

			ID3D11Buffer* vertexHolder = entityArray[i]->GetVertBuffer();
			context->IASetVertexBuffers(0, 1, &vertexHolder, &stride, &offset);
			
			context->IASetIndexBuffer(entityArray[i]->GetIndBuffer(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(
				entityArray[i]->GetIndCount(),
				0,
				0);
		}
	}

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	//vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	//vertexShader->SetShader();
	//pixelShader->SetShader();

	// Set buffers in the input assembler
	//  - Do this ONCE PER OBJECT you're drawing, since each object might
	//    have different geometry.
	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Finally do the actual drawing
	//  - Do this ONCE PER OBJECT you intend to draw
	//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
	//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
	//     vertices in the currently set VERTEX BUFFER
	//context->DrawIndexed(
	//	3,     // The number of indices to use (we could draw a subset if we wanted)
	//	0,     // Offset to the first index we want to use
	//	0);    // Offset to add to each index when looking up vertices
/*
	ID3D11Buffer* m1VertexHolder = mesh1->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &m1VertexHolder, &stride, &offset);
	context->IASetIndexBuffer(mesh1->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		3,
		0,
		0);

		*/
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