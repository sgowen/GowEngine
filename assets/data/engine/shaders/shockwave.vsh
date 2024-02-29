uniform mat4 u_matrix;

uniform float u_centerX;
uniform float u_centerY;

attribute vec2 a_vertex;

varying vec2 v_texCoord;
varying vec2 v_center;

void main()
{
    gl_Position = u_matrix * vec4(a_vertex, 0.0, 1.0);
    vec2 pos = vec2(gl_Position.x, gl_Position.y);
    v_texCoord = (pos + 1.0) / 2.0;
    
    vec4 center = vec4(u_centerX, u_centerY, 0, 1);
    vec4 screenCenter = u_matrix * center;
    
    vec2 c = vec2(screenCenter.x, screenCenter.y);
    v_center = (c + 1.0) / 2.0;
}
