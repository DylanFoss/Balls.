#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_State;
uniform vec2 u_Scale;

vec4 Get(int x, int y)
{
	vec4 texColor = texture(u_State, (v_TexCoord+(vec2(x,y)/u_Scale)));
    texColor.r = texColor.r > 0.5 ? 1.0 : 0.0;
	return texColor;
}

void main() {

    float sum = Get(-1, -1).r +
        Get(-1, 0).r +
        Get(-1, 1).r +
        Get(0, -1).r +
        Get(0, 1).r +
        Get(1, -1).r +
        Get(1, 0).r +
        Get(1, 1).r;

    if (sum == 3)
    {
       o_Color = vec4(vec3(1.0), 1.0);
    }
    else if (sum == 2)
    {
        vec4 current = Get(0, 0);
        o_Color = vec4(current.r, max(current.g - (0.01 * (1 - current.r)), 0.0), 0.0, 1.0);
    }
    else
    {
        vec4 current = Get(0, 0);
        o_Color = vec4(0.0, max(current.g - 0.01, 0.0), 0.0, 1.0);
    }
}