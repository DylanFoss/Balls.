#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_State;
uniform vec2 u_Scale;
uniform vec2 u_Cell;

void main() {

    bvec2 boundOne = greaterThanEqual(v_TexCoord, (u_Cell) / u_Scale);
    bvec2 boundTwo = lessThan(v_TexCoord, (u_Cell + vec2(1, 1)) / u_Scale);

    if (boundOne.x && boundTwo.x)
        if (boundOne.y && boundTwo.y)
        {
            o_Color = vec4(vec3(1 - texture(u_State, v_TexCoord).r), 1.0f);
        }
}