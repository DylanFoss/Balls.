#shader vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * a_Position;
	v_TexCoord = a_TexCoord;
};