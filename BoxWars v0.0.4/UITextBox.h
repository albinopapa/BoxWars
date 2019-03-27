#pragma once

#include "Font.h"
#include "D3DGraphics.h"

#include <cstdint>
#include <string>

struct TextBox
{
	void Draw( const Vec2i& position, int width, int height, const Font& font, D3DGraphics& gfx )const noexcept;
	void DrawHorizontal( std::int32_t _x, std::int32_t _y, const Font& _font, D3DGraphics& _gfx )const noexcept;
	void DrawVertical( std::int32_t _x, std::int32_t _y, const Font& _font, D3DGraphics& _gfx )const noexcept;

	Color color = Colors::white;
	const char* name = nullptr;
	std::string text;
};
