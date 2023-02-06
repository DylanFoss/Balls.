#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

void main()
{
	float pct = distance(v_TexCoord, vec2(0.5));

	o_Color = vec4(v_TexCoord, 0.0f, pct < 0.5f ? 1.0f : 0.0f);
};