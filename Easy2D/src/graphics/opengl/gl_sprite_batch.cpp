#include <easy2d/graphics/opengl/gl_sprite_batch.h>
#include <easy2d/utils/logger.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

namespace easy2d {

// 顶点着色器
static const char* SPRITE_VERTEX_SHADER = R"(
#version 330 core
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;

uniform mat4 uViewProjection;

out vec2 vTexCoord;
out vec4 vColor;

void main() {
    gl_Position = uViewProjection * vec4(aPosition, 0.0, 1.0);
    vTexCoord = aTexCoord;
    vColor = aColor;
}
)";

// 片段着色器
static const char* SPRITE_FRAGMENT_SHADER = R"(
#version 330 core
in vec2 vTexCoord;
in vec4 vColor;

uniform sampler2D uTexture;
uniform int uUseSDF;
uniform float uSdfOnEdge;
uniform float uSdfScale;

out vec4 fragColor;

void main() {
    if (uUseSDF == 1) {
        float dist = texture(uTexture, vTexCoord).r;
        float sd = (dist - uSdfOnEdge) * uSdfScale;
        float w = fwidth(sd);
        float alpha = smoothstep(-w, w, sd);
        fragColor = vec4(vColor.rgb, vColor.a * alpha);
    } else {
        fragColor = texture(uTexture, vTexCoord) * vColor;
    }
}
)";

GLSpriteBatch::GLSpriteBatch()
    : vao_(0), vbo_(0), ibo_(0), currentTexture_(nullptr), currentIsSDF_(false), drawCallCount_(0), spriteCount_(0) {
    vertices_.reserve(MAX_SPRITES * VERTICES_PER_SPRITE);
    indices_.reserve(MAX_SPRITES * INDICES_PER_SPRITE);
}

GLSpriteBatch::~GLSpriteBatch() {
    shutdown();
}

bool GLSpriteBatch::init() {
    // 创建并编译着色器
    if (!shader_.compileFromSource(SPRITE_VERTEX_SHADER, SPRITE_FRAGMENT_SHADER)) {
        E2D_LOG_ERROR("Failed to compile sprite batch shader");
        return false;
    }

    // 生成 VAO、VBO、IBO
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_);

    // 设置 VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, MAX_SPRITES * VERTICES_PER_SPRITE * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // 设置顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // 生成索引缓冲区
    std::vector<GLuint> indices;
    indices.reserve(MAX_SPRITES * INDICES_PER_SPRITE);
    for (size_t i = 0; i < MAX_SPRITES; ++i) {
        GLuint base = static_cast<GLuint>(i * VERTICES_PER_SPRITE);
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 0);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    return true;
}

void GLSpriteBatch::shutdown() {
    if (vao_ != 0) {
        glDeleteVertexArrays(1, &vao_);
        vao_ = 0;
    }
    if (vbo_ != 0) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }
    if (ibo_ != 0) {
        glDeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }
}

void GLSpriteBatch::begin(const glm::mat4& viewProjection) {
    viewProjection_ = viewProjection;
    vertices_.clear();
    currentTexture_ = nullptr;
    currentIsSDF_ = false;
    drawCallCount_ = 0;
    spriteCount_ = 0;
}

void GLSpriteBatch::draw(const Texture& texture, const SpriteData& data) {
    // 如果纹理改变或缓冲区已满，先 flush
    if (currentTexture_ != nullptr && (currentTexture_ != &texture || currentIsSDF_ != data.isSDF || vertices_.size() >= MAX_SPRITES * VERTICES_PER_SPRITE)) {
        flush();
    }

    currentTexture_ = &texture;
    currentIsSDF_ = data.isSDF;

    // 计算变换后的顶点位置
    glm::vec2 anchorOffset(data.size.x * data.anchor.x, data.size.y * data.anchor.y);
    
    float cosR = cosf(data.rotation);
    float sinR = sinf(data.rotation);
    
    auto transform = [&](float x, float y) -> glm::vec2 {
        float rx = x - anchorOffset.x;
        float ry = y - anchorOffset.y;
        return glm::vec2(
            data.position.x + rx * cosR - ry * sinR,
            data.position.y + rx * sinR + ry * cosR
        );
    };

    glm::vec4 color(data.color.r, data.color.g, data.color.b, data.color.a);

    // 添加四个顶点（图片已在加载时翻转，纹理坐标直接使用）
    // v0(左上) -- v1(右上)
    //   |           |
    // v3(左下) -- v2(右下)
    Vertex v0{ transform(0, 0), glm::vec2(data.texCoordMin.x, data.texCoordMin.y), color };
    Vertex v1{ transform(data.size.x, 0), glm::vec2(data.texCoordMax.x, data.texCoordMin.y), color };
    Vertex v2{ transform(data.size.x, data.size.y), glm::vec2(data.texCoordMax.x, data.texCoordMax.y), color };
    Vertex v3{ transform(0, data.size.y), glm::vec2(data.texCoordMin.x, data.texCoordMax.y), color };

    vertices_.push_back(v0);
    vertices_.push_back(v1);
    vertices_.push_back(v2);
    vertices_.push_back(v3);

    spriteCount_++;
}

void GLSpriteBatch::end() {
    if (!vertices_.empty()) {
        flush();
    }
}

void GLSpriteBatch::flush() {
    if (vertices_.empty() || currentTexture_ == nullptr) return;

    // 绑定纹理
    GLuint texID = static_cast<GLuint>(reinterpret_cast<uintptr_t>(currentTexture_->getNativeHandle()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    // 使用着色器
    shader_.bind();
    shader_.setMat4("uViewProjection", viewProjection_);
    shader_.setInt("uTexture", 0);
    shader_.setInt("uUseSDF", currentIsSDF_ ? 1 : 0);
    shader_.setFloat("uSdfOnEdge", 128.0f / 255.0f);
    shader_.setFloat("uSdfScale", 255.0f / 64.0f);

    // 更新 VBO 数据
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(Vertex), vertices_.data());

    // 绘制
    glBindVertexArray(vao_);
    GLsizei indexCount = static_cast<GLsizei>(vertices_.size() / VERTICES_PER_SPRITE * INDICES_PER_SPRITE);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    drawCallCount_++;
    vertices_.clear();
}

} // namespace easy2d
