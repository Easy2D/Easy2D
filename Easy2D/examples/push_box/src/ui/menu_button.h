#pragma once

#include <easy2d/easy2d.h>

namespace pushbox {

class MenuButton : public easy2d::Button {
public:
  static easy2d::Ptr<MenuButton> create(easy2d::Ptr<easy2d::FontAtlas> font,
                                        const easy2d::String& text,
                                        easy2d::Function<void()> onClick);

  void setEnabled(bool enabled);
  bool isEnabled() const { return enabled_; }

private:
  bool enabled_ = true;
  easy2d::Function<void()> onClick_;
};

} // namespace pushbox
