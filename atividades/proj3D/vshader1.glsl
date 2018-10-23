#version 400

layout (location = 0) in vec4 vPosition;

uniform mat4 model;
out vec4 v2fcolor;

void main()
{
    gl_Position = model * vPosition;
    float gray = (gl_Position.z + 1) * 0.5;
    v2fcolor = vec4(gray, gray, gray, 1);
}


//layout ( location = 1) in vec3 normal;

//out vec4 inout_color;

//void main() {
//    inout_color = vec4(abs(normal), 1);
//    //gl_Position = ;
//}
