#shader fragment
#version 330 core

// Based on the code by:
// Author @patriciogv - 2015
// Title: Mosaic
// found here: https://thebookofshaders.com/10/
// thank you :)

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(54.9898, -28.233))) * 43758.5453123);
}

void main() {
    vec2 ipos = floor(gl_FragCoord.xy);  // get the integer coords
    vec4 color = vec4(vec3(random(ipos)), 1.0f);
    o_Color = color; //.r > 0.9 ? vec4(vec3(1.0),1.0) : vec4(vec3(0.0),1.0);
}