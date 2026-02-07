#include <easy2d/ui/button.h>
#include <easy2d/graphics/render_backend.h>
#include <easy2d/app/application.h>
#include <algorithm>
#include <cmath>

namespace easy2d {

/**
 * @brief 构造函数，初始化按钮状态并设置事件监听器
 */
Button::Button() {
    setSpatialIndexed(false);
    auto& dispatcher = getEventDispatcher();
    dispatcher.addListener(EventType::UIHoverEnter, [this](Event&) {
        hovered_ = true;
        // 鼠标进入按钮区域，改变光标为手型
        auto& app = Application::instance();
        app.window().setCursor(hoverCursor_);
        cursorChanged_ = true;
    });
    dispatcher.addListener(EventType::UIHoverExit, [this](Event&) {
        hovered_ = false;
        pressed_ = false;
        // 鼠标离开按钮区域，恢复默认光标
        if (cursorChanged_) {
            auto& app = Application::instance();
            app.window().resetCursor();
            cursorChanged_ = false;
        }
    });
    dispatcher.addListener(EventType::UIPressed, [this](Event&) { pressed_ = true; });
    dispatcher.addListener(EventType::UIReleased, [this](Event&) { pressed_ = false; });
    dispatcher.addListener(EventType::UIClicked, [this](Event&) { if (onClick_) onClick_(); });
}

/**
 * @brief 创建按钮实例的工厂方法
 * @return 按钮智能指针
 */
Ptr<Button> Button::create() {
    return makePtr<Button>();
}

/**
 * @brief 设置按钮文字，并根据文字大小自动调整按钮尺寸
 * @param text 要显示的文字
 */
void Button::setText(const String& text) {
    text_ = text;
    if (font_ && getSize().empty()) {
        Vec2 textSize = font_->measureText(text_);
        setSize(textSize.x + padding_.x * 2.0f, textSize.y + padding_.y * 2.0f);
    }
}

/**
 * @brief 设置字体，并根据当前文字自动调整按钮尺寸
 * @param font 字体图集
 */
void Button::setFont(Ptr<FontAtlas> font) {
    font_ = font;
    if (font_ && getSize().empty() && !text_.empty()) {
        Vec2 textSize = font_->measureText(text_);
        setSize(textSize.x + padding_.x * 2.0f, textSize.y + padding_.y * 2.0f);
    }
}

/**
 * @brief 设置内边距，并根据当前文字自动调整按钮尺寸
 * @param padding 内边距向量
 */
void Button::setPadding(const Vec2& padding) {
    padding_ = padding;
    if (font_ && getSize().empty() && !text_.empty()) {
        Vec2 textSize = font_->measureText(text_);
        setSize(textSize.x + padding_.x * 2.0f, textSize.y + padding_.y * 2.0f);
    }
}

/**
 * @brief 设置文字颜色
 * @param color 颜色值
 */
void Button::setTextColor(const Color& color) {
    textColor_ = color;
}

/**
 * @brief 设置纯色背景的颜色状态
 * @param normal 正常状态颜色
 * @param hover 悬停状态颜色
 * @param pressed 按下状态颜色
 */
void Button::setBackgroundColor(const Color& normal, const Color& hover, const Color& pressed) {
    bgNormal_ = normal;
    bgHover_ = hover;
    bgPressed_ = pressed;
}

/**
 * @brief 设置边框样式
 * @param color 边框颜色
 * @param width 边框宽度
 */
void Button::setBorder(const Color& color, float width) {
    borderColor_ = color;
    borderWidth_ = width;
}

/**
 * @brief 设置圆角半径
 * @param radius 圆角半径（像素）
 */
void Button::setCornerRadius(float radius) {
    cornerRadius_ = std::max(0.0f, radius);
}

/**
 * @brief 设置是否启用圆角矩形
 * @param enabled 是否启用
 */
void Button::setRoundedCornersEnabled(bool enabled) {
    roundedCornersEnabled_ = enabled;
}

/**
 * @brief 设置是否使用Alpha遮罩进行点击检测（用于不规则形状按钮）
 * @param enabled 是否启用
 */
void Button::setUseAlphaMaskForHitTest(bool enabled) {
    useAlphaMaskForHitTest_ = enabled;
}

/**
 * @brief 设置点击回调函数
 * @param callback 回调函数
 */
void Button::setOnClick(Function<void()> callback) {
    onClick_ = std::move(callback);
}

/**
 * @brief 设置悬停时的鼠标光标形状
 * @param cursor 光标形状枚举
 */
void Button::setHoverCursor(CursorShape cursor) {
    hoverCursor_ = cursor;
}

/**
 * @brief 设置背景图片，支持三种状态
 * @param normal 正常状态图片
 * @param hover 悬停状态图片（可选）
 * @param pressed 按下状态图片（可选）
 */
void Button::setBackgroundImage(Ptr<Texture> normal, Ptr<Texture> hover, Ptr<Texture> pressed) {
    imgNormal_ = normal;
    imgHover_ = hover ? hover : normal;
    imgPressed_ = pressed ? pressed : (hover ? hover : normal);
    useImageBackground_ = (normal != nullptr);

    // 如果使用原图大小模式，设置按钮大小为图片大小
    if (useImageBackground_ && scaleMode_ == ImageScaleMode::Original && normal) {
        setSize(static_cast<float>(normal->getWidth()), static_cast<float>(normal->getHeight()));
    }
}

/**
 * @brief 设置图片缩放模式
 * @param mode 缩放模式枚举值
 */
void Button::setBackgroundImageScaleMode(ImageScaleMode mode) {
    scaleMode_ = mode;
    // 如果切换到原图大小模式，更新按钮大小
    if (useImageBackground_ && scaleMode_ == ImageScaleMode::Original && imgNormal_) {
        setSize(static_cast<float>(imgNormal_->getWidth()), static_cast<float>(imgNormal_->getHeight()));
    }
}

/**
 * @brief 设置自定义尺寸
 * @param size 尺寸向量
 */
void Button::setCustomSize(const Vec2& size) {
    setSize(size.x, size.y);
}

/**
 * @brief 设置自定义尺寸
 * @param width 宽度
 * @param height 高度
 */
void Button::setCustomSize(float width, float height) {
    setSize(width, height);
}

/**
 * @brief 根据缩放模式计算图片绘制尺寸
 * @param buttonSize 按钮尺寸
 * @param imageSize 图片原始尺寸
 * @return 计算后的绘制尺寸
 */
Vec2 Button::calculateImageSize(const Vec2& buttonSize, const Vec2& imageSize) {
    switch (scaleMode_) {
        case ImageScaleMode::Original:
            return imageSize;

        case ImageScaleMode::Stretch:
            return buttonSize;

        case ImageScaleMode::ScaleFit: {
            float scaleX = buttonSize.x / imageSize.x;
            float scaleY = buttonSize.y / imageSize.y;
            float scale = std::min(scaleX, scaleY);
            return Vec2(imageSize.x * scale, imageSize.y * scale);
        }

        case ImageScaleMode::ScaleFill: {
            float scaleX = buttonSize.x / imageSize.x;
            float scaleY = buttonSize.y / imageSize.y;
            float scale = std::max(scaleX, scaleY);
            return Vec2(imageSize.x * scale, imageSize.y * scale);
        }
    }
    return imageSize;
}

/**
 * @brief 绘制背景图片，根据当前状态选择对应的图片
 * @param renderer 渲染后端
 * @param rect 按钮矩形区域
 */
void Button::drawBackgroundImage(RenderBackend& renderer, const Rect& rect) {
    Texture* texture = nullptr;
    if (pressed_ && imgPressed_) {
        texture = imgPressed_.get();
    } else if (hovered_ && imgHover_) {
        texture = imgHover_.get();
    } else if (imgNormal_) {
        texture = imgNormal_.get();
    }

    if (!texture) return;

    Vec2 imageSize(static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));
    Vec2 buttonSize(rect.size.width, rect.size.height);
    Vec2 drawSize = calculateImageSize(buttonSize, imageSize);

