#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform UboScene
{
	mat4 projection;
	mat4 view;
} scene;

layout(location = 0) in vec3 inPosition;

layout(location = 4) in mat4 inModelMatrix;
layout(location = 8) in vec4 inColour;

layout(location = 0) out vec4 outColour;

out gl_PerVertex
{
	vec4 gl_Position;
};

void main()
{
	vec4 worldPosition = inModelMatrix * vec4(inPosition, 1.0f);

	gl_Position = scene.projection * scene.view * worldPosition;

	outColour = inColour;
}
