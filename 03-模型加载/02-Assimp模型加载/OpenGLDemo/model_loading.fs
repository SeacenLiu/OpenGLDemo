#version 330 core
out vec4 FragColor;                 // 输出颜色向量

in vec2 TexCoords;                  // 输入纹理坐标

uniform sampler2D texture_diffuse1; // 漫反射纹理

void main()
{
    FragColor = texture(texture_diffuse1, TexCoords);
}
