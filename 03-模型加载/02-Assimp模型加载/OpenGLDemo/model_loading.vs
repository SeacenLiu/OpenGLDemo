#version 330 core
layout (location = 0) in vec3 aPos;       // 位置坐标
layout (location = 1) in vec3 aNormal;    // 法向量
layout (location = 2) in vec2 aTexCoords; // 纹理坐标

out vec2 TexCoords;

uniform mat4 model;                       // 模型矩阵
uniform mat4 view;                        // 视图矩阵
uniform mat4 projection;                  // 投影矩阵

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