    // 计算绘制位置（居中）
    Vec2 drawPos(
        rect.origin.x + (rect.size.width - drawSize.x) * 0.5f,
        rect.origin.y + (rect.size.height - drawSize.y) * 0.5f
    );

    Rect destRect(drawPos.x, drawPos.y, drawSize.x, drawSize.y);
    
    // 绘制图片（使用Alpha混合）
    renderer.drawSprite(*texture, destRect, Rect(0, 0, imageSize.x, imageSize.y), Colors::White, 0.0f, Vec2::Zero());
}

/**
 * @brief 绘制圆角矩形边框
 * @param renderer 渲染后端
 * @param rect 矩形区域
 * @param color 颜色
 * @param radius 圆角半径
 */
void Button::drawRoundedRect(RenderBackend& renderer, const Rect& rect, const Color& color, float radius) {
    // 限制圆角半径不超过矩形尺寸的一半
    float maxRadius = std::min(rect.size.width, rect.size.height) * 0.5f;
    radius = std::min(radius, maxRadius);
    
    if (radius <= 0.0f) {
        // 圆角为0，使用普通矩形
        renderer.drawRect(rect, color, borderWidth_);
        return;
    }

    const int segments = 8; // 每个圆角的线段数
    float x = rect.origin.x;
    float y = rect.origin.y;
    float w = rect.size.width;
    float h = rect.size.height;
    float r = radius;

    // 绘制四条直线边
    // 上边
    renderer.drawLine(Vec2(x + r, y), Vec2(x + w - r, y), color, borderWidth_);
    // 下边
    renderer.drawLine(Vec2(x + r, y + h), Vec2(x + w - r, y + h), color, borderWidth_);
    // 左边
    renderer.drawLine(Vec2(x, y + r), Vec2(x, y + h - r), color, borderWidth_);
    // 右边
    renderer.drawLine(Vec2(x + w, y + r), Vec2(x + w, y + h - r), color, borderWidth_);

    // 绘制四个圆角（使用线段近似）
    // 左上角
    for (int i = 0; i < segments; i++) {
        float angle1 = 3.14159f * 0.5f * (float)i / segments + 3.14159f;
        float angle2 = 3.14159f * 0.5f * (float)(i + 1) / segments + 3.14159f;
        Vec2 p1(x + r + r * cosf(angle1), y + r + r * sinf(angle1));
        Vec2 p2(x + r + r * cosf(angle2), y + r + r * sinf(angle2));
        renderer.drawLine(p1, p2, color, borderWidth_);
    }
    // 右上角
    for (int i = 0; i < segments; i++) {
        float angle1 = 3.14159f * 0.5f * (float)i / segments + 3.14159f * 1.5f;
        float angle2 = 3.14159f * 0.5f * (float)(i + 1) / segments + 3.14159f * 1.5f;
        Vec2 p1(x + w - r + r * cosf(angle1), y + r + r * sinf(angle1));
        Vec2 p2(x + w - r + r * cosf(angle2), y + r + r * sinf(angle2));
        renderer.drawLine(p1, p2, color, borderWidth_);
    }
    // 右下角
    for (int i = 0; i < segments; i++) {
        float angle1 = 3.14159f * 0.5f * (float)i / segments;
        float angle2 = 3.14159f * 0.5f * (float)(i + 1) / segments;
        Vec2 p1(x + w - r + r * cosf(angle1), y + h - r + r * sinf(angle1));
        Vec2 p2(x + w - r + r * cosf(angle2), y + h - r + r * sinf(angle2));
        renderer.drawLine(p1, p2, color, borderWidth_);
    }
    // 左下角
    for (int i = 0; i < segments; i++) {
        float angle1 = 3.14159f * 0.5f * (float)i / segments + 3.14159f * 0.5f;
        float angle2 = 3.14159f * 0.5f * (float)(i + 1) / segments + 3.14159f * 0.5f;
        Vec2 p1(x + r + r * cosf(angle1), y + h - r + r * sinf(angle1));
        Vec2 p2(x + r + r * cosf(angle2), y + h - r + r * sinf(angle2));
        renderer.drawLine(p1, p2, color, borderWidth_);
    }
}

