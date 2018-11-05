#version 400

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec4 lightPosition[2];

out vec3 fN;
out vec3 fE;
out vec3 fL[2];

void main ()
{
    vec4 VMvPosition = viewMatrix * modelMatrix * vPosition;

    for (int i = 0; i < 2; ++i)
    {
        fL[i] = lightPosition[i].xyz - VMvPosition.xyz;
    }

    fN = mat3(viewMatrix) * normalMatrix * vNormal;
    fE = -VMvPosition.xyz;

    gl_Position = projectionMatrix * VMvPosition;
}
