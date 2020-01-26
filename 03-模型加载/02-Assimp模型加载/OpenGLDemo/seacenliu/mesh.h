//
//  mesh.h
//  OpenGLDemo
//
//  Created by SeacenLiu on 2020/1/25.
//  Copyright © 2020 SeacenLiu. All rights reserved.
//

#ifndef mesh_h
#define mesh_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
using namespace std;

#include <assimp/types.h>

#include "shader.h"

// 顶点数据
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

// 纹理数据
struct Texture {
    unsigned int id;
    /** 类型命名标准
     * 漫反射纹理: texture_diffuseN
     * 镜面光纹理: texture_specularN
    */
    string type;
    aiString path;  // 我们储存纹理的路径用于与其它纹理进行比较
};

// 网格
class Mesh {
public:
    // 顶点数据
    vector<Vertex> vertices;
    // 索引数据
    vector<unsigned int> indices;
    // 纹理数据
    vector<Texture> textures;
    
    // 构造函数
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        setupMesh();
    }
    // 绘制函数
    void Draw(Shader shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            // 在绑定之前激活相应的纹理单元
            glActiveTexture(GL_TEXTURE0 + i);
            // 获取纹理序号（diffuse_textureN 中的 N）
            string number;
            string name = textures[i].type;
            if(name == "texture_diffuse")
                number = std::to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = std::to_string(specularNr++);

            shader.setFloat(("material." + name + number).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // 绘制网格
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
private:
    // 渲染数据
    unsigned int VAO, VBO, EBO;
    // 配置网格数据
    void setupMesh() {
        // 创建 VBO、VAO、EBO
        glGenBuffers(1, &VBO);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &EBO);
        // 配置 VBO、VAO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        // 配置 EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     &indices[0], GL_STATIC_DRAW);
        
        // 顶点位置
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // 顶点法线
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // 顶点纹理坐标
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        
        // 解绑
        glBindVertexArray(0);
    }
};

#endif /* mesh_h */
