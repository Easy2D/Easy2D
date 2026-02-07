#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/color.h>
#include <easy2d/core/math_types.h>
#include <easy2d/graphics/font.h>
#include <easy2d/graphics/texture.h>
#include <easy2d/graphics/opengl/gl_texture.h>
#include <stb/stb_truetype.h>
#include <stb/stb_rect_pack.h>
#include <unordered_map>
#include <vector>
#include <memory>

namespace easy2d {

// ============================================================================
// OpenGL 字体图集实现 - 使用 stb_rect_pack 进行矩形打包
// ============================================================================
class GLFontAtlas : public FontAtlas {
public:
    GLFontAtlas(const std::string& filepath, int fontSize, bool useSDF = false);
    ~GLFontAtlas();

    // FontAtlas 接口实现
    const Glyph* getGlyph(char32_t codepoint) const override;
    Texture* getTexture() const override { return texture_.get(); }
    int getFontSize() const override { return fontSize_; }
    float getAscent() const override { return ascent_; }
    float getDescent() const override { return descent_; }
    float getLineGap() const override { return lineGap_; }
    float getLineHeight() const override { return ascent_ - descent_ + lineGap_; }
    Vec2 measureText(const String& text) override;
    bool isSDF() const override { return useSDF_; }

private:
    // 图集配置
    static constexpr int ATLAS_WIDTH = 512;
    static constexpr int ATLAS_HEIGHT = 512;
    static constexpr int PADDING = 2;  // 字形之间的间距

    int fontSize_;
    bool useSDF_;
    mutable std::unique_ptr<GLTexture> texture_;
    mutable std::unordered_map<char32_t, Glyph> glyphs_;
    
    // stb_rect_pack 上下文
    mutable stbrp_context packContext_;
    mutable std::vector<stbrp_node> packNodes_;
    mutable int currentY_;
    
    std::vector<unsigned char> fontData_;
    stbtt_fontinfo fontInfo_;
    float scale_;
    float ascent_;
    float descent_;
    float lineGap_;

    void createAtlas();
    void cacheGlyph(char32_t codepoint) const;
};

} // namespace easy2d
