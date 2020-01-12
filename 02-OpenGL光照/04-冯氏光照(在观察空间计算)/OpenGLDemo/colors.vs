#version 330 core
// 基于观察空间使用冯氏光照模型
layout (location = 0) in vec3 aPos;    // 位置坐标
layout (location = 1) in vec3 aNormal; // 法向量

out vec3 FragPos;  // 渲染位置
out vec3 Normal;   // 法向量
out vec3 LightPos; // 光源位置

uniform vec3 lightPos; // 光源位置

uniform mat4 model;                 // 模型矩阵
uniform mat4 view;                  // 视图矩阵
uniform mat4 projection;            // 投影矩阵

void main()
{
     gl_Position = projection * view * model * vec4(aPos, 1.0);
    // 观察空间的顶点位置
    FragPos = vec3(view * model * vec4(aPos, 1.0));
    // 观察空间的法向量
    Normal = mat3(transpose(inverse(view * model))) * aNormal;
    // 观察空间中的光源位置
    LightPos = vec3(view * vec4(lightPos, 1.0));
}
