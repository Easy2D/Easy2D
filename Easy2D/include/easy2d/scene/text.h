#pragma once

#include <easy2d/scene/node.h>
#include <easy2d/core/color.h>
#include <easy2d/core/string.h>
#include <easy2d/graphics/font.h>

namespace easy2d {

// ============================================================================
// 文字节点
// ============================================================================
class Text : public Node {
public:
    Text();
    explicit Text(const String& text);
    ~Text() override = default;

    // ------------------------------------------------------------------------
    // 文字内容
    // ------------------------------------------------------------------------
    void setText(const String& text);
    const String& getText() const { return text_; }

    // ------------------------------------------------------------------------
    // 字体
    // ------------------------------------------------------------------------
    void setFont(Ptr<FontAtlas> font);
    Ptr<FontAtlas> getFont() const { return font_; }

    // ------------------------------------------------------------------------
    // 文字属性
    // ------------------------------------------------------------------------
    void setTextColor(const Color& color);
    Color getTextColor() const { return color_; }

    void setFontSize(int size);
    int getFontSize() const { return fontSize_; }

    // ------------------------------------------------------------------------
    // 对齐方式
    // ------------------------------------------------------------------------
    enum class Alignment {
        Left,
        Center,
        Right
    };
    
    void setAlignment(Alignment align);
    Alignment getAlignment() const { return alignment_; }

    // ------------------------------------------------------------------------
    // 尺寸计算
    // ------------------------------------------------------------------------
    Vec2 getTextSize() const;
    float getLineHeight() const;

    // ------------------------------------------------------------------------
    // 静态创建方法
    // ------------------------------------------------------------------------
    static Ptr<Text> create();
    static Ptr<Text> create(const String& text);
    static Ptr<Text> create(const String& text, Ptr<FontAtlas> font);

    Rect getBoundingBox() const override;

protected:
    void onDraw(RenderBackend& renderer) override;
    void generateRenderCommand(std::vector<RenderCommand>& commands, int zOrder) override;

private:
    String text_;
    Ptr<FontAtlas> font_;
    Color color_ = Colors::White;
    int fontSize_ = 16;
    Alignment alignment_ = Alignment::Left;
    
    mutable Vec2 cachedSize_ = Vec2::Zero();
    mutable bool sizeDirty_ = true;
    
    void updateCache() const;
};

} // namespace easy2d
