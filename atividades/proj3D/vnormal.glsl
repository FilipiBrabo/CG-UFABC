#version 400

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;

out vec4 inoutColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main ()
{
    vec4 VMvPosition = viewMatrix * modelMatrix * vPosition;
    vec3 N = normalize(mat3(viewMatrix) * normalMatrix * vNormal);

    inoutColor = vec4(abs(N), 1);

    gl_Position = projectionMatrix * VMvPosition;
}
