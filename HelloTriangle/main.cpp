//
//  main.cpp
//  HelloTriangle
//
//  Created by SeacenLiu on 2019/12/6.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

/**
 * https://learnopengl-cn.github.io/01%20Getting%20started/04%20Hello%20Triangle/
 *
 * 顶点数组对象：Vertex Array Object，VAO
 *            - 在GPU内存（通常被称为显存）中储存大量顶点
 *            - 储存内容：
 *              - glEnableVertexAttribArray和glDisableVertexAttribArray的调用。
 *              - 通过glVertexAttribPointer设置的顶点属性配置。
 *              - 通过glVertexAttribPointer调用与顶点属性关联的顶点缓冲对象
 * 顶点缓冲对象：Vertex Buffer Object，VBO
 *            - 存储顶点状态
 * 索引缓冲对象：Element Buffer Object，EBO或Index Buffer Object，IBO
 *
 */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

/** 顶点数组
 * x1, y1, z1,
 * x2, y2, z2,
 * x3, y3, z3,
 * (z1 = z2 = z3 = 0 在同一平面)
 *
 * 数据结构
 *        VERTEX1   VERTEX2  VERTEX3
 *        X  Y  Z   X  Y  Z  X  Y  Z
 * BYTE: 0       12        24       36
 *
 * - 位置数据被储存为32位（4字节）浮点值。
 * - 每个位置包含3个这样的值。
 * - 在这3个值之间没有空隙（或其他值）。这几个值在数组中紧密排列(Tightly Packed)。
 * - 数据中第一个值在缓冲开始的位置。
 */
float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#pragma mark - 顶点着色器
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

#pragma mark - 片元着色器
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

// 窗口回调函数，它会在每次窗口大小被调整的时候被调用
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 处理用户输入函数
void processInput(GLFWwindow *window);
// 检查着色器编译情况
int CheackCompileShader(GLuint shader);
// 检查着色器连接情况
int CheckLinkShaderProgram(GLuint program);

int main(int argc, const char * argv[]) {
    // --- 初始化 GLFW ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // --- 创建窗口 ---
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH,
                                          SCR_HEIGHT,
                                          "Hello Triangle",
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
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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
    
    // --- 配置 VBO & VAO ---
    // 创建 VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    // 绑定顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 创建 VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 绑定 VAO
    glBindVertexArray(VAO);
    // 将定义的顶点对象放进 VBO 中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 指定顶点数据的解析方式
    // 标准化: 所有数据都会被映射到0（对于有符号型signed数据是-1）到1之间
    glVertexAttribPointer(0,                 // 顶点属性 (location = 0)
                          3,                 // 顶点属性的大小，即顶点的维度 (vec3)
                          GL_FLOAT,          // 元素类型 (vec3 由 GL_FALSE 组成)
                          GL_FALSE,          // 是否希望数据被标准化
                          3 * sizeof(float), // 步长，即顶点属性组之间的间隔
                          (void*)0);         // 数据在缓冲中起始位置的偏移量
    // 以顶点属性位置值作为参数，启用顶点属性
    glEnableVertexAttribArray(0);
    // 绑定 VAO
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // 一个简单的渲染循环（每一帧调用一次）
    while (!glfwWindowShouldClose(window)) {
        // 处理用户输入
        processInput(window);
        
        // 渲染指令
        // 设置清空屏幕所用的颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 绘制三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
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

int CheackCompileShader(GLuint shader) {
    int  success = 0;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return success;
}

int CheckLinkShaderProgram(GLuint program) {
    int  success = 0;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::VERTEX::LINK_FAILED\n" << infoLog << std::endl;
    }
    return success;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    // 用户点击 ESC 键监听
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
