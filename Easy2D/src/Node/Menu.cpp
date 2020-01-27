#include <easy2d/e2dnode.h>

easy2d::Menu::Menu()
	: _enable(true)
{
}

easy2d::Menu::Menu(const std::vector<Button*>& buttons)
	: _enable(true)
{
	for (auto button : buttons)
	{
		this->addButton(button);
	}
}

bool easy2d::Menu::isEnable() const
{
	return _enable;
}

size_t easy2d::Menu::getButtonCount() const
{
	return _buttons.size();
}

void easy2d::Menu::setEnable(bool enable)
{
	if (_enable != enable)
	{
		_enable = enable;

		for (auto button : _buttons)
		{
			button->setEnable(enable);
		}
	}
}

void easy2d::Menu::addButton(Button * button)
{
	if (button)
	{
		this->addChild(button);
		_buttons.push_back(button);
		button->setEnable(_enable);
	}
}

bool easy2d::Menu::removeButton(Button * button)
{
	if (_buttons.empty())
	{
		return false;
	}

	this->removeChild(button);

	if (button)
	{
		size_t size = _buttons.size();
		for (size_t i = 0; i < size; ++i)
		{
			if (_buttons[i] == button)
			{
				// 移除按钮前，将它启用
				button->setEnable(true);
				_buttons.erase(_buttons.begin() + i);
				return true;
			}
		}
	}
	return false;
}

const std::vector<easy2d::Button*>& easy2d::Menu::getAllButtons() const
{
	return _buttons;
}
