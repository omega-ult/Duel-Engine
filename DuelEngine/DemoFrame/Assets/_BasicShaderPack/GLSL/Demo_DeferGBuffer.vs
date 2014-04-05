#version 330
#define	VES_POSITION		0
#define	VES_BLENTWEIGHT		1
#define	VES_BLENDINDICES	2
#define	VES_NORMAL			3
#define	VES_DIFFUSE			4
#define	VES_SPECULAR		5
#define	VES_TEXCOORD		6
#define	VES_BINORMAL		14
#define	VES_TANGENT			15

layout(location = VES_POSITION)	in vec3 inPosition;  
layout(location = VES_NORMAL)	in vec3	inNormal;
layout(location = VES_TEXCOORD)	in vec2 inTexcoord;  

// auto parameter.
uniform mat4 Auto_WorldViewProjMatrix;
uniform mat4 Auto_WorldViewMatrix;

out	Varing
{
	vec4	vpos;	// view space
	vec2	texcoord;
	vec3	vnormal;
} VS_StageOutput;

void main(void)
{
	vec4 pos = vec4(inPosition, 1.0f);
	gl_Position = Auto_WorldViewProjMatrix * pos;
	
	VS_StageOutput.vpos = Auto_WorldViewMatrix * pos;
	VS_StageOutput.texcoord = inTexcoord;
	VS_StageOutput.vnormal = (Auto_WorldViewMatrix * vec4(inNormal, 0.0f)).xyz;
}