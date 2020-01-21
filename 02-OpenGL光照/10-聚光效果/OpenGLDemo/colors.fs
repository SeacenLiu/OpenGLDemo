#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;        // 纹理坐标
out vec4 FragColor;       // 输出颜色

uniform vec3 viewPos;     // 观察者位置（相机位置）

// 光照结构体
struct Light {
    vec3 position;
    vec3  direction;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
    float outerCutOff;
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
    vec3 lightDir = normalize(light.position - FragPos); // 获取指向光源的向量
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    // clamp函数把第一个参数约束在了0.0到1.0之间，保证强度值不会在[0, 1]区间之外
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 环境光照(ambient)
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // 漫反射光照(diffuse)
    vec3 norm = normalize(Normal); // 标准化法向量
    float diff = max(dot(norm, lightDir), 0.0); // 进行点乘计算光源对当前片段实际的漫发射影响
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // 镜面反射光照(specular)
    vec3 viewDir = normalize(viewPos - FragPos); // 计算视线方向向量(指向眼睛)
    vec3 reflectDir = reflect(-lightDir, norm); // 计算沿着法线轴的反射向量
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 计算反光度
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
    
    // 光照衰减公式
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                               light.quadratic * (distance * distance));
    // 从周围环境中去除衰减，否则在很远的距离内，由于周围环境的因素，聚光灯内部的光线会比外部的光线暗
    // ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    // 将不对环境光做出影响，让它总是能有一点光
    diffuse  *= intensity;
    specular *= intensity;
    
    // 合并反射颜色
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

/** 僵硬的聚光效果
// 片段着色器里的计算都是在世界空间坐标中进行的
void main()
{
    vec3 lightDir = normalize(light.position - FragPos); // 获取指向光源的向量
    float theta = dot(lightDir, normalize(-light.direction));
    if(theta > light.cutOff)
    {
        // 执行光照计算
        // 环境光照(ambient)
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
        
        // 漫反射光照(diffuse)
        vec3 norm = normalize(Normal); // 标准化法向量
        float diff = max(dot(norm, lightDir), 0.0); // 进行点乘计算光源对当前片段实际的漫发射影响
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
        
        // 镜面反射光照(specular)
        vec3 viewDir = normalize(viewPos - FragPos); // 计算视线方向向量(指向眼睛)
        vec3 reflectDir = reflect(-lightDir, norm); // 计算沿着法线轴的反射向量
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 计算反光度
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;
        
        // 光照衰减公式
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance +
                                   light.quadratic * (distance * distance));
        // 从周围环境中去除衰减，否则在很远的距离内，由于周围环境的因素，聚光灯内部的光线会比外部的光线暗
        // ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        
        // 合并反射颜色
        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else
    {
        // 否则，使用环境光，让场景在聚光之外时不至于完全黑暗
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
    }
}
*/
