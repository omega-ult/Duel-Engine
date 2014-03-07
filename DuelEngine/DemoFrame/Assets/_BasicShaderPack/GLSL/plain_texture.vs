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


layout(location = VES_POSITION)		in vec3 attrib_position;  
layout(location = VES_TEXCOORD0)	in vec2 attrib_texcoord;  


out	Varing
{
	vec4	pos;
	vec2	texcoord;
} VS_StateOutput;

uniform mat4 gtransform;

void main(void)
{
	vec4 outPos = gtransform * vec4(attrib_position,1.0f);
	gl_Position = outPos;
	VS_StateOutput.pos = outPos;
	VS_StateOutput.texcoord = attrib_texcoord;
}