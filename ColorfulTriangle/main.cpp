//
//  main.cpp
//  ColorfulTriangle
//
//  Created by SeacenLiu on 2019/12/9.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 * 多彩三角形
 * 顶点数组中加入颜色的表示，由于三点的颜色不同，根据距离会产生”片段插值“现象，即以两点为中心，
 * 向中间根据距离来配置色值的百分比，进行混色的操作现象
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderhelp.h"
#include "glhelp.h"
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    // 位置              // 颜色
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos; // 位置变量的属性位置值为 0 \n"
    "layout (location = 1) in vec3 aColor; // 颜色变量的属性位置值为 1\n"
    "out vec3 ourColor; // 为片段着色器指定一个颜色输出 \n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0); // 注意我们如何把一个vec3作为vec4的构造器的参数\n"
    "   ourColor = aColor; // 将ourColor设置为我们从顶点数据那里得到的输入颜色\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0);\n"
    "}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    // --------------- 初始化 GLFW ---------------
    glfwInitialize();
    // --------------- 创建窗口 ---------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          "Two Triangle",
                                          NULL,
                                          NULL);
    if (window == NULL) {
        cout << "创建窗口失败" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // --------------- 初始化 GLAD ---------------
    if (!gladInitialize()) {
        cout << "初始化 GLAD 拓展失败" << endl;
        return -1;
    }
    // --------------- 构建着色器程序 ---------------
    GLuint shaderProgram = CreateShaderProgram(vertexShaderSource,
                                               fragmentShaderSource);
    if (!shaderProgram) {
        cout << "着色器程序创建失败" << endl;
        return -1;
    }
    // --------------- 配置 VBO & VAO ---------------
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 0 是索引
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // 1 是索引
    // 设置完成后解绑，防止误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // --------------- 渲染循环 ---------------
    while (!glfwWindowShouldClose(window)) {
        // 1. 处理用户输入
        processInput(window);
        // 2. 背景渲染颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 3. 绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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
