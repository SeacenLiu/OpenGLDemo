#version 330 core
layout (location = 0) in vec3 aPos;       // 位置坐标
layout (location = 1) in vec3 aNormal;    // 法向量
layout (location = 2) in vec2 aTexCoords; // 纹理坐标

out vec3 FragPos;   // 渲染位置
out vec3 Normal;    // 法向量
out vec2 TexCoords; // 纹理坐标

uniform mat4 model;                 // 模型矩阵
uniform mat4 view;                  // 视图矩阵
uniform mat4 projection;            // 投影矩阵

void main()
{
    // 世界空间中的顶点位置
    FragPos = vec3(model * vec4(aPos, 1.0));
    // 用模型矩阵左上角的逆矩阵的转置矩阵移除对法向量错误缩放(不等比缩放)的影响
    // http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
}
