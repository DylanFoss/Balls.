#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Textures[3];
uniform float u_TexIndex;

void main()
{
	vec3 liveColour = vec3(1., 1., 0.);
	vec3 deadColour = vec3(0., 0., 0.);
	vec3 trailColour = vec3(0., 1., 0.);
	
	int index = int(u_TexIndex);
	vec4 stateInfo = texture(u_Textures[index], v_TexCoord);

	o_Color = stateInfo.r < 1.0 ? vec4(mix(trailColour, deadColour, 1 - stateInfo.g),1.0) : vec4(liveColour, 1.0);
}