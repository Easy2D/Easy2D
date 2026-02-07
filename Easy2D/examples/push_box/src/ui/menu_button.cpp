#include "menu_button.h"

#include <easy2d/core/color.h>
#include <easy2d/event/event.h>

namespace pushbox {

easy2d::Ptr<MenuButton> MenuButton::create(easy2d::Ptr<easy2d::FontAtlas> font,
                                          const easy2d::String& text,
                                          easy2d::Function<void()> onClick) {
  auto btn = easy2d::makePtr<MenuButton>();
  btn->setFont(font);
  btn->setText(text);
  btn->setPadding(easy2d::Vec2(0.0f, 0.0f));
  btn->setBackgroundColor(easy2d::Colors::Transparent, easy2d::Colors::Transparent,
                          easy2d::Colors::Transparent);
  btn->setBorder(easy2d::Colors::Transparent, 0.0f);
  btn->setTextColor(easy2d::Colors::Black);

  btn->onClick_ = std::move(onClick);
  btn->setOnClick([wbtn = easy2d::WeakPtr<MenuButton>(btn)]() {
    if (auto self = wbtn.lock()) {
      if (self->enabled_ && self->onClick_) {
        self->onClick_();
      }
    }
  });

  btn->getEventDispatcher().addListener(
      easy2d::EventType::UIHoverEnter,
      [wbtn = easy2d::WeakPtr<MenuButton>(btn)](easy2d::Event&) {
        if (auto self = wbtn.lock()) {
          if (self->enabled_) {
            self->setTextColor(easy2d::Colors::Blue);
          }
        }
      });

  btn->getEventDispatcher().addListener(
      easy2d::EventType::UIHoverExit,
      [wbtn = easy2d::WeakPtr<MenuButton>(btn)](easy2d::Event&) {
        if (auto self = wbtn.lock()) {
          if (self->enabled_) {
            self->setTextColor(easy2d::Colors::Black);
          }
        }
      });

  return btn;
}

void MenuButton::setEnabled(bool enabled) {
  enabled_ = enabled;
  setTextColor(enabled ? easy2d::Colors::Black : easy2d::Colors::LightGray);
}

} // namespace pushbox

