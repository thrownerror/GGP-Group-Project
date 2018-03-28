
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
	//float4 color		: COLOR;

};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
};

Texture2D	 textureSRV			: register(t0);
Texture2D	 textureNRM			: register(t1);
SamplerState basicSampler		: register(s0);


float3 calculateNormal(VertexToPixel input)
{
	// Get the normal vector from the image map
	float3 normalFromMap = textureNRM.Sample(basicSampler, input.uv).rgb;
	normalFromMap = normalize(normalFromMap * 2 - 1);

	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);

	// Create the TBN matrix which we use to convert from tangent to world space
	float3x3 TBN = float3x3(T, B, N);
	return normalize(mul(normalFromMap, TBN));
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Normalize vectors
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// Recalculate normal via normal map
	input.normal = calculateNormal(input);

//	return float4 (input.normal,  1);
//	return float4(input.uv, 0, 1);
	float4 surfaceColor = textureSRV.Sample(basicSampler, input.uv);
	//input.normal = normalize(input.normal);
	float3 negatedDirection = normalize(-light.Direction);
	//negatedDirection = normalize(negatedDirection);

	float3 lightAmount = saturate(dot(input.normal, negatedDirection));
	float4 light1Result = float4((light.DiffuseColor * lightAmount) + light.AmbientColor, 1) * surfaceColor;
	//return float4((light.DiffuseColor * lightAmount) + light.AmbientColor, 1);

	float3 negatedDirection2 = normalize(-light2.Direction);
	float3 light2Amount = saturate(dot(input.normal, negatedDirection2));
	float4 light2Result = float4((light2.DiffuseColor * light2Amount) + light2.AmbientColor, 1) * surfaceColor;

	return light1Result + light2Result;
	//return float4(light.DiffuseColor);//input.normal, 1);
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	//return input.color;
	//return float4(1,0,0,1);
}