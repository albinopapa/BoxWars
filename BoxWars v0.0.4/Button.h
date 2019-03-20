#pragma once

#include "D3DGraphics.h"

#include <string>

class Button
{
public:
	Button() = default;
	Button( const char* _name, int _x, int _y )noexcept;

	bool IsActive()const noexcept;

	void SetActive()noexcept;
	void SetInactive()noexcept;
	void Draw( const Font& _font, D3DGraphics& _graphics )const noexcept;
public:
	static constexpr int width = 10;
	static constexpr int height = 10;

private:
	static constexpr Color activeFont = Colors::white, inactiveFont = Colors::gray;
	static constexpr Color activeColor = Colors::blue, inactiveColor = Colors::dark_blue;
	static constexpr Color activeBorder = Colors::white, inactiveBorder = Colors::gray;
	static constexpr Color pushed = Colors::green;

	std::string name;
	Color bgColor = inactiveColor;
	Color fontColor = inactiveFont;
	Color borderColor = inactiveBorder;
	int x = 0, y = 0;
	bool isActive = false;
};

