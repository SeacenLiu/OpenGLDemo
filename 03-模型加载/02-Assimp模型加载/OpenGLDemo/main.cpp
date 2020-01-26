//
//  main.cpp
//  OpenGLDemo
//
//  Created by SeacenLiu on 2020/1/6.
//  Copyright © 2020 SeacenLiu. All rights reserved.
//

/**
 * Assimp 网格
 * https://learnopengl-cn.github.io/03%20Model%20Loading/02%20Mesh/
 */

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

#include "model.h"

// 回调函数定义
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// 配置
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 相机
Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 计时
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(int argc, const char * argv[]) {
    // --------------- 初始化 GLFW ---------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // --------------- 创建窗口 ---------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          "Learn OpenGL",
                                          NULL,
                                          NULL);
    if (window == NULL) {
        std::cout << "创建窗口失败" << std::endl;
        glfwTerminate();
        return -1;
    }
    // 配置上下文环境
    glfwMakeContextCurrent(window);
    // 配置帧缓存大小变化回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // 配置鼠标事件回调
    glfwSetCursorPosCallback(window, mouse_callback);
    // 配置滚轮事件回调
    glfwSetScrollCallback(window, scroll_callback);
    // 隐藏鼠标光标展示
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // --------------- 初始化 GLAD ---------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "初始化 GLAD 拓展失败" << std::endl;
        return -1;
    }
    
    // --------------- 配置 OpenGL 全局状态 ---------------
    // 打开深度测试功能
    glEnable(GL_DEPTH_TEST);
    
    // --------------- 加载着色器程序 ---------------
    
    
    return 0;
}

// 处理用户键盘操作
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// 处理窗口变化事件（系统或用户所为）
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 处理鼠标移动操作
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // 因为坐标系不同，所有需要颠倒y值

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// 处理鼠标滚轮事件
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
