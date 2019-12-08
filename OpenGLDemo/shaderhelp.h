//
//  ShaderHelp.h
//  OpenGLDemo
//
//  Created by SeacenLiu on 2019/12/8.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

#ifndef ShaderHelp_h
#define ShaderHelp_h

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

#endif /* ShaderHelp_h */
