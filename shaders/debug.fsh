#version 330 core

in vec2 texcoord;
out vec4 fColor;

// 纹理数据
uniform sampler2D shadowtex;	// shadow纹理

void main()
{       
    fColor = vec4(vec3(texture2D(shadowtex, texcoord).r*0.5+0.5), 1); 
}
