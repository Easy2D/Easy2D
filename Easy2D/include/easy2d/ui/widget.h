#pragma once

#include <easy2d/scene/node.h>

namespace easy2d {

class Widget : public Node {
public:
    Widget();
    ~Widget() override = default;

    void setSize(const Size& size);
    void setSize(float width, float height);
    Size getSize() const { return size_; }

    Rect getBoundingBox() const override;

private:
    Size size_ = Size::Zero();
};

} // namespace easy2d

