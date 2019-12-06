//
//  main.cpp
//  HelloWindow
//
//  Created by SeacenLiu on 2019/12/6.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 https://learnopengl-cn.github.io/01%20Getting%20started/03%20Hello%20Window/
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 窗口回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理用户输入函数
void processInput(GLFWwindow *window);

int main(int argc, const char * argv[]) {
    // 配置 GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    // 初始化 GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    // 设置窗口的维度，将(-1到1)范围内的坐标映射到(0, 800)和(0, 600)
    glViewport(0, 0, 800, 600);
    
    // 注册窗口改变回调函数（当窗口被第一次显示的时候framebuffer_size_callback也会被调用）
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    // 一个简单的渲染循环（每一帧调用一次）
    while (!glfwWindowShouldClose(window)) {
        // 处理用户输入
        processInput(window);
        
        // 渲染指令
        // （状态设置函数）设置清空屏幕所用的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // （状态使用的函数）清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        // 检查并调用事件，交换缓冲
        // 交换颜色缓冲（一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上
        glfwSwapBuffers(window);
        // 检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
        glfwPollEvents();
    }
    
    // 确释放/删除之前的分配的所有资源
    glfwTerminate();
    
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    // 用户点击 ESC 键监听
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
