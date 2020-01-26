//
//  camera.h
//  OpenGLDemo
//
//  Created by SeacenLiu on 2020/1/6.
//  Copyright © 2020 SeacenLiu. All rights reserved.
//

#ifndef camera_h
#define camera_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 相机移动的方向定义（避免与窗口系统的方法冲突）
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// 相机的默认值
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

// 一个OpenGL的摄像机类，处理输入并计算相应的欧拉角、向量和矩阵
class Camera {
public:
    // 相机属性
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // 欧拉角
    float Yaw;
    float Pitch;
    // 相机交互配置
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // 使用向量初始化的构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW, float pitch = PITCH) :
           Front(glm::vec3(0.0f, 0.0f, -1.0f)),
           MovementSpeed(SPEED),
           MouseSensitivity(SENSITIVITY),
           Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    
    // 使用标量值初始化的构造函数
    Camera(float posX, float posY, float posZ,
           float upX, float upY, float upZ,
           float yaw, float pitch) :
           Front(glm::vec3(0.0f, 0.0f, -1.0f)),
           MovementSpeed(SPEED),
           MouseSensitivity(SENSITIVITY),
           Zoom(ZOOM) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    
    // 获取视图矩阵（使用欧拉角和LookAt矩阵计算）
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // 处理键盘输入事件
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // 处理鼠标输入事件
    void ProcessMouseMovement(float xoffset,
                              float yoffset,
                              GLboolean constrainPitch = true) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // 边界保护，避免场景翻转
        if (constrainPitch) {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        
        // 通过更新欧拉角更新前、右和上向量
        updateCameraVectors();
    }
    
    // 处理鼠标滚轮事件（只支持垂直滚动事件）
    void ProcessMouseScroll(float yoffset) {
        if (Zoom >= 1.0f && Zoom <= 45.0f)
            Zoom -= yoffset;
        if (Zoom <= 1.0f)
            Zoom = 1.0f;
        if (Zoom >= 45.0f)
            Zoom = 45.0f;
    }

private:
    // 通过更新相机的欧拉角计算前、右和上向量
    void updateCameraVectors()
    {
        // 计算新的前向量
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // 重新计算右、上向量
        // (需要将向量归一化，因为你向上或向下看的次数越多，它们的长度就越接近0，导致运动变慢)
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif /* camera_h */
