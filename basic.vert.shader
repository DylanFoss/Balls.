#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec4 v_Color;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * a_Position;
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
};