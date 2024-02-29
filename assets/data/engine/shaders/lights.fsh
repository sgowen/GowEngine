#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texCoord;

uniform mat4 u_matrix;
uniform vec4 u_lightPositions[12];
uniform vec4 u_lightColors[12];
uniform vec4 u_ambientColor;
uniform vec4 u_falloff;
uniform ivec4 u_numLights;

uniform sampler2D u_texture;
uniform sampler2D u_normalMap;

void main()
{
    vec4 diffuseColor = texture2D(u_texture, v_texCoord);
    vec3 normalMapColor = texture2D(u_normalMap, v_texCoord).rgb;
    vec3 ambient = u_ambientColor.rgb * u_ambientColor.a;
    
    vec3 summ = vec3(0.0);
    for (int i = 0; i < u_numLights.x; ++i)
    {
        vec4 lightPosIn = u_lightPositions[i];
        vec4 light = vec4(lightPosIn.x, lightPosIn.y, 0, 1);
        vec4 lightNorm = u_matrix * light;
        vec2 c = vec2(lightNorm.x, lightNorm.y);
        vec2 lightPos = (c + 1.0) / 2.0;
        vec3 LightDir = vec3(lightPos.xy - v_texCoord, lightPosIn.z);
        float D = length(LightDir);
        vec3 N = normalize(normalMapColor * 2.0 - 1.0);
        vec3 L = normalize(LightDir);
        vec3 Diffuse = (u_lightColors[i].rgb * u_lightColors[i].a) * max(dot(N, L), 0.0);
        float Attenuation = 1.0 / (u_falloff.x + (u_falloff.y*D) + (u_falloff.z*D*D));
        vec3 Intensity = Diffuse * Attenuation;
        summ += Intensity;
    }
    
    summ += ambient;
    gl_FragColor = vec4(summ * diffuseColor.rgb, diffuseColor.a);
}
