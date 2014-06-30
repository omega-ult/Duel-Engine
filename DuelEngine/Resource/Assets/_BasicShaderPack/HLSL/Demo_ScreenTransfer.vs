struct VS_Input
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
};
struct VS_Output
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD0;
};

VS_Output main(in VS_Input vsin)
{
	VS_Output vsout;
	vsout.pos = vsin.pos;
	vsout.texcoord = vsin.texcoord;
	return vsout;
}