#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace easy2d {

// ============================================================================
// OpenGL Shader 程序
// ============================================================================
class GLShader {
public:
    GLShader();
    ~GLShader();

    // 从源码编译
    bool compileFromSource(const char* vertexSource, const char* fragmentSource);
    
    // 从文件加载并编译
    bool compileFromFile(const std::string& vertexPath, const std::string& fragmentPath);

    // 使用/激活
    void bind() const;
    void unbind() const;

    // Uniform 设置
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat4(const std::string& name, const glm::mat4& value);

    // 获取程序 ID
    GLuint getProgramID() const { return programID_; }

    // 检查是否有效
    bool isValid() const { return programID_ != 0; }

private:
    GLuint programID_;
    std::unordered_map<std::string, GLint> uniformCache_;

    GLuint compileShader(GLenum type, const char* source);
    GLint getUniformLocation(const std::string& name);
};

} // namespace easy2d
