attribute vec2 a_vertex;

varying vec2 v_texCoord;

void main()
{
    v_texCoord = (a_vertex + 1.0) / 2.0;
    
    gl_Position = vec4(a_vertex, 0, 1);
}
