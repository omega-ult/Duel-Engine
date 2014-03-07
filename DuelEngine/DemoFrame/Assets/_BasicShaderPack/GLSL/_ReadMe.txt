// 为确保shader能够正确和引擎绑定, 每个GLSL shader遵循以下规则
// use following rules to make sure the engine can bind shader correctly.
// add these macros before any glsl vert shader.
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

// use this style in the glsl vert shader,
layout(location = VES_POSITION)		in vec3 attrib_position;  

// make sure there is no naming conflict, use this block style in any shader(vert, geo, frag) 
out	Varing	// use same block name "Varing"
{
	vec4	pos;
} VS_StateOutput;	// user-defined name, use "[StageName]_[UserDefinedName]" [StageName] := VS | GS | PS
