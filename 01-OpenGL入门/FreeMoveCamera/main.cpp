//
//  main.cpp
//  FreeMoveCamera
//
//  Created by SeacenLiu on 2019/12/13.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 * 自由移动相机 Demo
 *
 * 修改位置：View 矩阵
 * // lookAt(相机世界坐标位置，目标世界坐标位置， 世界空间的上向量(y轴正方向))
 * glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
 *
 * - 通过 W S A D 自由移动相机位置，即 cameraPos 的位置
 * - 通过鼠标移动，改变目标物体yaw和pitch角度，改变 cameraFront
 * - 通过鼠标滚轮，改变 fov 值缩放视角
 *
 *
 * 注意：
 * - 坐标轴方向灵活获取
 *      - glm::normalize(glm::cross(cameraFront, cameraUp))
 *      - 上面的意思是“z 的负方向向量” 叉乘 “y 的正方向向量” 等于 “x 的正方向向量”
 * - 处理FPS风格摄像机的鼠标输入方向获取步骤
 *      1. 计算鼠标距上一帧的偏移量。
 *      2. 把偏移量添加到摄像机的俯仰角和偏航角中。
 *      3. 对偏航角和俯仰角进行最大和最小值的限制。
 *      4. 计算方向向量。
 *
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "glhelp.h"
#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 上一帧鼠标的X值
float lastX = SCR_WIDTH / 2.0;
// 上一帧鼠标的Y值
float lastY = SCR_HEIGHT / 2.0;
// 是否是第一次移动鼠标
bool firstMouse = false;
float yaw = -90.0f;
float pitch = 0.0f;

float fov = 45.0f;

// 当前帧与上一帧的时间差
float deltaTime = 0.0f;
// 上一帧的时间
float lastFrame = 0.0f;
// 相机位置
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f,  3.0f);
// z 轴负方向向量
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
// y 轴正方向向量
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f,  0.0f);

// 立方体六面的顶点
float vertices[] = {
//     ---- 位置 ----      - 纹理坐标 -
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

// 立方体的方位
glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f),
  glm::vec3( 2.0f,  5.0f, -15.0f),
  glm::vec3(-1.5f, -2.2f, -2.5f),
  glm::vec3(-3.8f, -2.0f, -12.3f),
  glm::vec3( 2.4f, -0.4f, -3.5f),
  glm::vec3(-1.7f,  3.0f, -7.5f),
  glm::vec3( 1.3f, -2.0f, -2.5f),
  glm::vec3( 1.5f,  2.0f, -2.5f),
  glm::vec3( 1.5f,  0.2f, -1.5f),
  glm::vec3(-1.3f,  1.0f, -1.5f)
};

// 鼠标事件回调
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 鼠标滚轮回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main(int argc, const char * argv[]) {
    // --------------- 初始化 GLFW ---------------
    glfwInitialize();
    // --------------- 创建窗口 ---------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          "Free Move Camera",
                                          NULL,
                                          NULL);
    if (window == NULL) {
        std::cout << "创建窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // --------------- 初始化 GLAD ---------------
    if (!gladInitialize()) {
        std::cout << "初始化 GLAD 拓展失败" << std::endl;
        return -1;
    }
    // --------------- 构建着色器程序 ---------------
    Shader ourShader("./shader.vs", "./shader.fs");
    // --------------- 配置 VBO & VAO ---------------
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    // 设置完成后解绑，防止误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // --------------- 配置 2D 纹理 ---------------
    // 创建 2D 纹理
    int width, height, nrChannels;
    unsigned char *data;
    // 箱子纹理
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("./container.jpg",  // 图片路径
                                    &width,        // 图片宽度返回地址
                                    &height,       // 图片高度返回地址
                                    &nrChannels,   // 颜色通道个数返回地址
                                    0);            //
    if (data) {
        glTexImage2D(GL_TEXTURE_2D,     // 纹理类型
                     0,                 // 多级渐远纹理类型
                     GL_RGB,            // 纹理存储格式
                     width,             // 纹理宽度
                     height,            // 纹理高度
                     0,                 // 历史遗留参数(0即可)
                     GL_RGB,            // 源图数据格式
                     GL_UNSIGNED_BYTE,  // 源图数据类型
                     data);             // 源图数据
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "纹理加载失败" << std::endl;
        return -1;
    }
    stbi_image_free(data);
    // 笑脸纹理
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("./awesomeface.png",  // 图片路径
                     &width,        // 图片宽度返回地址
                     &height,       // 图片高度返回地址
                     &nrChannels,   // 颜色通道个数返回地址
                     0);            //
    if (data) {
        glTexImage2D(GL_TEXTURE_2D,     // 纹理类型
                     0,                 // 多级渐远纹理类型
                     GL_RGBA,           // 纹理存储格式
                     width,             // 纹理宽度
                     height,            // 纹理高度
                     0,                 // 历史遗留参数(0即可)
                     GL_RGBA,           // 源图数据格式
                     GL_UNSIGNED_BYTE,  // 源图数据类型
                     data);             // 源图数据
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "纹理加载失败" << std::endl;
        return -1;
    }
    stbi_image_free(data);
    // --------------- 材质设置 ---------------
    ourShader.use();
    // 手动设置
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // 使用着色器类设置
    ourShader.setInt("texture2", 1);
    // 启用深度测试 -> 绘制遮挡效果
    glEnable(GL_DEPTH_TEST);
    
    // 告诉 GLFW 应该隐藏光标
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // 设置鼠标移动回调
    glfwSetCursorPosCallback(window, mouse_callback);
    // 设置鼠标滚轮回调
    glfwSetScrollCallback(window, scroll_callback);
    
    // --------------- 渲染循环 ---------------
    while (!glfwWindowShouldClose(window)) {
        // 1. 处理用户输入
        processInput(window);
        
        // --------------------------------------------------------
        // 2. 背景渲染颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 因为启动了深度测试，还需要在迭代前清除深度缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // --------------------------------------------------------
        
        // 3. 绘制三角形
        // 3-1: 激活并绑定2D纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // 3-2: 激活着色器程序
        ourShader.use();
        
        // 3-3: 绑定 VAO
        glBindVertexArray(VAO);
        
        // 3-4: 绘制顶点数组 - 多个绘制
        // --------------- 矩阵变换 ---------------
        // 视图矩阵 - 相机位置移动变换
        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        
        // 投影矩阵 - 3D 变 2D 变换
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov),
                                      ((float)SCR_WIDTH / (float)SCR_HEIGHT),
                                      0.1f,
                                      100.0f);
        // 设置 uniform 属性
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        
        for(unsigned int i = 0; i < 10; i++) {
            // 模型矩阵 - 模型自身变换 - 自身位移位置 + 自身旋转
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        // 4. 交换缓冲
        glfwSwapBuffers(window);
        // 5. 处理事件
        glfwPollEvents();
    }
    // 释放/删除之前的分配的所有资源
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // 移动速度设置
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 2.5f * deltaTime;
    
    // 移动效果设置
    // W - 上移镜头
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
    // S - 下移镜头
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    // A - 左移镜头
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    // D - 右移镜头
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    
    std::cout << "cameraPos: x:" << cameraPos.x << " y:" << cameraPos.y << " z:" << cameraPos.z << std::endl;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.05;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   -= xoffset;
    pitch -= yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (fov >= 1.0f && fov <= 45.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 45.0f)
        fov = 45.0f;
}
