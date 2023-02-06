#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_Textures[3];
uniform float u_TexIndex;

void main()
{
	vec3 liveColour = vec3(1., 1., 1.);
	vec3 deadColour = vec3(0., 0., 0.);
	vec3 trailColour = vec3(0., 1., 0.);

	vec3 heatColour1 = vec3(0.95, 0.43, 0.13);
	vec3 heatColour2 = vec3(1., 0.9, 0.);
	vec3 heatColour3 = vec3(0.1, 0.63, 0.33);
	vec3 heatColour4 = vec3(0.12, 0.48, 0.67);
	

	int index = int(u_TexIndex);
	vec4 stateInfo = texture(u_Textures[index], v_TexCoord);

	if (stateInfo.g > 0.9)
	{
		trailColour = heatColour1;
		deadColour = heatColour2;
	}
	else if (stateInfo.g > 0.8 && stateInfo.g < 0.9)
	{
		trailColour = heatColour2;
		deadColour = liveColour;
	}
	else if (stateInfo.g > 0.7 && stateInfo.g < 0.8)
	{
		trailColour = heatColour3;
		deadColour = heatColour3;
	}
	else if (stateInfo.g > 0.5 && stateInfo.g < 0.7)
	{
		trailColour = heatColour3;
		deadColour = heatColour4;
	}
	else if (stateInfo.g < 0.5)
	{
		trailColour = heatColour4;
		deadColour = vec3(0., 0., 0.);
	}

	o_Color = stateInfo.r < 1.0 ? vec4(mix(trailColour, deadColour, 1 - stateInfo.g),1.0) : vec4(liveColour, 1.0);
};