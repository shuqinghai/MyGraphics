#version 330 core
out vec4 FragColor;
in VS_OUT
{
    vec2 texcoord;  //纹理坐标
    vec3 worldPos;//世界坐标
    vec3 normal;//法向量
} fs_in;
void main()
{
    FragColor = vec4(0.0,1.0,0.0,1.0);
}
