#include <easy2d/graphics/opengl/gl_shader.h>
#include <easy2d/utils/logger.h>
#include <fstream>
#include <sstream>

namespace easy2d {

GLShader::GLShader() : programID_(0) {
}

GLShader::~GLShader() {
    if (programID_ != 0) {
        glDeleteProgram(programID_);
    }
}

bool GLShader::compileFromSource(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) return false;

    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }

    programID_ = glCreateProgram();
    glAttachShader(programID_, vertexShader);
    glAttachShader(programID_, fragmentShader);
    glLinkProgram(programID_);

    GLint success;
    glGetProgramiv(programID_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(programID_, 512, nullptr, infoLog);
        E2D_LOG_ERROR("Shader program linking failed: {}", infoLog);
        glDeleteProgram(programID_);
        programID_ = 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return success == GL_TRUE;
}

bool GLShader::compileFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::ifstream vShaderFile(vertexPath);
    std::ifstream fShaderFile(fragmentPath);

    if (!vShaderFile.is_open() || !fShaderFile.is_open()) {
        E2D_LOG_ERROR("Failed to open shader files: {}, {}", vertexPath, fragmentPath);
        return false;
    }

    std::stringstream vShaderStream, fShaderStream;
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    return compileFromSource(vShaderStream.str().c_str(), fShaderStream.str().c_str());
}

void GLShader::bind() const {
    glUseProgram(programID_);
}

void GLShader::unbind() const {
    glUseProgram(0);
}

void GLShader::setBool(const std::string& name, bool value) {
    glUniform1i(getUniformLocation(name), value ? 1 : 0);
}

void GLShader::setInt(const std::string& name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

void GLShader::setFloat(const std::string& name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

void GLShader::setVec2(const std::string& name, const glm::vec2& value) {
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void GLShader::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void GLShader::setVec4(const std::string& name, const glm::vec4& value) {
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void GLShader::setMat4(const std::string& name, const glm::mat4& value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

GLuint GLShader::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        E2D_LOG_ERROR("Shader compilation failed: {}", infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLint GLShader::getUniformLocation(const std::string& name) {
    auto it = uniformCache_.find(name);
    if (it != uniformCache_.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(programID_, name.c_str());
    uniformCache_[name] = location;
    return location;
}

} // namespace easy2d
