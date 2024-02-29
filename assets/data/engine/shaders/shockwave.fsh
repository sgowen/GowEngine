#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D u_texture;
uniform float u_timeElapsed;
uniform int u_isTransforming;

varying vec2 v_texCoord;
varying vec2 v_center;

void main()
{
    vec2 uv = v_texCoord;
    vec2 center = vec2(v_center.x, v_center.y);
    vec2 texCoord = uv;
    float distance = distance(uv, center);
    float time = u_timeElapsed;
    if ((distance <= (time + 0.1)) && (distance >= (time - 0.1)))
    {
        float diff = (distance - time);
        float powDiff = 1.0 - pow(abs(diff * 10.0), 0.8);
        float diffTime = diff  * powDiff;
        vec2 diffUV = normalize(uv - center);
        texCoord = uv + (diffUV * diffTime);
    }
    
    gl_FragColor = texture2D(u_texture, texCoord);
    
    if (u_isTransforming == 1 && distance < time)
    {
        gl_FragColor.r *= 1.2;
        gl_FragColor.b *= 1.1;
    }
    else if (u_isTransforming == 0 && distance > time)
    {
        gl_FragColor.r *= 1.2;
        gl_FragColor.b *= 1.1;
    }
}
