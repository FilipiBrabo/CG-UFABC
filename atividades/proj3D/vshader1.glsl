#version 400

layout (location = 0) in vec4 vPosition;

out vec4 v2fcolor ;

void main()
{
    vec4 vPosition2 = vPosition.xyzw;
    vPosition2.z *= -1;
    gl_Position = vPosition2;
    float gray = (gl_Position.z + 1) * 0.5;
    v2fcolor = vec4(gray, gray, gray, 1);
}
