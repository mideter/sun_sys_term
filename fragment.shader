#version 300 es

precision mediump float;


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

uniform bool useTexture;
uniform bool useNormal;

uniform lowp sampler2D textureSampler;


in mediump vec3 normal;
in highp vec3 position;
in mediump vec2 textureCoord;


out mediump vec4 glFragColor;


void ads(out vec3 ambAndDiff, out vec3 specular)
{
    vec3 s = normalize(light.position - position);

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
    vec4 textColor = vec4(1.0f);
    if (useTexture)
    {
        textColor = texture2D( textureSampler, textureCoord );
    }

    if (useNormal)
    {
        vec3 ambientAndDiffuse;
        vec3 spec;
        ads(ambientAndDiffuse, spec);
        glFragColor = vec4(ambientAndDiffuse, 1.0) * textColor + vec4(spec, 1.0);
    }
    else
    {
        glFragColor = textColor * 1.2;
    }
}
