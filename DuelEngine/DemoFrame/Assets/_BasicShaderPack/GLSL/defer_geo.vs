// add these macros before any glsl shader.
#version 330
#define	VES_POSITION		0
#define	VES_BLENTWEIGHT		1
#define	VES_BLENDINDICES	2
#define	VES_NORMAL			3
#define	VES_DIFFUSE			4
#define	VES_SPECULAR		5
#define	VES_TEXCOORD0		6
#define	VES_TEXCOORD1		7
#define	VES_TEXCOORD2		8
#define	VES_TEXCOORD3		9
#define	VES_TEXCOORD4		10
#define	VES_TEXCOORD5		11
#define	VES_TEXCOORD6		12
#define	VES_TEXCOORD7		13
#define	VES_BINORMAL		14
#define	VES_TANGENT			15

// use this style in the 
layout(location = VES_POSITION)		in vec3 attrib_position;  
layout(location = VES_NORMAL)		in vec2 attrib_normal;  
layout(location = VES_TEXCOORD0)	in vec3 attrib_normal;  

out	VS_State_Output
{
	vec4	hpos;
	vec2	tex;
	vec3	vnormal;
	vec2	depth;
} VSStateOutput;

uniform mat4x4 gObjectWVP;
uniform mat4x4 gObjectWV;

void main(void)
{
/// lalala 什么语法问题就先不管了.
/*
	VSStateOutput.hpos = mul(float4(pos,1.0f),gObjectWVP);
	VSStateOutput.tex = tex;
	VSStateOutput.vnormal = mul(float4(normal,0.0f),gObjectWV).xyz;
	
	VSStateOutput.depth = VSStateOutput.hpos.zw;
*/
}
