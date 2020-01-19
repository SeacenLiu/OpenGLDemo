//
//  glhelp.h
//  OpenGLDemo
//
//  Created by SeacenLiu on 2019/12/8.
//  Copyright © 2019 SeacenLiu. All rights reserved.
//

#ifndef glhelp_h
#define glhelp_h

bool glfwInitialize() {
    glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    return true;
}

bool gladInitialize() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    } else {
        return true;
    }
}

#endif /* glhelp_h */