/**
 * @brief 填充圆角矩形
 * @param renderer 渲染后端
 * @param rect 矩形区域
 * @param color 颜色
 * @param radius 圆角半径
 */
void Button::fillRoundedRect(RenderBackend& renderer, const Rect& rect, const Color& color, float radius) {
    // 限制圆角半径不超过矩形尺寸的一半
    float maxRadius = std::min(rect.size.width, rect.size.height) * 0.5f;
    radius = std::min(radius, maxRadius);
    
    if (radius <= 0.0f) {
        // 圆角为0，使用普通矩形填充
        renderer.fillRect(rect, color);
        return;
    }

    const int segments = 8; // 每个圆角的线段数
    float x = rect.origin.x;
    float y = rect.origin.y;
    float w = rect.size.width;
    float h = rect.size.height;
    float r = radius;

    // 构建圆角矩形的顶点（使用三角形扇形填充）
    std::vector<Vec2> vertices;
    
    // 中心矩形区域
    vertices.push_back(Vec2(x + r, y + r));           // 左上内角
    vertices.push_back(Vec2(x + w - r, y + r));       // 右上内角
    vertices.push_back(Vec2(x + w - r, y + h - r));   // 右下内角
    vertices.push_back(Vec2(x + r, y + h - r));       // 左下内角
    
    renderer.fillPolygon(vertices, color);
    
    // 填充四个侧边矩形
    // 上边
    renderer.fillRect(Rect(x + r, y, w - 2 * r, r), color);
    // 下边
    renderer.fillRect(Rect(x + r, y + h - r, w - 2 * r, r), color);
    // 左边
    renderer.fillRect(Rect(x, y + r, r, h - 2 * r), color);
    // 右边
    renderer.fillRect(Rect(x + w - r, y + r, r, h - 2 * r), color);

    // 填充四个圆角（使用扇形）
    // 左上角
    vertices.clear();
    vertices.push_back(Vec2(x + r, y + r));
    for (int i = 0; i <= segments; i++) {
        float angle = 3.14159f + 3.14159f * 0.5f * (float)i / segments;
        vertices.push_back(Vec2(x + r + r * cosf(angle), y + r + r * sinf(angle)));
    }
    renderer.fillPolygon(vertices, color);
    
    // 右上角
    vertices.clear();
    vertices.push_back(Vec2(x + w - r, y + r));
    for (int i = 0; i <= segments; i++) {
        float angle = 3.14159f * 1.5f + 3.14159f * 0.5f * (float)i / segments;
        vertices.push_back(Vec2(x + w - r + r * cosf(angle), y + r + r * sinf(angle)));
    }
    renderer.fillPolygon(vertices, color);
    
    // 右下角
    vertices.clear();
    vertices.push_back(Vec2(x + w - r, y + h - r));
    for (int i = 0; i <= segments; i++) {
        float angle = 0 + 3.14159f * 0.5f * (float)i / segments;
        vertices.push_back(Vec2(x + w - r + r * cosf(angle), y + h - r + r * sinf(angle)));
    }
    renderer.fillPolygon(vertices, color);
    
    // 左下角
    vertices.clear();
    vertices.push_back(Vec2(x + r, y + h - r));
    for (int i = 0; i <= segments; i++) {
        float angle = 3.14159f * 0.5f + 3.14159f * 0.5f * (float)i / segments;
        vertices.push_back(Vec2(x + r + r * cosf(angle), y + h - r + r * sinf(angle)));
    }
    renderer.fillPolygon(vertices, color);
}

