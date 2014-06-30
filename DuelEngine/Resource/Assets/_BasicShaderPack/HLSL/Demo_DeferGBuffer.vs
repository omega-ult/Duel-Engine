// auto parameter.
float4x4 Auto_WorldViewProjMatrix;
float4x4 Auto_WorldViewMatrix;

struct VS_Input
{
	float4 pos : POSITION0;
	float3 norm : NORMAL0;
	float2 texcoord : TEXCOORD0;
};
struct VS_Output
{
	float4 vpos : POSITION0; // view space
	float2 texcoord : TEXCOORD0;
	float3 vnormal : COLOR0;
	float2 hdepth : COLOR1;
};

VS_Output main(in VS_Input vsin)
{
	VS_Output vsout;
	
	vsout.vpos = Auto_WorldViewMatrix * vsin.pos;
	vsout.texcoord = vsin.texcoord;
	vsout.vnormal = (Auto_WorldViewMatrix * float4(vsin.norm, 0.0f)).xyz;
	vsout.hdepth = vsin.pos.zw;
}