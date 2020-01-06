//
//  main.cpp
//  HelloRectange
//
//  Created by SeacenLiu on 2019/12/8.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 * https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/#_10
 *
 * 自己的理解：
 * VBO: 顶点缓冲对象
 *      - 顶点信息，将顶点信息保存在显存里面
 *
 * VAO: 顶点数组对象
 *      - 顶点缓冲对象的属性信息，保存顶点属性调用
 *      - 一般存储的内容
 *          - glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
 *          - 通过glVertexAttribPointer设置的顶点属性配置。
 *          - 通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象。
 *
 * EBO: 索引缓冲对象
 *      - 顶点绘制顺序信息，存放顶点的绘制顺序索引
 *
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shaderhelp.h"
#include "glhelp.h"

// 窗口的宽高
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点数组
float vertices[] = {
     0.5f, 0.5f, 0.0f,   // 右上角
     0.5f, -0.5f, 0.0f,  // 右下角
    -0.5f, -0.5f, 0.0f,  // 左下角
    -0.5f, 0.5f, 0.0f    // 左上角
};

// 索引数组
unsigned int indices[] = {
    0, 1, 3, // 第一个三角形
    1, 2, 3  // 第二个三角形
};

// 顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// 片元着色器
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// 帧缓冲大小改变回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 用户输入处理
void processInput(GLFWwindow *window) {
    // 用户点击 ESC 键监听
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, const char * argv[]) {
    // --- 初始化 GLFW ---
    glfwInitialize();
    
    // --- 创建窗口 ---
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          "Hello Rectange",
                                          NULL,
                                          NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // --- 初始化 GLAD ---
    if (!gladInitialize()) {
       std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // --- 配置顶点着色器 ---
    unsigned int vertexShader;
    // 创建顶点着色器
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 加载着色器源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // 编译着色器
    glCompileShader(vertexShader);
    // 检验编译结果
    CheackCompileShader(vertexShader);
    
    // --- 配置顶点着色器 ---
    unsigned int fragmentShader;
    // 创建顶点着色器
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // 加载着色器源码
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // 编译着色器
    glCompileShader(fragmentShader);
    // 检验编译结果
    CheackCompileShader(fragmentShader);
    
    // --- 配置着色器程序 ---
    unsigned int shaderProgram;
    // 创建着色器程序
    shaderProgram = glCreateProgram();
    // 附加顶点着色器到着色器程序
    glAttachShader(shaderProgram, vertexShader);
    // 附加片元着色器到着色器程序
    glAttachShader(shaderProgram, fragmentShader);
    // 连接程序
    glLinkProgram(shaderProgram);
    // 检测链接着色器程序结果
    CheckLinkShaderProgram(shaderProgram);
    // 把着色器对象链接到程序对象以后就不需要前面的着色器对象了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    
    // --- VBO、VAO、EBO 配置 ---
    unsigned int VBO, VAO, EBO;
    // VBO 配置
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertices),
                 vertices,
                 GL_STATIC_DRAW);
    // VAO 配置
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // EBO 配置
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(indices),
                 indices,
                 GL_STATIC_DRAW);
    // 使用后解绑，防止误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // 绘制模式切换
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 用线绘制
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
    
    // 一个简单的渲染循环（每一帧调用一次）
    while (!glfwWindowShouldClose(window)) {
        // 1. 处理用户输入
        processInput(window);
        
        // 2. 渲染指令
        // 2-1: 设置清空屏幕所用的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 3. 绘制三角形
        // 3-1: 使用编译好的周色漆程序
        glUseProgram(shaderProgram);
        // 3-2: 绑定 VAO
        glBindVertexArray(VAO);
        // 3-3: 绘制操作
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // 4. 检查并调用事件，交换缓冲
        glfwSwapBuffers(window);
        // 5. 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
        glfwPollEvents();
    }
    
    // 释放/删除之前的分配的所有资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    
    return 0;
}