/**
 * @brief 按钮绘制主函数，按正确顺序渲染所有视觉元素
 * 
 * 渲染顺序：
 * 1. 背景（图片或纯色）- 最底层
 * 2. 边框 - 中间层
 * 3. 文字 - 最顶层
 * 
 * 注意：此方法在场景渲染的精灵批次中被调用。
 * 由于 fillRect 和 drawRect 使用形状渲染管线（与精灵批次不同），
 * 需要在调用它们之前结束精灵批次，然后在 drawText 之前重新开始。
 */
void Button::onDraw(RenderBackend& renderer) {
    Rect rect = getBoundingBox();
    if (rect.empty()) {
        return;
    }

    // ========== 第1层：绘制背景（图片或纯色）==========
    if (useImageBackground_) {
        // 图片背景使用精灵批次绘制
        drawBackgroundImage(renderer, rect);
    } else {
        // 纯色背景使用 fillRect 或 fillRoundedRect 绘制
        renderer.endSpriteBatch();
        
        Color bg = bgNormal_;
        if (pressed_) {
            bg = bgPressed_;
        } else if (hovered_) {
            bg = bgHover_;
        }
        
        if (roundedCornersEnabled_) {
            fillRoundedRect(renderer, rect, bg, cornerRadius_);
        } else {
            renderer.fillRect(rect, bg);
        }
        
        renderer.beginSpriteBatch();
    }

    // ========== 第2层：绘制边框 ==========
    renderer.endSpriteBatch();
    
    if (borderWidth_ > 0.0f) {
        if (roundedCornersEnabled_) {
            drawRoundedRect(renderer, rect, borderColor_, cornerRadius_);
        } else {
            renderer.drawRect(rect, borderColor_, borderWidth_);
        }
    }
    
    renderer.beginSpriteBatch();

    // ========== 第3层：绘制文字 ==========
    if (font_ && !text_.empty()) {
        Vec2 textSize = font_->measureText(text_);
        
        Vec2 textPos(
            rect.center().x - textSize.x * 0.5f,
            rect.center().y - textSize.y * 0.5f
        );

        float minX = rect.left() + padding_.x;
        float minY = rect.top() + padding_.y;
        float maxX = rect.right() - padding_.x - textSize.x;
        float maxY = rect.bottom() - padding_.y - textSize.y;
        
        textPos.x = std::max(minX, std::min(textPos.x, maxX));
        textPos.y = std::max(minY, std::min(textPos.y, maxY));

        Color finalTextColor = textColor_;
        finalTextColor.a = 1.0f;
        
        renderer.drawText(*font_, text_, textPos, finalTextColor);
    }
}

