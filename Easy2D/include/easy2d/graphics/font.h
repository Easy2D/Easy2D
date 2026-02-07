#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/color.h>
#include <easy2d/core/string.h>
#include <easy2d/core/math_types.h>

namespace easy2d {

// ============================================================================
// 字形信息
// ============================================================================
struct Glyph {
    float u0, v0;       // 纹理坐标左下角
    float u1, v1;       // 纹理坐标右上角
    float width;        // 字形宽度（像素）
    float height;       // 字形高度（像素）
    float bearingX;     // 水平偏移
    float bearingY;     // 垂直偏移
    float advance;      // 前进距离
};

// ============================================================================
// 字体图集接口
// ============================================================================
class FontAtlas {
public:
    virtual ~FontAtlas() = default;

    // 获取字形信息
    virtual const Glyph* getGlyph(char32_t codepoint) const = 0;
    
    // 获取纹理
    virtual class Texture* getTexture() const = 0;
    
    // 获取字体大小
    virtual int getFontSize() const = 0;

    virtual float getAscent() const = 0;
    virtual float getDescent() const = 0;
    virtual float getLineGap() const = 0;
    virtual float getLineHeight() const = 0;
    
    // 计算文字尺寸
    virtual Vec2 measureText(const String& text) = 0;
    
    // 是否支持 SDF 渲染
    virtual bool isSDF() const = 0;
};

} // namespace easy2d
