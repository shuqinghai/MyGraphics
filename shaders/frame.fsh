#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
const float offset = 1.0/300.0;
void main()
{ 
//-----------------------------------------用于像素化
//    int pixelSize = 1200;//像素大小
//    FragColor = vec4(vec3(texture2D(screenTexture, floor(TexCoords*pixelSize)/pixelSize)), 1.0);

    FragColor.rgb = texture2D(screenTexture, TexCoords.st).rgb;
    //FragColor = vec4(vec3(texture2D(screenTexture, TexCoords)), 1.0);
    //FragColor =vec4(TexCoords,0.0,1.0);
//-----------------------------------------用卷积或者高斯模糊进行了边缘检测和模糊，不同的kernel对应着不同的效果，该效果可以和原图叠加
//     vec2 offsets[9] = vec2[](
//             vec2(-offset,  offset), // 左上
//             vec2( 0.0f,    offset), // 正上
//             vec2( offset,  offset), // 右上
//             vec2(-offset,  0.0f),   // 左
//             vec2( 0.0f,    0.0f),   // 中
//             vec2( offset,  0.0f),   // 右
//             vec2(-offset, -offset), // 左下
//             vec2( 0.0f,   -offset), // 正下
//             vec2( offset, -offset)  // 右下
//         );

//         float kernel[9] = float[](
//             -1, -1, -1,
//             -1,  8, -1,
//             -1, -1, -1
//         );
////         float kernel[9] = float[](
////             0, -1, 0,
////             -1,  4, -1,
////             0, -1, 0
////         );
////         float kernel[9] = float[](
////             0, -1, 0,
////             -1,  5, -1,
////             0, -1, 0
////         );
////         float kernel[9] = float[](
////             -1, -2, -1,
////             -2,  5, -2,
////             -1, -2, -1
////         );
////     float kernel[9] = float[](
////         1.0 / 16, 2.0 / 16, 1.0 / 16,
////         2.0 / 16, 4.0 / 16, 2.0 / 16,
////         1.0 / 16, 2.0 / 16, 1.0 / 16
////     );
////          float kernel[9] = float[](
////              1.0, 1.0 , 1.0,
////              1.0, -8.0 , 1.0,
////              1.0, 1.0 , 1.0
////          );


//         vec3 sampleTex[9];
//         for(int i = 0; i < 9; i++)
//         {
//             sampleTex[i] = vec3(texture2D(screenTexture, TexCoords.st + offsets[i]));
//         }
//         vec3 col = vec3(0.0);
//         for(int i = 0; i < 9; i++)
//             col += sampleTex[i] * kernel[i];

//         FragColor = vec4(col, 1.0);//+vec4(vec3(texture2D(screenTexture, TexCoords.st)),1.0);
}
