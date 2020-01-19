#version 330 core
layout (location = 0) in vec3 aPos;    // 位置坐标
layout (location = 1) in vec3 aNormal; // 法向量

out vec3 LightingColor;   // 光照颜色结果

uniform vec3 lightPos;    // 光源位置
uniform vec3 lightColor;  // 光照颜色
uniform vec3 viewPos;     // 观察者位置（相机位置）

uniform mat4 model;                 // 模型矩阵
uniform mat4 view;                  // 视图矩阵
uniform mat4 projection;            // 投影矩阵

void main()
{
    gl_Position = projection * view * vec4(FragPos, 1.0);
    
    // gouraud shading
    // --------------------------------------------------
    vec3 Position = vec3(model * vec4(aPos, 1.0));  // 世界空间顶点位置
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal; // 世界空间法向量
    
    // 环境光照(ambient)
    float ambientStrength = 0.1;  // 常量环境因子
    vec3 ambient = ambientStrength * lightColor; // 乘以光的颜色得到环境光照分量
    
    // 漫反射光照(diffuse)
    vec3 norm = normalize(Normal); // 标准化法向量
    vec3 lightDir = normalize(lightPos - FragPos); // 获取指向光源的向量
    float diff = max(dot(norm, lightDir), 0.0); // 进行点乘计算光源对当前片段实际的漫发射影响
    vec3 diffuse = diff * lightColor; // 乘以光的颜色得到漫反射分量
    
    // 镜面反射光照(specular)
    float specularStrength = 0.5; // 镜面强度(中等亮度颜色)
    vec3 viewDir = normalize(viewPos - FragPos); // 计算视线方向向量(指向眼睛)
    vec3 reflectDir = reflect(-lightDir, norm); // 计算沿着法线轴的反射向量
    int shininess = 32; // 高光的反光度(Shininess): 物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); // 计算反光度
    vec3 specular = specularStrength * spec * lightColor; // 计算镜面分量
    
    // 合并反射颜色
    LightingColor = ambient + diffuse + specular;
}
