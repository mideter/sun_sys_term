#version 330

uniform highp mat4 modelViewMatrix;
uniform highp mat3 normalMatrix;
uniform highp mat4 projectionMatrix;
uniform highp mat4 mvpMatrix;

attribute highp vec3 vertexPosition;
attribute mediump vec3 vertexNormal;
attribute mediump vec2 vertexTextureCoord;

varying mediump vec3 normal;
varying highp vec3 position;
varying mediump vec2 textureCoord;


void main(void) {
    normal = normalize(normalMatrix * vertexNormal);
    position = vec3(modelViewMatrix * vec4(vertexPosition, 1.0));
    textureCoord = vertexTextureCoord;

    gl_Position = mvpMatrix * vec4(vertexPosition, 1.0);
}
