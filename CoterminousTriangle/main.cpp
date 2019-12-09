//
//  main.cpp
//  CoterminousTriangle
//
//  Created by SeacenLiu on 2019/12/9.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 * 使用glDrawArrays，尝试绘制两个彼此相连的三角形
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
    -0.5f,  0.5f, 0.0f,
    -1.0f, -0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     1.0f, -0.5f, 0.0f,
     0.0f, -0.5f, 0.0f,
};

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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
        // 3. 绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
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
