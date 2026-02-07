#include <easy2d/scene/text.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/graphics/render_command.h>

namespace easy2d {

Text::Text() {
    // 文字默认锚点为左上角，这样setPosition(0, 0)会在左上角显示
    setAnchor(0.0f, 0.0f);
}

Text::Text(const String& text) : text_(text) {
    sizeDirty_ = true;
    // 文字默认锚点为左上角，这样setPosition(0, 0)会在左上角显示
    setAnchor(0.0f, 0.0f);
}

void Text::setText(const String& text) {
    text_ = text;
    sizeDirty_ = true;
    updateSpatialIndex();
}

void Text::setFont(Ptr<FontAtlas> font) {
    font_ = font;
    sizeDirty_ = true;
    updateSpatialIndex();
}

void Text::setTextColor(const Color& color) {
    color_ = color;
}

void Text::setFontSize(int size) {
    fontSize_ = size;
    sizeDirty_ = true;
    updateSpatialIndex();
}

void Text::setAlignment(Alignment align) {
    alignment_ = align;
    updateSpatialIndex();
}

Vec2 Text::getTextSize() const {
    updateCache();
    return cachedSize_;
}

float Text::getLineHeight() const {
    if (font_) {
        return font_->getLineHeight();
    }
    return static_cast<float>(fontSize_);
}

void Text::updateCache() const {
    if (!sizeDirty_ || !font_) {
        return;
    }
    
    cachedSize_ = font_->measureText(text_);
    sizeDirty_ = false;
}

Ptr<Text> Text::create() {
    return makePtr<Text>();
}

Ptr<Text> Text::create(const String& text) {
    return makePtr<Text>(text);
}

Ptr<Text> Text::create(const String& text, Ptr<FontAtlas> font) {
    auto t = makePtr<Text>(text);
    t->setFont(font);
    return t;
}

Rect Text::getBoundingBox() const {
    if (!font_ || text_.empty()) {
        return Rect();
    }

    updateCache();
    Vec2 size = cachedSize_;
    if (size.x <= 0.0f || size.y <= 0.0f) {
        return Rect();
    }

    Vec2 pos = getPosition();

    if (alignment_ != Alignment::Left) {
        if (alignment_ == Alignment::Center) {
            pos.x -= size.x * 0.5f;
        } else if (alignment_ == Alignment::Right) {
            pos.x -= size.x;
        }
    }

    return Rect(pos.x, pos.y, size.x, size.y);
}

void Text::onDraw(RenderBackend& renderer) {
    if (!font_ || text_.empty()) {
        return;
    }
    
    Vec2 pos = getPosition();
    
    // Calculate horizontal offset based on alignment
    if (alignment_ != Alignment::Left) {
        Vec2 size = getTextSize();
        if (alignment_ == Alignment::Center) {
            pos.x -= size.x * 0.5f;
        } else if (alignment_ == Alignment::Right) {
            pos.x -= size.x;
        }
    }

    renderer.drawText(*font_, text_, pos, color_);
}

void Text::generateRenderCommand(std::vector<RenderCommand>& commands, int zOrder) {
    if (!font_ || text_.empty()) {
        return;
    }

    Vec2 pos = getPosition();

    // 计算对齐偏移（与 onDraw 一致）
    if (alignment_ != Alignment::Left) {
        Vec2 size = getTextSize();
        if (alignment_ == Alignment::Center) {
            pos.x -= size.x * 0.5f;
        } else if (alignment_ == Alignment::Right) {
            pos.x -= size.x;
        }
    }

    // 创建渲染命令
    RenderCommand cmd;
    cmd.type = RenderCommandType::Text;
    cmd.zOrder = zOrder;
    cmd.data = TextData{
        font_,
        text_,
        pos,
        color_
    };

    commands.push_back(std::move(cmd));
}

} // namespace easy2d
