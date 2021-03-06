// auto parameter.
float4x4 Auto_WorldViewProjMatrix;


struct VS_Input
{
	float4 pos : POSITION0;
	float3 norm : NORMAL0;
	float2 texcoord : TEXCOORD0;
};
struct VS_Output
{
	float4 hpos : POSITION0;
	float2 texcoord : TEXCOORD0;
};

VS_Output main(in VS_Input vsin)
{
	VS_Output vsout;
	
	float4 wpos = float4(vsin.pos.xyz, 1.0f);
	vsout.hpos = mul(Auto_WorldViewProjMatrix, wpos);
	
	vsout.texcoord = vsin.texcoord;
	
	return vsout;
}