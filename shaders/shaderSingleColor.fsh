#version 330 core
out vec4 FragColor;
in VS_OUT
{
    vec2 texcoord;  //��������
    vec3 worldPos;//��������
    vec3 normal;//������
} fs_in;
void main()
{
    FragColor = vec4(0.0,1.0,0.0,1.0);
}
