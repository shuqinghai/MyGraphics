#version 330 core

layout (location = 0) in vec3 vPosition;  // cpu传入的顶点坐标

uniform mat4 model;
uniform mat4 view;//视图矩阵
uniform mat4 projection;//投影矩阵
void main() {
    gl_Position = projection*view*model * vec4(vPosition, 1.0);
}
