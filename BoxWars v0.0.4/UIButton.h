#pragma once

#include "Rect.h"
#include "Font.h"
#include "D3DGraphics.h"

struct Button
{
	Button() = default;
	Button( const char* _name )noexcept;

	void Select()noexcept { hilighted = true; }
	void Deselect()noexcept { hilighted = false; }
	void Draw( const Vec2i& pos, const Font& font, D3DGraphics& gfx )const noexcept;

	static constexpr Color color = Colors::white;
	const char* name;
	bool hilighted = false;
};
