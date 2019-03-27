#pragma once

#include "Font.h"
#include "Rect.h"
#include "Sprite.h"
#include "Def.h"

#include <memory>

#include <d3d9.h>
#include <wrl/client.h>

class D3DGraphics
{
public:
	D3DGraphics( HWND hWnd );

	void BeginFrame()noexcept;
	void EndFrame();

	void PutPixel( int x, int y, Color c )noexcept;

	void DrawLine( int x1, int y1, int x2, int y2, Color color )noexcept;
	void DrawBezier( const Point& p0, const Point& p1, const Point& p2, Color color )noexcept;

	void DrawCircle( int cx, int cy, int radius, Color color )noexcept;
	void DrawDisc( int cx, int cy, int r, Color color )noexcept;

	void DrawChar( char c, int x, int y, const Font& font, Color color )noexcept;
	void DrawString( const char* string, int xoff, int yoff, const Font& font, Color color )noexcept;

	void DrawRectangle( const RectI& _rect, Color color )noexcept;
	void DrawEmptyRectangle( const RectI& _rect, Color color )noexcept;

	void DrawSprite( int x, int y, const Sprite& sprite )noexcept;

	void DrawTextBoxTopLeft( int _x, int _y, const Font& _font, Color _color )noexcept;
	void DrawTextBoxTopRight( int _x, int _y, const Font& _font, Color _color )noexcept;
	void DrawTextBoxBottomLeft( int _x, int _y, const Font& _font, Color _color )noexcept;
	void DrawTextBoxBottomRight( int _x, int _y, const Font& _font, Color _color )noexcept;
	
	RectI ClipToScreen( int x, int y, int width, int height )const noexcept;
private:
	Microsoft::WRL::ComPtr<IDirect3D9>			pDirect3D;
	Microsoft::WRL::ComPtr<IDirect3DDevice9>	pDevice;
	Microsoft::WRL::ComPtr<IDirect3DSurface9>	pBackBuffer;
	Sprite surface;
};