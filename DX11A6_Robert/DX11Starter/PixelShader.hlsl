
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
	float  fogFactor	: FOG;
	float4 posForShadow	: TEXCOORD1;
	//float4 color		: COLOR;

};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float3 Position;
	float3 Color;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight light2;
	PointLight pointLight;

	float3 cameraPosition;

	float4 fogColor;
};

Texture2D	 textureSRV					: register(t0);
Texture2D	 textureNRM					: register(t1);
Texture2D	 shadowSRV					: register(t2);
SamplerState basicSampler				: register(s0);
SamplerComparisonState shadowSampler	: register(s1);


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

	// Sample the texture for the surface color
	float4 surfaceColor = textureSRV.Sample(basicSampler, input.uv);

	// Calculate the surface color after factoring in fog
	float4 surfaceFog = input.fogFactor * surfaceColor + (1.0 - input.fogFactor) * fogColor;

	// Calculate shadow map data

	// Calculate the position of this pixel ON THE SHADOW MAP
	// remembering to flip the Y
	float2 shadowUV = (input.posForShadow.xy / input.posForShadow.w) * 0.5f + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;

	// Calculate the depth of this pixel from the light
	float depthFromLight = input.posForShadow.z / input.posForShadow.w;

	// Actually sample the shadow map and determine if this pixel is in shadow
	float shadowAmount = shadowSRV.SampleCmpLevelZero(shadowSampler, shadowUV, depthFromLight);

	//return float4(1 - shadowAmount, 0, 0, 1);

	// Get the result after applying the first light (plus shadow map for the first light)
	float3 light1Amount = saturate(dot(input.normal, -light.Direction));
	float4 light1Result = float4((light.DiffuseColor * light1Amount) + light.AmbientColor, 1) * surfaceFog * shadowAmount;

	// Get the result after applying the second light
	float3 light2Amount = saturate(dot(input.normal, -light2.Direction));
	float4 light2Result = float4((light2.DiffuseColor * light2Amount) + light2.AmbientColor, 1) * surfaceFog;

	// Get the result after applying the third (specular point) light
	float3 dirToPointLight = normalize(pointLight.Position - input.position);
	float pointNdotL = dot(input.normal, dirToPointLight);
	pointNdotL = saturate(pointNdotL);
	float4 pointLightResult = float4((pointLight.Color * pointNdotL), 1);

	float3 dirToCamera = normalize(cameraPosition - input.position);
	float3 refl = reflect(-dirToPointLight, input.normal);
	float specExp = 128;
	float spec = pow(saturate(dot(dirToCamera, refl)), specExp);

	// Add all together
	return light1Result + light2Result + pointLightResult + spec.xxxx;
}