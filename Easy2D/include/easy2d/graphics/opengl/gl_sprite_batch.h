#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/color.h>
#include <easy2d/core/math_types.h>
#include <easy2d/graphics/texture.h>
#include <easy2d/graphics/opengl/gl_shader.h>
#include <glm/mat4x4.hpp>
#include <vector>

namespace easy2d {

// ============================================================================
// OpenGL 精灵批渲染器
// ============================================================================
class GLSpriteBatch {
public:
    static constexpr size_t MAX_SPRITES = 10000;
    static constexpr size_t VERTICES_PER_SPRITE = 4;
    static constexpr size_t INDICES_PER_SPRITE = 6;

    struct Vertex {
        glm::vec2 position;
        glm::vec2 texCoord;
        glm::vec4 color;
    };

    struct SpriteData {
        glm::vec2 position;
        glm::vec2 size;
        glm::vec2 texCoordMin;
        glm::vec2 texCoordMax;
        glm::vec4 color;
        float rotation;
        glm::vec2 anchor;
        bool isSDF = false;
    };

    GLSpriteBatch();
    ~GLSpriteBatch();

    bool init();
    void shutdown();

    void begin(const glm::mat4& viewProjection);
    void draw(const Texture& texture, const SpriteData& data);
    void end();

    // 统计
    uint32_t getDrawCallCount() const { return drawCallCount_; }
    uint32_t getSpriteCount() const { return spriteCount_; }

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ibo_;
    GLShader shader_;
    
    std::vector<Vertex> vertices_;
    std::vector<GLuint> indices_;
    
    const Texture* currentTexture_;
    bool currentIsSDF_;
    glm::mat4 viewProjection_;
    
    uint32_t drawCallCount_;
    uint32_t spriteCount_;

    void flush();
    void setupShader();
};

} // namespace easy2d
