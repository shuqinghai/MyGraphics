#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
in VS_OUT
{
    vec2 texcoord;  //�?顶�?�纹??????�????????��??
    vec3 worldPos;//顶�?��??�???????
    vec3 normal;//顶�?��??�???
} gl_in[];
out vec2 TexCoords;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude;
    return position + vec4(direction, 0.0);
}


vec3 GetNormal()
{
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}

void main() {
    vec3 normal = GetNormal();
    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gl_in[0].texcoord;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gl_in[1].texcoord;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gl_in[2].texcoord;
    EmitVertex();
    EndPrimitive();
}
