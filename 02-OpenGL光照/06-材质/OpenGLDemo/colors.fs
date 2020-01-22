#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;       // 输出颜色

uniform vec3 viewPos;     // 观察者位置（相机位置）

// 光照结构体
struct Light {
    vec3 position;

    vec3 ambient;   // 环境光强度向量 * 环境光颜色向量
    vec3 diffuse;   // 漫反射光强度向量 * 漫反射光颜色向量
    vec3 specular;  // 镜面反射强度向量 * 反光的颜色向量
};
uniform Light light;

// 材质结构体
struct Material {
    vec3  ambient;   // 环境光照分量（物体颜色）
    vec3  diffuse;   // 漫反射光照分量（物体颜色）
    vec3  specular;  // 镜面光照分量
    float shininess; // 反光度分量（影响镜面高光的散射/半径）
};
uniform Material material;

// 片段着色器里的计算都是在世界空间坐标中进行的
void main()
{
    // 环境光照(ambient)
    vec3 ambient = light.ambient * material.ambient; // 乘以光的颜色得到环境光照分量
    
    // 漫反射光照(diffuse)
    vec3 norm = normalize(Normal); // 标准化法向量
    vec3 lightDir = normalize(light.position - FragPos); // 获取指向光源的向量
    float diff = max(dot(norm, lightDir), 0.0); // 进行点乘计算光源对当前片段实际的漫发射影响
    vec3 diffuse = light.diffuse * (diff * material.diffuse); // 乘以光的颜色得到漫反射分量
    
    // 镜面反射光照(specular)
    vec3 viewDir = normalize(viewPos - FragPos); // 计算视线方向向量(指向眼睛)
    vec3 reflectDir = reflect(-lightDir, norm); // 计算沿着法线轴的反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 计算反光度
    vec3 specular = light.specular * (spec * material.specular); // 乘以光的颜色得到镜面反射光照分量
    
    // 合并反射颜色
//    vec3 result = (ambient + diffuse + specular) * objectColor;
//    FragColor = vec4(result, 1.0);
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
