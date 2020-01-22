#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;        // 纹理坐标
out vec4 FragColor;       // 输出颜色

uniform vec3 viewPos;     // 观察者位置（相机位置）

// 定向光光源结构体
struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

// 点光源结构体
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

// 聚光光源结构体
struct SpotLight {
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
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

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
    // 属性
    // 标准化法向量
    vec3 norm = normalize(Normal);
    // 计算视线方向向量(指向眼睛)
    vec3 viewDir = normalize(viewPos - FragPos);

    // 第一阶段：定向光照
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    // 第三阶段：聚光
    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}

// 定向光光照计算
// light: 定向光光源
// normal: 平面法向量
// viewDir: 视线方向向量
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    // 光照方向（光照结构体中的反方向，对其进行标椎化）（light.direction：是从中心指向外面的）
    vec3 lightDir = normalize(-light.direction);
    // 漫反射着色（法向量与方向向量点乘：|normal|*|lightDir|*cos）
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色（1.计算反射向量. 2.视线向量点乘反射向量，再进行shininess立方计算，计算相应程度的效果）
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

// 点光源光照计算
// light: 点光源
// normal: 平面法向量
// fragPos: 着色位置
// viewDir: 视线方向向量
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 光照方向（光源位置 - 着色位置 = 着色位置指向光源的向量）
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色（法向量与光照向量点乘：|normal|*|lightDir|*cos）
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色（1.计算反射向量. 2.视线向量点乘反射向量，再进行shininess立方计算，计算相应程度的效果）
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 衰减
    // 距离：光源距离着色位置的距离大小
    float distance    = length(light.position - fragPos);
    // 衰弱公式运用
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                 light.quadratic * (distance * distance));
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// 聚光光照计算
// spotLight: 聚光光源
// normal: 平面法向量
// fragPos: 着色位置
// viewDir: 视线方向向量
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // 获取指向光源的向量
    vec3 lightDir = normalize(light.position - FragPos);
    // lightDir 与 -light.direction 的夹角余弦值
    float theta = dot(lightDir, normalize(-light.direction));
    // 外圆锥与内圆锥夹角之差的余弦值
    float epsilon = light.cutOff - light.outerCutOff;
    // 计算平滑过渡的强度
    // clamp函数把第一个参数约束在了0.0到1.0之间，保证强度值不会在[0, 1]区间之外
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // 环境光照(ambient)
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    
    // 漫反射光照(diffuse)
    vec3 norm = normalize(Normal); // 标准化法向量
    float diff = max(dot(norm, lightDir), 0.0); // 进行点乘计算光源对当前片段实际的漫发射影响
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
    
    // 镜面反射光照(specular)
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
    return (ambient + diffuse + specular);
}
