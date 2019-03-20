#pragma once

#include "Font.h"
#include "Rect.h"
#include "Sprite.h"

#include <memory>

#include <d3d9.h>
#include <wrl/client.h>

//define the total width and height of the screen
constexpr auto TOTALSCREENWIDTH = 1400;
constexpr auto TOTALSCREENHEIGHT = 700;


class D3DGraphics
{
public:
	D3DGraphics( HWND hWnd );

	void PutPixel( int x, int y, Color c )noexcept;
	void DrawLine( int x1, int y1, int x2, int y2, Color color )noexcept;
	void DrawCircle( int cx, int cy, int radius, Color color )noexcept;
	void DrawDisc( int cx, int cy, int r, Color color )noexcept;
	void DrawSprite( int x, int y, const Sprite& sprite )noexcept;
	void DrawChar( char c, int x, int y, const Font& font, Color color )noexcept;
	void DrawString( const char* string, int xoff, int yoff, const Font& font, Color color )noexcept;
	void BeginFrame()noexcept;
	void EndFrame();

	void DrawRectangle( const RectI& _rect, Color color )noexcept;
	void DrawEmptyRectangle( const RectI& _rect, Color color )noexcept;

private:
	Microsoft::WRL::ComPtr<IDirect3D9>			pDirect3D;
	Microsoft::WRL::ComPtr<IDirect3DDevice9>	pDevice;
	Microsoft::WRL::ComPtr<IDirect3DSurface9>	pBackBuffer;
	Sprite surface;
};