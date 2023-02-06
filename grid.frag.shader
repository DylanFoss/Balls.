#shader fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform vec2 u_GridDimensions;
uniform vec2 u_WindowDimensions;
uniform float u_Zoom;

uniform vec3 u_GridColour;

void main()
{
    vec2 uv = v_TexCoord;
    vec3 color = u_GridColour;
    float a = 1;

    //to-do: potentially change these to uniforms vs working them out in each thread.
    float numCells = min(u_GridDimensions.x, u_GridDimensions.y) * u_Zoom;
    float pixelsPerCell = round(max(u_WindowDimensions.x, u_WindowDimensions.y) / numCells);

    uv *= u_GridDimensions;
    uv = (abs(fract(uv - 0.5) - 0.5) * 0.5) / fwidth(uv);
    float gridMask = min(uv.x, uv.y);

    // alpha is decided by if we are on a gridline
    a = (1. - gridMask);
    //alpha decided based on how many cells are visible (0.05 is a magic number, a value based on how zoomed could be used.) 
    a *= (mix(0., 1., pixelsPerCell * 0.05));

    color = u_GridColour * (1 - gridMask);
    o_Color = vec4(vec3(color), a);
}