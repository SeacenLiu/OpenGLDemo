//
//  model.h
//  OpenGLDemo
//
//  Created by SeacenLiu on 2020/1/25.
//  Copyright © 2020 SeacenLiu. All rights reserved.
//

#ifndef model_h
#define model_h

#include "mesh.h"

// assimp 头文件
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// stb_image 头文件
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model {
public:
    // 构造函数
    Model(char *path) {
        loadModel(path);
    }
    // 绘制函数
    void Draw(Shader shader) {
        for (unsigned int i = 0; i < meshes.size(); ++i)
            meshes[i].Draw(shader);
    }
private:
    // 网格数据
    vector<Mesh> meshes;
    // 已加载的纹理数据
    vector<Texture> textures_loaded;
    // 模型路径
    string directory;
    // 加载模型函数
    void loadModel(string path) {
        // 使用 assimp 读入场景数据
        Assimp::Importer importer;
        // Post-processing(后期处理)
        // - aiProcess_Triangulate: 将模型所有的图元形状变换为三角形
        // - aiProcess_FlipUVs: 处理的时候翻转y轴的纹理坐标
        // - aiProcess_GenNormals: 为每个顶点创建法线
        // - aiProcess_SplitLargeMeshes: 将比较大的网格分割成更小的子网格，用于减少单个网格的顶点数
        // - aiProcess_OptimizeMeshes: 将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化
        const aiScene *scene = importer.ReadFile(path,
                                                 aiProcess_Triangulate | aiProcess_FlipUVs);
        if(!scene                                        // Scene 是否为空
           || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE  // 场景是否加载完毕
           || !scene->mRootNode) {                       // 是否存在根结点
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        // 配置文件路径
        directory = path.substr(0, path.find_last_of('/'));
        // 递归处理结点
        processNode(scene->mRootNode, scene);
        
    }
    // 处理结点
    void processNode(aiNode *node, const aiScene *scene) {
        // 处理节点所有的网格（如果有的话）
        for(unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // 接下来对它的子节点重复这一过程
        for(unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }
    
    // 处理网格
    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        vector<Vertex> vertices;      // 顶点数据
        vector<unsigned int> indices; // 网格索引数据
        vector<Texture> textures;     // 纹理数据

        // 处理顶点
        for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex vertex;
            // 处理顶点位置
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // 处理法线
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
            // 处理纹理坐标
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }
        
        // 处理索引
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }
        
        // 处理材质
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            // 漫反射材质
            vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                               aiTextureType_DIFFUSE,
                                                               "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 镜面光照材质
            vector<Texture> specularMaps = loadMaterialTextures(material,
                                                                aiTextureType_SPECULAR,
                                                                "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }
    
    // 加载材质
    vector<Texture> loadMaterialTextures(aiMaterial *mat,
                                         aiTextureType type,
                                         string typeName) {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            // 防止重复加载相同纹理
            for (unsigned int j = 0; j < textures_loaded.size(); ++j) {
                if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) { // 如果纹理还没有被加载，则加载它
                Texture texture;
                texture.id   = TextureFromFile(str.C_Str(), directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // 添加到已加载的纹理中
            }
        }
        return textures;
    }
};

unsigned int TextureFromFile(const char *path,
                             const string &directory,
                             bool gamma) {
    // 拼接文件路径
    string filename = string(path);
    filename = directory + '/' + filename;
    // 创建纹理对象
    unsigned int textureID;
    glGenTextures(1, &textureID);
    // 加载纹理文件
    int width, height, nrComponents; // 宽度、高度、颜色通道数
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        // 配置存储格式
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        else
            format = GL_RGBA;
        
        // 绑定当前纹理
        glBindTexture(GL_TEXTURE_2D, textureID);
        // 创建2D纹理
        glTexImage2D(GL_TEXTURE_2D,     // 纹理类型
                     0,                 // 多级渐远纹理类型
                     format,            // 纹理存储格式
                     width,             // 纹理宽度
                     height,            // 纹理高度
                     0,                 // 历史遗留参数(0即可)
                     format,            // 源图数据格式
                     GL_UNSIGNED_BYTE,  // 源图数据类型
                     data);             // 源图数据
        // 为纹理对象生成一组完整的 mipmap？
        glGenerateMipmap(GL_TEXTURE_2D);
        // 为当前绑定的纹理对象设置环绕模式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // 为当前绑定的纹理对象设置过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // 释放资源
        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        // 释放资源
        stbi_image_free(data);
    }

    return textureID;
}

#endif /* model_h */