// ============================================================================
// ToggleImageButton 实现
// ============================================================================

/**
 * @brief 切换按钮构造函数
 */
ToggleImageButton::ToggleImageButton() {
    setOnClick([this]() {
        toggle();
    });
}

/**
 * @brief 创建切换按钮实例的工厂方法
 * @return 切换按钮智能指针
 */
Ptr<ToggleImageButton> ToggleImageButton::create() {
    return makePtr<ToggleImageButton>();
}

/**
 * @brief 设置两种状态的图片
 * @param stateOffNormal 关闭状态-正常
 * @param stateOnNormal 开启状态-正常
 * @param stateOffHover 关闭状态-悬停（可选）
 * @param stateOnHover 开启状态-悬停（可选）
 * @param stateOffPressed 关闭状态-按下（可选）
 * @param stateOnPressed 开启状态-按下（可选）
 */
void ToggleImageButton::setStateImages(Ptr<Texture> stateOffNormal, Ptr<Texture> stateOnNormal,
                                        Ptr<Texture> stateOffHover, Ptr<Texture> stateOnHover,
                                        Ptr<Texture> stateOffPressed, Ptr<Texture> stateOnPressed) {
    imgOffNormal_ = stateOffNormal;
    imgOnNormal_ = stateOnNormal;
    imgOffHover_ = stateOffHover ? stateOffHover : stateOffNormal;
    imgOnHover_ = stateOnHover ? stateOnHover : stateOnNormal;
    imgOffPressed_ = stateOffPressed ? stateOffPressed : stateOffNormal;
    imgOnPressed_ = stateOnPressed ? stateOnPressed : stateOnNormal;

    if (imgOffNormal_) {
        setSize(static_cast<float>(imgOffNormal_->getWidth()),
                static_cast<float>(imgOffNormal_->getHeight()));
    }
}

/**
 * @brief 设置开关状态
 * @param on 是否开启
 */
void ToggleImageButton::setOn(bool on) {
    if (isOn_ != on) {
        isOn_ = on;
        if (onStateChange_) {
            onStateChange_(isOn_);
        }
    }
}

/**
 * @brief 切换当前状态
 */
