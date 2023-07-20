#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 42) out;

in VS_OUT
{
    vec2 texcoord;      // 纹理坐标
    vec3 worldPos;      // 世界坐标
    vec3 normal;        // 法向量
} gs_in[];

const float ARROW_LENGTH = 12.0;    // 箭头长度
const float CONE_RADIUS = 20.0;      // 圆锥底部半径
const int NUM_SEGMENTS = 36;        // 圆锥底部分段数

void GenerateArrow(int index)
{
    vec4 position = gl_in[index].gl_Position;
    vec3 normal = normalize(gs_in[index].normal);
    vec4 arrowTip = position + vec4(normal * ARROW_LENGTH * 1.2, 0.0);

    // 计算副法向量
    vec3 auxNormal = cross(normal, vec3(0.0, 0.0, 1.0));
    if (length(auxNormal) < 0.001)
        auxNormal = cross(normal, vec3(0.0, 1.0, 0.0));
    auxNormal = normalize(auxNormal);

    // 计算主法向量
    vec3 mainNormal = cross(normal, auxNormal);

    // 生成箭头顶部
    gl_Position = arrowTip;
    EmitVertex();

    // 生成圆锥底部顶点
    for (int i = 0; i <= NUM_SEGMENTS; i++)
    {
        float angle = radians(float(i) * (360.0 / float(NUM_SEGMENTS)));
        vec3 circlePoint = vec3(cos(angle), sin(angle), 0.0) * CONE_RADIUS;
        vec3 vertexPosition = vec3(position) + normal * ARROW_LENGTH + auxNormal * circlePoint.x + mainNormal * circlePoint.y;
        gl_Position = vec4(vertexPosition, 1.0);
        EmitVertex();
    }

    EndPrimitive();
}

void main()
{
    GenerateArrow(0);
    GenerateArrow(1);
    GenerateArrow(2);
}




//#version 330 core
//layout (triangles) in;
//layout (triangle_strip, max_vertices = 42) out;

//in VS_OUT
//{
//    vec2 texcoord;  // 纹理坐标
//    vec3 worldPos;  // 世界坐标
//    vec3 normal;    // 法向量
//} gs_in[];

//const float MAGNITUDE = 0.05;       // 箭头长度倍数
//const float CONE_HEIGHT = 0.2;      // 圆锥高度
//const float CONE_RADIUS = 0.05;     // 圆锥底部半径
//const float CYLINDER_RADIUS = 0.02; // 圆柱的半径

//void GenerateArrow(int index)
//{
//    vec4 position = gl_in[index].gl_Position;
//    vec3 normal = normalize(gs_in[index].normal);

//    // 生成圆锥部分
//    for (int i = 0; i <= 360; i += 10)
//    {
//        float angle = radians(float(i));
//        vec4 vertexPosition = position + vec4(normal * MAGNITUDE, 0.0) + vec4(cos(angle) * CONE_RADIUS, sin(angle) * CONE_RADIUS, 0.0, 0.0);
//        gl_Position = vertexPosition;
//        EmitVertex();
//    }

//    // 生成圆锥顶部
//    gl_Position = position + vec4(normal * (MAGNITUDE + CONE_HEIGHT), 0.0);
//    EmitVertex();

//    // 生成圆柱部分
//    for (int i = 0; i <= 360; i += 10)
//    {
//        float angle = radians(float(i));
//        vec4 topPosition = position + vec4(normal * (MAGNITUDE + CONE_HEIGHT), 0.0) + vec4(cos(angle) * CYLINDER_RADIUS, sin(angle) * CYLINDER_RADIUS, 0.0, 0.0);
//        vec4 basePosition = position + vec4(normal * MAGNITUDE, 0.0) + vec4(cos(angle) * CYLINDER_RADIUS, sin(angle) * CYLINDER_RADIUS, 0.0, 0.0);

//        gl_Position = topPosition;
//        EmitVertex();

//        gl_Position = basePosition;
//        EmitVertex();
//    }

//    // 生成连接箭头身体的顶部
//    gl_Position = position + vec4(normal * MAGNITUDE, 0.0);
//    EmitVertex();

//    // 生成箭头翼2
//    gl_Position = position + vec4(normal * MAGNITUDE, 0.0) + vec4(normalize(cross(normal, vec3(0.0, 0.0, 1.0))) * MAGNITUDE, 0.0);
//    EmitVertex();

//    EndPrimitive();
//}

//void main()
//{
//    GenerateArrow(0); // 第一个顶点的法向量箭头
//    GenerateArrow(1); // 第二个顶点的法向量箭头
//    GenerateArrow(2); // 第三个顶点的法向量箭头
//}



//#version 330 core
//layout (triangles) in;
//layout (line_strip, max_vertices = 6) out;

//in VS_OUT//接口块的声明和定义要一致，后面的名字可以任意
//{
//    vec2 texcoord;  //纹理坐标
//    vec3 worldPos;//世界坐标
//    vec3 normal;//法向量
//} gs_in[];//是一个数组的形式，是图元构成的，三角形就是012
//const float MAGNITUDE = 0.05;//放大倍数
////gl_in是内置的接口块
//void GenerateLine(int index)
//{
//    gl_Position =  gl_in[index].gl_Position;
//    EmitVertex();//新建一个顶点
//    gl_Position =  (gl_in[index].gl_Position +
//                                vec4(normalize(gs_in[index].normal), 0.0) * MAGNITUDE);
////        gl_Position =  (gl_in[index].gl_Position +
////                                    vec4(0.0,-1.0,0.0, 0.0) * MAGNITUDE);
//    EmitVertex();//又一个
//    EndPrimitive();//结束
//}

//void main()
//{
//    GenerateLine(0); // 三角形第一个顶点法线
//    GenerateLine(1); // 三角形第二个顶点法线
//    GenerateLine(2); // 三角形第三个顶点法线

//}
//#version 330 core
//layout (triangles) in;
//layout (line_strip, max_vertices = 6) out;

//in VS_OUT
//{
//    vec2 texcoord;  // 纹理坐标
//    vec3 worldPos;  // 世界坐标
//    vec3 normal;    // 法向量
//} gs_in[];

//const float MAGNITUDE = 0.1;  // 箭头长度倍数

//void GenerateArrow(int index)
//{
//    vec4 position = gl_in[index].gl_Position;
//    vec3 normal = normalize(gs_in[index].normal);

//    // 箭头翼1
//    gl_Position = position + vec4(normal * MAGNITUDE, 1.0) + vec4(-0.02, 0.0, 0.0, 0.0);
//    EmitVertex();

//    // 箭头顶
//    gl_Position = position + vec4(normal * MAGNITUDE*1.2, 1.0);
//    EmitVertex();

//    // 箭头尾
//    gl_Position = position;
//    EmitVertex();



//    // 箭头顶
//    gl_Position = position + vec4(normal * MAGNITUDE*1.2, 1.0);
//    EmitVertex();


//    // 箭头翼2
//    gl_Position = position + vec4(normal * MAGNITUDE, 1.0) + vec4(0.02, 0.0, 0.0, 0.0);
//    EmitVertex();

//    EndPrimitive();
//}

//void main()
//{
//    GenerateArrow(0); // 三角形第一个顶点法线
//    GenerateArrow(1); // 三角形第二个顶点法线
//    GenerateArrow(2); // 三角形第三个顶点法线
//}

