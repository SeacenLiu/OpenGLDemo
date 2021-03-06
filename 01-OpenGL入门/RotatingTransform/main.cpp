//
//  main.cpp
//  RotatingTransform
//
//  Created by SeacenLiu on 2019/12/12.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 * 使用矩阵进行位移，并根据时间进行旋转
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "glhelp.h"
#include <iostream>
// GLM include
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
    0, 1, 3, // 右下三角形
    1, 2, 3  // 左上三角形
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main(int argc, const char * argv[]) {
    // --------------- 初始化 GLFW ---------------
    glfwInitialize();
    // --------------- 创建窗口 ---------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          "Two Transform",
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
    // --------------- 配置 VBO & VAO & EBO ---------------
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 设置完成后解绑，防止误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    // --------------- 渲染循环 ---------------
    while (!glfwWindowShouldClose(window)) {
        // 1. 处理用户输入
        processInput(window);
        // 2. 背景渲染颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 3. 绘制三角形
        // 3-1: 激活并绑定2D纹理
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        // 3-2: 激活着色器程序
        ourShader.use();
        
        // -------------------------------------------------------
        /** 3-3: 矩阵位移+旋转
         * 1. 先位移后旋转 - 以(0.5,-0.5)为圆心旋转
         * 2. 先旋转后位移 - 以(0,0)为圆心旋转
         *
         * 矩阵变换建议: 1.缩放 2. 旋转 3. 位移 (具体看需求)
         */
        glm::mat4 trans;
        /** 1. 先位移后旋转
         * trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
         * trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0,0,1));
         */
        // 2. 先旋转后位移
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0,0,1));
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        // -------------------------------------------------------
        
        // 3-4: 绑定 VAO
        glBindVertexArray(VAO);
        // 3-5: 绑定 EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // 3-6: 绘制顶点索引
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
}