void ToggleImageButton::toggle() {
    setOn(!isOn_);
}

/**
 * @brief 设置状态改变回调
 * @param callback 回调函数
 */
void ToggleImageButton::setOnStateChange(Function<void(bool)> callback) {
    onStateChange_ = std::move(callback);
}

/**
 * @brief 设置两种状态的文字
 * @param textOff 关闭状态文字
 * @param textOn 开启状态文字
 */
void ToggleImageButton::setStateText(const String& textOff, const String& textOn) {
    textOff_ = textOff;
    textOn_ = textOn;
    useStateText_ = true;
}

/**
 * @brief 设置两种状态的文字颜色
 * @param colorOff 关闭状态颜色
 * @param colorOn 开启状态颜色
 */
void ToggleImageButton::setStateTextColor(const Color& colorOff, const Color& colorOn) {
    textColorOff_ = colorOff;
    textColorOn_ = colorOn;
    useStateTextColor_ = true;
}

/**
 * @brief 切换按钮绘制主函数
 * 
 * 渲染顺序：
 * 1. 状态图片背景 - 最底层
 * 2. 边框 - 中间层
 * 3. 状态文字 - 最顶层
 */
void ToggleImageButton::onDraw(RenderBackend& renderer) {
    Rect rect = getBoundingBox();
    if (rect.empty()) {
        return;
    }

    // ========== 第1层：根据当前状态和交互状态选择并绘制图片 ==========
    Ptr<Texture> texture = nullptr;

    if (isOn_) {
        if (isPressed() && imgOnPressed_) {
            texture = imgOnPressed_;
        } else if (isHovered() && imgOnHover_) {
            texture = imgOnHover_;
        } else {
            texture = imgOnNormal_;
        }
    } else {
        if (isPressed() && imgOffPressed_) {
            texture = imgOffPressed_;
        } else if (isHovered() && imgOffHover_) {
            texture = imgOffHover_;
        } else {
            texture = imgOffNormal_;
        }
    }

    if (texture) {
        Vec2 imageSize(static_cast<float>(texture->getWidth()), static_cast<float>(texture->getHeight()));
        Vec2 buttonSize(rect.size.width, rect.size.height);
        Vec2 drawSize = imageSize;

        Vec2 drawPos(
            rect.origin.x + (buttonSize.x - drawSize.x) * 0.5f,
            rect.origin.y + (buttonSize.y - drawSize.y) * 0.5f
        );

        Rect destRect(drawPos.x, drawPos.y, drawSize.x, drawSize.y);
        renderer.drawSprite(*texture, destRect, Rect(0, 0, imageSize.x, imageSize.y), Colors::White, 0.0f, Vec2::Zero());
    }

    // ========== 第2层：绘制边框 ==========
    renderer.endSpriteBatch();
    
    float borderWidth = 1.0f;
    Color borderColor = isOn_ ? Color(0.0f, 1.0f, 0.0f, 0.8f) : Color(0.6f, 0.6f, 0.6f, 1.0f);
    if (borderWidth > 0.0f) {
        if (isRoundedCornersEnabled()) {
            drawRoundedRect(renderer, rect, borderColor, getCornerRadius());
        } else {
            renderer.drawRect(rect, borderColor, borderWidth);
        }
    }
    
    renderer.beginSpriteBatch();

    // ========== 第3层：绘制状态文字 ==========
    auto font = getFont();
    if (font) {
        String textToDraw;
        if (useStateText_) {
            textToDraw = isOn_ ? textOn_ : textOff_;
        } else {
            textToDraw = getText();
        }

        Color colorToUse;
        if (useStateTextColor_) {
            colorToUse = isOn_ ? textColorOn_ : textColorOff_;
        } else {
            colorToUse = getTextColor();
        }

        if (!textToDraw.empty()) {
            Vec2 textSize = font->measureText(textToDraw);
            Vec2 textPos(rect.center().x - textSize.x * 0.5f, rect.center().y - textSize.y * 0.5f);
            
            colorToUse.a = 1.0f;
            
            renderer.drawText(*font, textToDraw, textPos, colorToUse);
        }
    }
}

} // namespace easy2d
