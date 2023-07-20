#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
out vec2 TexCoords;

void main()//用于渲染帧缓存画面的着色器，比较简单
{
    gl_Position = vec4(vPosition,1.0);
    TexCoords = vTexcoord;
}  
