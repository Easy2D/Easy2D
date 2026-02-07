#include <easy2d/graphics/opengl/gl_font_atlas.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#define STB_RECT_PACK_IMPLEMENTATION
#include <stb/stb_rect_pack.h>
#include <easy2d/utils/logger.h>
#include <fstream>
#include <algorithm>

namespace easy2d {

// ============================================================================
// 构造函数 - 初始化字体图集
// ============================================================================
GLFontAtlas::GLFontAtlas(const std::string& filepath, int fontSize, bool useSDF)
    : fontSize_(fontSize)
    , useSDF_(useSDF)
    , currentY_(0)
    , scale_(0.0f)
    , ascent_(0.0f)
    , descent_(0.0f)
    , lineGap_(0.0f) {
    
    // 加载字体文件
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        E2D_LOG_ERROR("Failed to load font: {}", filepath);
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    fontData_.resize(size);
    if (!file.read(reinterpret_cast<char*>(fontData_.data()), size)) {
        E2D_LOG_ERROR("Failed to read font file: {}", filepath);
        return;
    }

    // 初始化 stb_truetype
    if (!stbtt_InitFont(&fontInfo_, fontData_.data(), stbtt_GetFontOffsetForIndex(fontData_.data(), 0))) {
        E2D_LOG_ERROR("Failed to init font: {}", filepath);
        return;
    }

    scale_ = stbtt_ScaleForPixelHeight(&fontInfo_, static_cast<float>(fontSize_));
    
    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&fontInfo_, &ascent, &descent, &lineGap);
    ascent_ = static_cast<float>(ascent) * scale_;
    descent_ = static_cast<float>(descent) * scale_;
    lineGap_ = static_cast<float>(lineGap) * scale_;

    createAtlas();
}

// ============================================================================
// 析构函数
// ============================================================================
GLFontAtlas::~GLFontAtlas() = default;

// ============================================================================
// 获取字形 - 如果字形不存在则缓存它
// ============================================================================
const Glyph* GLFontAtlas::getGlyph(char32_t codepoint) const {
    auto it = glyphs_.find(codepoint);
    if (it == glyphs_.end()) {
        cacheGlyph(codepoint);
        it = glyphs_.find(codepoint);
    }
    return (it != glyphs_.end()) ? &it->second : nullptr;
}

// ============================================================================
// 测量文本尺寸
// ============================================================================
Vec2 GLFontAtlas::measureText(const String& text) {
    float width = 0.0f;
    float height = getAscent() - getDescent();
    float currentWidth = 0.0f;

    for (char32_t codepoint : text.toUtf32()) {
        if (codepoint == '\n') {
            width = std::max(width, currentWidth);
            currentWidth = 0.0f;
            height += getLineHeight();
            continue;
        }

        const Glyph* glyph = getGlyph(codepoint);
        if (glyph) {
            currentWidth += glyph->advance;
        }
    }

    width = std::max(width, currentWidth);
    return Vec2(width, height);
}

// ============================================================================
// 创建图集纹理 - 初始化空白纹理和矩形打包上下文
// ============================================================================
void GLFontAtlas::createAtlas() {
    int channels = useSDF_ ? 1 : 4;
    std::vector<uint8_t> emptyData(ATLAS_WIDTH * ATLAS_HEIGHT * channels, 0);
    texture_ = std::make_unique<GLTexture>(ATLAS_WIDTH, ATLAS_HEIGHT, emptyData.data(), channels);
    texture_->setFilter(true);
    
    // 初始化矩形打包上下文
    packNodes_.resize(ATLAS_WIDTH);
    stbrp_init_target(&packContext_, ATLAS_WIDTH, ATLAS_HEIGHT, packNodes_.data(), ATLAS_WIDTH);
}

