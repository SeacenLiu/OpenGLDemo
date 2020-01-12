#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;       // 输出颜色
  
uniform vec3 lightPos;    // 光源位置
uniform vec3 lightColor;  // 光照颜色
uniform vec3 objectColor; // 物体颜色
uniform vec3 viewPos;     // 观察者位置（相机位置）

// 片段着色器里的计算都是在世界空间坐标中进行的
void main()
{
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
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    // 合并反射颜色
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
