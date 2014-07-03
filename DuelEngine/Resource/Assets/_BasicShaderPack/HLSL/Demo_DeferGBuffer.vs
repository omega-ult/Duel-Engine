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
	float4 hpos : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 vnormal : COLOR0;
	float2 hdepth : COLOR1;
	float4 vpos : COLOR2; // view space
};

VS_Output main(in VS_Input vsin)
{
	VS_Output vsout;
	
	float4 wpos = float4(vsin.pos.xyz, 1.0f);
	vsout.hpos = mul(Auto_WorldViewProjMatrix, wpos);
	
	vsout.vpos = mul(Auto_WorldViewMatrix, wpos);
	vsout.texcoord = vsin.texcoord;
	vsout.vnormal = mul(Auto_WorldViewMatrix, float4(vsin.norm, 0.0f)).xyz;
	vsout.hdepth = vsout.hpos.zw;
	
	return vsout;
}