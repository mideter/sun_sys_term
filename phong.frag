#version 330

struct Material {
    lowp vec3 ka;
    lowp vec3 kd;
    lowp vec3 ks;
    lowp float shininess;
};

struct Light {
    lowp vec3 position;
    lowp vec3 intensity;
};

uniform Material mat;
uniform Light light;

uniform sampler2D textureSampler;

varying mediump vec3 normal;
varying highp vec3 position;
varying mediump vec2 textureCoord;


void ads(out vec3 ambAndDiff, out vec3 specular)
{
    vec3 s = normalize( vec3(light.position));

    vec3 viewDir = normalize( vec3(-position) );
    vec3 reflectLightDir = reflect(-s, normal);

    vec3 ambient = light.intensity * mat.ka;
    float SdotN = max( dot(s, normal), 0.0);
 // if(SdotN == 0.0)
 //     SdotN = dot(s, -normal);

    vec3 diffuse = light.intensity * mat.kd * SdotN;
    ambAndDiff = ambient + diffuse;

    specular = vec3(0.0);
    if(SdotN > 0.0)
    {
        float RdotV = max( dot(reflectLightDir, viewDir), 0.0 );
        specular = light.intensity * mat.ks * pow(RdotV, mat.shininess);
    }
}


void main(void) {
  /*  vec3 n = normalize(N);
    vec3 L = normalize(light.position.xyz - v);
    vec3 E = normalize(-v);
    vec3 R = normalize(reflect(-L, n));

    float LdotN = dot(L, n);
    float diffuse = max(LdotN, 0.0);
    vec3 spec = vec3(0, 0, 0);

    if(LdotN > 0.0) {
        float RdotE = max(dot(R, E), 0.0);
        spec = light.intensity * pow(RdotE, mat.shininess);
    }
*/
    vec3 ambientAndDiffuse;
    vec3 spec;
    ads(ambientAndDiffuse, spec);

    vec4 textureColor = texture( textureSampler, textureCoord );

    gl_FragColor = vec4(ambientAndDiffuse, 1.0) * textureColor + vec4(spec, 1.0);
}
