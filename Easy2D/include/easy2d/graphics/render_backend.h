#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/color.h>
#include <easy2d/core/math_types.h>
#include <easy2d/core/string.h>
#include <glm/mat4x4.hpp>

namespace easy2d {

// 前向声明
class Window;
class Texture;
class FontAtlas;
class Shader;

// ============================================================================
// 渲染后端类型
// ============================================================================
enum class BackendType {
    OpenGL,
    // Vulkan,
    // Metal,
    // D3D11,
    // D3D12
};

// ============================================================================
// 混合模式
// ============================================================================
enum class BlendMode {
    None,       // 不混合
    Alpha,      // 标准 Alpha 混合
    Additive,   // 加法混合
    Multiply    // 乘法混合
};

// ============================================================================
// 渲染后端抽象接口
// ============================================================================
class RenderBackend {
public:
    virtual ~RenderBackend() = default;

    // ------------------------------------------------------------------------
    // 生命周期
    // ------------------------------------------------------------------------
    virtual bool init(Window* window) = 0;
    virtual void shutdown() = 0;

    // ------------------------------------------------------------------------
    // 帧管理
    // ------------------------------------------------------------------------
    virtual void beginFrame(const Color& clearColor) = 0;
    virtual void endFrame() = 0;
    virtual void setViewport(int x, int y, int width, int height) = 0;
    virtual void setVSync(bool enabled) = 0;

    // ------------------------------------------------------------------------
    // 状态设置
    // ------------------------------------------------------------------------
    virtual void setBlendMode(BlendMode mode) = 0;
    virtual void setViewProjection(const glm::mat4& matrix) = 0;

    // ------------------------------------------------------------------------
    // 纹理
    // ------------------------------------------------------------------------
    virtual Ptr<Texture> createTexture(int width, int height, const uint8_t* pixels, int channels) = 0;
    virtual Ptr<Texture> loadTexture(const std::string& filepath) = 0;

    // ------------------------------------------------------------------------
    // 精灵批渲染
    // ------------------------------------------------------------------------
    virtual void beginSpriteBatch() = 0;
    virtual void drawSprite(const Texture& texture, 
                           const Rect& destRect,
                           const Rect& srcRect,
                           const Color& tint,
                           float rotation,
                           const Vec2& anchor) = 0;
    virtual void drawSprite(const Texture& texture,
                           const Vec2& position,
                           const Color& tint) = 0;
    virtual void endSpriteBatch() = 0;

    // ------------------------------------------------------------------------
    // 形状渲染
    // ------------------------------------------------------------------------
    virtual void drawLine(const Vec2& start, const Vec2& end, const Color& color, float width = 1.0f) = 0;
    virtual void drawRect(const Rect& rect, const Color& color, float width = 1.0f) = 0;
    virtual void fillRect(const Rect& rect, const Color& color) = 0;
    virtual void drawCircle(const Vec2& center, float radius, const Color& color, int segments = 32, float width = 1.0f) = 0;
    virtual void fillCircle(const Vec2& center, float radius, const Color& color, int segments = 32) = 0;
    virtual void drawTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color, float width = 1.0f) = 0;
    virtual void fillTriangle(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Color& color) = 0;
    virtual void drawPolygon(const std::vector<Vec2>& points, const Color& color, float width = 1.0f) = 0;
    virtual void fillPolygon(const std::vector<Vec2>& points, const Color& color) = 0;

    // ------------------------------------------------------------------------
    // 文字渲染
    // ------------------------------------------------------------------------
    virtual Ptr<FontAtlas> createFontAtlas(const std::string& filepath, int fontSize, bool useSDF = false) = 0;
    virtual void drawText(const FontAtlas& font, const String& text, const Vec2& position, const Color& color) = 0;
    virtual void drawText(const FontAtlas& font, const String& text, float x, float y, const Color& color) = 0;

    // ------------------------------------------------------------------------
    // 统计信息
    // ------------------------------------------------------------------------
    struct Stats {
        uint32_t drawCalls = 0;
        uint32_t triangleCount = 0;
        uint32_t textureBinds = 0;
        uint32_t shaderBinds = 0;
    };
    virtual Stats getStats() const = 0;
    virtual void resetStats() = 0;

    // ------------------------------------------------------------------------
    // 工厂方法
    // ------------------------------------------------------------------------
    static UniquePtr<RenderBackend> create(BackendType type);
};

} // namespace easy2d
