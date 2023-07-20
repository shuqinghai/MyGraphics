//#version 330 core
//layout (triangles) in;
//layout (triangle_strip, max_vertices = 3) out;
//in VS_OUT
//{
//    vec2 texcoord;  //纹理坐标
//    vec3 worldPos;//世界坐标
//    vec3 normal;//法向量
//} gs_in[];
//out vec2 TexCoords;
//out vec3 normal;
////uniform float time;

//vec4 explode(vec4 position, vec3 normal)//爆破
//{
//    float magnitude = 2.0;
//    vec3 direction = normal * ((sin(radians(time)) + 1.0) / 2.0) * magnitude;
//    return position + vec4(direction, 0.0);
//}


//vec3 GetNormal()//根据三角形得到平面的法向量
//{
//    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
//    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
//    return normalize(cross(a, b));
//}

//void main() {//该函数会影响线模型的生成，所以暂时没用
////    normal = GetNormal();
//    for(int i=0;i<3;++i)
//    {
//        TexCoords = gs_in[i].texcoord;
//        //gl_Position = explode(gl_in[i].gl_Position, normal);
//        gl_Position = gl_in[i].gl_Position;
//        EmitVertex();
//    }
//    EndPrimitive();
//}

