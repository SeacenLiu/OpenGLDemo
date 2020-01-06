//
//  ShaderHelp.h
//  OpenGLDemo
//
//  Created by SeacenLiu on 2019/12/8.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

#ifndef ShaderHelp_h
#define ShaderHelp_h

GLuint CreateShaderProgram(const char * vertexShaderSource,
                           const char * fragmentShaderSource) {
    int  success = 0;
    char infoLog[512];
    // 编译顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    // 编译片元着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }
    // 创建着色器程序
    GLuint shaderProgram = glCreateProgram();
    // 附加顶点着色器到着色器程序
    glAttachShader(shaderProgram, vertexShader);
    // 附加片元着色器到着色器程序
    glAttachShader(shaderProgram, fragmentShader);
    // 连接程序
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADERPROGRAM::VERTEX::LINK_FAILED\n" << infoLog << std::endl;
    }
    // 后续处理
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

// （准备弃用）检查编译着色器情况
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

// 检查连接着色器程序情况
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

#endif /* ShaderHelp_h */
