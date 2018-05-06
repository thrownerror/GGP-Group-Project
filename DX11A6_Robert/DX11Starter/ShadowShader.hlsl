
// Constant Buffer
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
float4 main(VertexShaderInput input) : SV_POSITION
{
	matrix worldViewProj = mul(mul(world, view), projection);
	return mul(float4(input.position, 1.0f), worldViewProj);
}