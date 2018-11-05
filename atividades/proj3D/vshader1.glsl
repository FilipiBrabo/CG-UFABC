# version 400

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 vNormal;

uniform mat4 modelMatrix;

out vec4 v2fcolor;

void main()
{   
    // Transform this vertex by modelMatrix
    gl_Position = modelMatrix * vPosition;

    float gray = (gl_Position.z + 1) * 0.5;
    v2fcolor = vec4(gray, gray, gray, 1);
}
