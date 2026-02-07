#pragma once

#include <easy2d/scene/node.h>
#include <easy2d/graphics/texture.h>

namespace easy2d {

// ============================================================================
// 精灵节点
// ============================================================================
class Sprite : public Node {
public:
    Sprite();
    explicit Sprite(Ptr<Texture> texture);
    ~Sprite() override = default;

    // 纹理
    void setTexture(Ptr<Texture> texture);
    Ptr<Texture> getTexture() const { return texture_; }

    // 纹理矩形 (用于图集)
    void setTextureRect(const Rect& rect);
    Rect getTextureRect() const { return textureRect_; }

    // 颜色混合
    void setColor(const Color& color);
    Color getColor() const { return color_; }

    // 翻转
    void setFlipX(bool flip);
    void setFlipY(bool flip);
    bool isFlipX() const { return flipX_; }
    bool isFlipY() const { return flipY_; }

    // 静态创建方法
    static Ptr<Sprite> create();
    static Ptr<Sprite> create(Ptr<Texture> texture);
    static Ptr<Sprite> create(Ptr<Texture> texture, const Rect& rect);

    Rect getBoundingBox() const override;

protected:
    void onDraw(RenderBackend& renderer) override;
    void generateRenderCommand(std::vector<RenderCommand>& commands, int zOrder) override;

private:
    Ptr<Texture> texture_;
    Rect textureRect_;
    Color color_ = Colors::White;
    bool flipX_ = false;
    bool flipY_ = false;
};

} // namespace easy2d
