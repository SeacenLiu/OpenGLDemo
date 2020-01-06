#version 330 core
out vec4 FragColor;       // 输出颜色
  
uniform vec3 objectColor; // 物体颜色
uniform vec3 lightColor;  // 光照颜色

void main()
{
    // 光照颜色向量 * 物体颜色向量 = 物体反射的颜色向量
    FragColor = vec4(lightColor * objectColor, 1.0);
}
