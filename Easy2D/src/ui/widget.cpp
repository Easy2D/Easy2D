#include <easy2d/ui/widget.h>
#include <cmath>

namespace easy2d {

Widget::Widget() {
    setSpatialIndexed(false);
}

void Widget::setSize(const Size& size) {
    size_ = size;
    updateSpatialIndex();
}

void Widget::setSize(float width, float height) {
    setSize(Size(width, height));
}

Rect Widget::getBoundingBox() const {
    if (size_.empty()) {
        return Rect();
    }

    auto pos = getPosition();
    auto anchor = getAnchor();
    auto scale = getScale();

    float w = size_.width * scale.x;
    float h = size_.height * scale.y;
    float x0 = pos.x - size_.width * anchor.x * scale.x;
    float y0 = pos.y - size_.height * anchor.y * scale.y;
    float x1 = x0 + w;
    float y1 = y0 + h;

    float l = std::min(x0, x1);
    float t = std::min(y0, y1);
    return Rect(l, t, std::abs(w), std::abs(h));
}

} // namespace easy2d
