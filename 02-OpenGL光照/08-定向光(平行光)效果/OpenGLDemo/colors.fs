#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;        // 纹理坐标
out vec4 FragColor;       // 输出颜色

uniform vec3 viewPos;     // 观察者位置（相机位置）

// 光照结构体
struct Light {
    // vec3 position; // 定向光不需要位置
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

// 材质结构体
struct Material {
    sampler2D  diffuse;   // 漫反射光照分量（环境光分量几乎所有情况下都等于漫反射颜色）
    sampler2D  specular;  // 镜面光照分量（纹理为黑白色，我们只关心强度，越白越强）
    float      shininess; // 反光度分量（影响镜面高光的散射/半径）
};
uniform Material material;

// 片段着色器里的计算都是在世界空间坐标中进行的
void main()
{
    // 环境光照(ambient)
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // 漫反射光照(diffuse)
    vec3 norm = normalize(Normal); // 标准化法向量
    vec3 lightDir = normalize(-light.direction); // 获取指向光源的向量
    float diff = max(dot(norm, lightDir), 0.0); // 进行点乘计算光源对当前片段实际的漫发射影响
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // 镜面反射光照(specular)
    vec3 viewDir = normalize(viewPos - FragPos); // 计算视线方向向量(指向眼睛)
    vec3 reflectDir = reflect(-lightDir, norm); // 计算沿着法线轴的反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 计算反光度
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
    // 合并反射颜色
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
