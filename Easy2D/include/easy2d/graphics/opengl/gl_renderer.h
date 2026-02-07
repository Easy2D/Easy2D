#pragma once

#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/opengl/gl_shader.h>
#include <easy2d/graphics/opengl/gl_sprite_batch.h>
#include <GL/glew.h>

namespace easy2d {

class Window;

// ============================================================================
// OpenGL 渲染器实现
// ============================================================================
class GLRenderer : public RenderBackend {
public:
    GLRenderer();
    ~GLRenderer() override;

    // RenderBackend 接口实现
    bool init(Window* window) override;
    void shutdown() override;

    void beginFrame(const Color& clearColor) override;
    void endFrame() override;
    void setViewport(int x, int y, int width, int height) override;
    void setVSync(bool enabled) override;

    void setBlendMode(BlendMode mode) override;
    void setViewProjection(const glm::mat4& matrix) override;

    Ptr<Texture> createTexture(int width, int height, const uint8_t* pixels, int channels) override;
    Ptr<Texture> loadTexture(const std::string& filepath) override;

    void beginSpriteBatch() override;
    void drawSprite(const Texture& texture, const Rect& destRect, const Rect& srcRect, 
                   const Color& tint, float rotation, const Vec2& anchor) override;
    void drawSprite(const Texture& texture, const Vec2& position, const Color& tint) override;
    void endSpriteBatch() override;

    void drawLine(const Vec2& start, const Vec2& end, const Color& color, float width) override;
    void drawRect(const Rect& rect, const Color& color, float width) override;
    void fillRect(const Rect& rect, const Color& color) override;
    void drawCircle(const Vec2& center, float radius, const Color& color, int segments, float width) override;
    void fillCircle(const Vec2& center, float radius, const Color& color, int segments) override;
    void drawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color, float width) override;
    void fillTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) override;
    void drawPolygon(const std::vector<Vec2>& points, const Color& color, float width) override;
    void fillPolygon(const std::vector<Vec2>& points, const Color& color) override;

    Ptr<FontAtlas> createFontAtlas(const std::string& filepath, int fontSize, bool useSDF = false) override;
    void drawText(const FontAtlas& font, const String& text, const Vec2& position, const Color& color) override;
    void drawText(const FontAtlas& font, const String& text, float x, float y, const Color& color) override;

    Stats getStats() const override { return stats_; }
    void resetStats() override;

private:
    Window* window_;
    GLSpriteBatch spriteBatch_;
    GLShader shapeShader_;
    
    GLuint shapeVao_;
    GLuint shapeVbo_;
    
    glm::mat4 viewProjection_;
    Stats stats_;
    bool vsync_;

    void initShapeRendering();
    void setupBlendMode(BlendMode mode);
};

} // namespace easy2d
