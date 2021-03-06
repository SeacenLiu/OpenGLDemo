//
//  main.cpp
//  GreenTriangle
//
//  Created by SeacenLiu on 2019/12/9.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/** 绿色渐变三角形
 * 通过在渲染循环中修改着色器中 uniform 的值，达成"绿->黑->绿"渐变效果
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderhelp.h"
#include "glhelp.h"
#include <cmath>
using namespace std;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor; // 在OpenGL程序代码中设定这个变量\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
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
        cout << "着色器程序创建数百" << endl;
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
        // 3. 绘制渐变三角形
        // 3-1: 重新绑定 VAO
        glBindVertexArray(VAO);
        // 3-2: 使用当前时间计算绿色的色值
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // 使用正弦周期函数进行计算
        // 3-3: 获取 uniform 属性
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        // 3-4: 使用该周色漆程序
        glUseProgram(shaderProgram);
        // 3-5: 向指定 uniform 属性中填充新值
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
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

