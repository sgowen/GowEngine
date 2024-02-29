uniform mat4 u_matrix;

attribute vec4 a_vertex;

varying vec2 v_texCoord;

void main()
{
    v_texCoord = vec2(a_vertex.z, a_vertex.w);
    
    gl_Position = u_matrix * vec4(a_vertex.x, a_vertex.y, 0.0, 1.0);
}