// ============================================================================
// 缓存字形 - 渲染字形到图集并存储信息
// 使用 stb_rect_pack 进行矩形打包
// ============================================================================
void GLFontAtlas::cacheGlyph(char32_t codepoint) const {
    int advance = 0;
    stbtt_GetCodepointHMetrics(&fontInfo_, static_cast<int>(codepoint), &advance, nullptr);
    float advancePx = advance * scale_;

    if (useSDF_) {
        constexpr int SDF_PADDING = 8;
        constexpr unsigned char ONEDGE_VALUE = 128;
        constexpr float PIXEL_DIST_SCALE = 64.0f;

        int w = 0, h = 0, xoff = 0, yoff = 0;
        unsigned char* sdf = stbtt_GetCodepointSDF(&fontInfo_,
                                                   scale_,
                                                   static_cast<int>(codepoint),
                                                   SDF_PADDING,
                                                   ONEDGE_VALUE,
                                                   PIXEL_DIST_SCALE,
                                                   &w, &h, &xoff, &yoff);
        if (!sdf || w <= 0 || h <= 0) {
            if (sdf) stbtt_FreeSDF(sdf, nullptr);
            Glyph glyph{};
            glyph.advance = advancePx;
            glyphs_[codepoint] = glyph;
            return;
        }

        stbrp_rect rect;
        rect.id = static_cast<int>(codepoint);
        rect.w = w + PADDING * 2;
        rect.h = h + PADDING * 2;

        stbrp_pack_rects(&packContext_, &rect, 1);
        if (!rect.was_packed) {
            E2D_LOG_WARN("Font atlas is full, cannot cache codepoint: {}", static_cast<int>(codepoint));
            stbtt_FreeSDF(sdf, nullptr);
            return;
        }

        int atlasX = rect.x + PADDING;
        int atlasY = rect.y + PADDING;

        Glyph glyph;
        glyph.width = static_cast<float>(w);
        glyph.height = static_cast<float>(h);
        glyph.bearingX = static_cast<float>(xoff);
        glyph.bearingY = static_cast<float>(yoff);
        glyph.advance = advancePx;

        // stb_rect_pack 使用左上角为原点，OpenGL纹理使用左下角为原点
        // 需要翻转V坐标
        float v0 = static_cast<float>(atlasY) / ATLAS_HEIGHT;
        float v1 = static_cast<float>(atlasY + h) / ATLAS_HEIGHT;
        glyph.u0 = static_cast<float>(atlasX) / ATLAS_WIDTH;
        glyph.v0 = 1.0f - v1;  // 翻转V坐标
        glyph.u1 = static_cast<float>(atlasX + w) / ATLAS_WIDTH;
        glyph.v1 = 1.0f - v0;  // 翻转V坐标

        glyphs_[codepoint] = glyph;

        glBindTexture(GL_TEXTURE_2D, texture_->getTextureID());
        GLint prevUnpackAlignment = 4;
        glGetIntegerv(GL_UNPACK_ALIGNMENT, &prevUnpackAlignment);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // OpenGL纹理坐标原点在左下角，需要将Y坐标翻转
        glTexSubImage2D(GL_TEXTURE_2D, 0, atlasX, ATLAS_HEIGHT - atlasY - h, w, h, GL_RED, GL_UNSIGNED_BYTE, sdf);
        glPixelStorei(GL_UNPACK_ALIGNMENT, prevUnpackAlignment);

        stbtt_FreeSDF(sdf, nullptr);
        return;
    }

    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    stbtt_GetCodepointBitmapBox(&fontInfo_, static_cast<int>(codepoint), scale_, scale_, &x0, &y0, &x1, &y1);
    int w = x1 - x0;
    int h = y1 - y0;
    int xoff = x0;
    int yoff = y0;

    if (w <= 0 || h <= 0) {
        Glyph glyph{};
        glyph.advance = advancePx;
        glyphs_[codepoint] = glyph;
        return;
    }

    std::vector<unsigned char> bitmap(static_cast<size_t>(w) * static_cast<size_t>(h), 0);
    stbtt_MakeCodepointBitmap(&fontInfo_, bitmap.data(), w, h, w, scale_, scale_, static_cast<int>(codepoint));

    // 使用 stb_rect_pack 打包矩形
    stbrp_rect rect;
    rect.id = static_cast<int>(codepoint);
    rect.w = w + PADDING * 2;
    rect.h = h + PADDING * 2;
    
    stbrp_pack_rects(&packContext_, &rect, 1);
    
    if (!rect.was_packed) {
        // 图集已满，无法缓存更多字形
        E2D_LOG_WARN("Font atlas is full, cannot cache codepoint: {}", static_cast<int>(codepoint));
        return;
    }

    int atlasX = rect.x + PADDING;
    int atlasY = rect.y + PADDING;

    // 创建字形信息
    Glyph glyph;
    glyph.width = static_cast<float>(w);
    glyph.height = static_cast<float>(h);
    glyph.bearingX = static_cast<float>(xoff);
    glyph.bearingY = static_cast<float>(yoff);
    glyph.advance = advancePx;

    // 计算纹理坐标（相对于图集）
    // stb_rect_pack 使用左上角为原点，OpenGL纹理使用左下角为原点
    // 需要翻转V坐标
    float v0 = static_cast<float>(atlasY) / ATLAS_HEIGHT;
    float v1 = static_cast<float>(atlasY + h) / ATLAS_HEIGHT;
    glyph.u0 = static_cast<float>(atlasX) / ATLAS_WIDTH;
    glyph.v0 = 1.0f - v1;  // 翻转V坐标
    glyph.u1 = static_cast<float>(atlasX + w) / ATLAS_WIDTH;
    glyph.v1 = 1.0f - v0;  // 翻转V坐标

    // 存储字形
    glyphs_[codepoint] = glyph;

    // 将单通道字形数据转换为 RGBA 格式（白色字形，Alpha 通道存储灰度）
    std::vector<uint8_t> rgbaData(w * h * 4);
    for (int i = 0; i < w * h; ++i) {
        uint8_t alpha = bitmap[static_cast<size_t>(i)];
        rgbaData[i * 4 + 0] = 255;  // R
        rgbaData[i * 4 + 1] = 255;  // G
        rgbaData[i * 4 + 2] = 255;  // B
        rgbaData[i * 4 + 3] = alpha; // A
    }

    // 更新纹理 - 将字形数据上传到图集的指定位置
    // OpenGL纹理坐标原点在左下角，需要将Y坐标翻转
    glBindTexture(GL_TEXTURE_2D, texture_->getTextureID());
    glTexSubImage2D(GL_TEXTURE_2D, 0, atlasX, ATLAS_HEIGHT - atlasY - h, w, h, GL_RGBA, GL_UNSIGNED_BYTE, rgbaData.data());
}

} // namespace easy2d
