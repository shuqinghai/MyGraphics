#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexcoord;
out vec2 TexCoords;

void main()//������Ⱦ֡���滭�����ɫ�����Ƚϼ�
{
    gl_Position = vec4(vPosition,1.0);
    TexCoords = vTexcoord;
}  
