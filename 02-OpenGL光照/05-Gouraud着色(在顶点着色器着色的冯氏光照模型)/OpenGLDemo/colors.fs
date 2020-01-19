#version 330 core
in vec3 LightingColor;    // 光照颜色
out vec4 FragColor;       // 输出颜色

uniform vec3 objectColor;  // 物体颜色

void main()
{
    FragColor = LightingColor * objectColor;;
}
