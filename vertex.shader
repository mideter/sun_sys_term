#version 300 es

uniform highp mat4 modelViewMatrix;
uniform highp mat3 normalMatrix;
uniform highp mat4 projectionMatrix;
uniform highp mat4 modelViewProjectionMatrix;

uniform bool isSkybox;

layout(location = 0) in highp vec3 vertexPosition;
layout(location = 1) in mediump vec3 vertexNormal;
layout(location = 2) in mediump vec2 vertexTextureCoord;

out mediump vec3 normal;
out highp vec3 position;
out mediump vec2 textureCoord;


void main(void) {
    normal = normalize(normalMatrix * vertexNormal);
    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
    textureCoord = vertexTextureCoord;

    gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
}
