#version 330 core
in vec3 FragPos;

out vec4 FragColor;  // 输出向量

void main()
{
    // (1.0, 1.0, 1.0, 1.0): 白色的光源
    FragColor = vec4(1.0);
}
