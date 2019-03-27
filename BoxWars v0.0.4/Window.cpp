#include "Window.h"
#include "Constants.h"
#include "Resource.h"


Window::Window()
{
	auto hInst = GetModuleHandle( nullptr );
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ),CS_CLASSDC,WindowProcSetup,0,0,
					  GetModuleHandle( NULL ),NULL,NULL,NULL,NULL,
					  "Chili DirectX Framework Window",NULL };
	wc.hIconSm = ( HICON )LoadImage( hInst, MAKEINTRESOURCE( IDI_APPICON16 ), IMAGE_ICON, 16, 16, 0 );
	wc.hIcon = ( HICON )LoadImage( hInst, MAKEINTRESOURCE( IDI_APPICON32 ), IMAGE_ICON, 32, 32, 0 );
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	RegisterClassEx( &wc );

	//rectangular framework window settings
	int x = 10, y = 30;
	RECT wr = { x, y, ScreenWidth + x, ScreenHeight + y };
	x = wr.left;
	y = wr.top;
	const int width = wr.right - wr.left;
	const int height = wr.bottom - wr.top;
	//.............................................................................................................//

	AdjustWindowRect( &wr, WS_OVERLAPPEDWINDOW, FALSE );
	handle = CreateWindowW( L"Chili DirectX Framework Window", L"Chili DirectX Framework",
		WS_OVERLAPPEDWINDOW, x, y, width, height,
		nullptr, nullptr, wc.hInstance, this );

	ShowWindow( handle, SW_SHOWDEFAULT );
	UpdateWindow( handle );
}

Window::operator HWND()const noexcept
{
	return handle;
}
Window::operator KeyboardServer&( )noexcept
{
	return kServ;
}
Window::operator MouseServer&( )noexcept
{
	return mServ;
}

Window::~Window()
{
	UnregisterClass( "Chili DirectX Framework Window", GetModuleHandle( nullptr ) );
}

void Window::Kill()const noexcept
{
	PostQuitMessage( 0 );
}
bool Window::ProcessMessages()noexcept
{
	MSG msg{};
	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		if( msg.message == WM_QUIT )
		{
			return true;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return false;
}

LRESULT CALLBACK Window::WindowProcSetup( HWND _handle, UINT _message, WPARAM _wparam, LPARAM _lparam )
{
	if( _message == WM_NCCREATE )
	{
		Window* pWindow = 
			reinterpret_cast< Window* >( reinterpret_cast< LPCREATESTRUCT >( _lparam )->lpCreateParams );

		SetWindowLongPtr( _handle, GWLP_USERDATA, reinterpret_cast< LONG_PTR >( pWindow ) );
		SetWindowLongPtr( _handle, GWLP_WNDPROC, reinterpret_cast< LONG_PTR >( &Window::WindowProcThunk ) );
	}

	return DefWindowProc( _handle, _message, _wparam, _lparam );
}
LRESULT CALLBACK Window::WindowProcThunk( HWND _handle, UINT _message, WPARAM _wparam, LPARAM _lparam )
{
	Window& window = *( reinterpret_cast< Window* >( GetWindowLongPtr( _handle, GWLP_USERDATA ) ) );
	return window.WindowProc( _message, _wparam, _lparam );
}
LRESULT Window::WindowProc( UINT _message, WPARAM _wparam, LPARAM _lparam )noexcept
{
	switch( _message )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

			// ************ KEYBOARD MESSAGES ************ //
		case WM_KEYDOWN:
		{
			const auto wasPressed = ( ( int( _wparam ) >> 29 ) & 1 ) == 1;
			if( !wasPressed )
				kServ.OnKeyPress( char( _wparam ) );
			else
			{
				int a = 0;
			}
			break;
		}
		case WM_KEYUP:
		{
			const auto wasPressed = ( ( int( _wparam ) >> 29 ) & 1 ) == 1;
			if( !wasPressed )
				kServ.OnKeyRelease( char( _wparam ) );
			break;
		}
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
		case WM_MOUSEMOVE:
		{
			int x = ( short )LOWORD( _lparam );
			int y = ( short )HIWORD( _lparam );

			if( x > 0 && x < ScreenWidth && y > 0 && y < ScreenHeight )
			{
				mServ.OnMouseMove( x, y );
				if( !mServ.IsInWindow() )
				{
					SetCapture( handle );
					mServ.OnMouseEnter();
				}
			}
			else
			{
				if( _wparam & ( MK_LBUTTON | MK_RBUTTON ) )
				{
					x = max( 0, x );
					x = min( ScreenWidth - 1, x );
					y = max( 0, y );
					y = min( ScreenHeight - 1, y );
					mServ.OnMouseMove( x, y );
				}
				else
				{
					ReleaseCapture();
					mServ.OnMouseLeave();
					mServ.OnLeftReleased();
					mServ.OnRightReleased();
				}
			}
		}
		break;
		case WM_LBUTTONDOWN:
			mServ.OnLeftPressed();
			break;
		case WM_RBUTTONDOWN:
			mServ.OnRightPressed();
			break;
		case WM_LBUTTONUP:
			mServ.OnLeftReleased();
			break;
		case WM_RBUTTONUP:
			mServ.OnRightReleased();
			break;
			// ************ END MOUSE MESSAGES ************ //
	}

	return DefWindowProc( handle, _message, _wparam, _lparam );
}
