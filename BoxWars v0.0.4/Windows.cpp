
#include <Windows.h>
#include <wchar.h>
#include "Game.h"
#include "resource.h"
#include "Mouse.h"
#include "D3DGraphics.h"

static KeyboardServer kServ;
static MouseServer mServ;

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

		// ************ KEYBOARD MESSAGES ************ //
		case WM_KEYDOWN:
		{
			const auto count = char( wParam & 0xF );
			const auto wasPressed = ( ( int( wParam ) >> 29 ) & 1 ) == 1;
			if(count <= 1)
				kServ.OnKeyPress( char( wParam ) );
			break;
		}
		case WM_KEYUP:
		{
			const auto count = char( wParam & 0xF );
			if(count <= 1)
				kServ.OnKeyRelease( char( wParam ) );
			break;
		}
		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
		case WM_MOUSEMOVE:
			{
				
				int x = (short)LOWORD( lParam );
				int y = (short)HIWORD( lParam );
				if( x > 0 && x < 800 && y > 0 && y < 600 )
				{
					mServ.OnMouseMove( x,y );
					if( !mServ.IsInWindow() )
					{
						SetCapture( hWnd );
						mServ.OnMouseEnter();
					}
				}
				else
				{
					if( wParam & (MK_LBUTTON | MK_RBUTTON) )
					{
						x = max( 0,x );
						x = min( 799,x );
						y = max( 0,y );
						y = min( 599,y );
						mServ.OnMouseMove( x,y );
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

    return DefWindowProc( hWnd, msg, wParam, lParam );
}
int WINAPI wWinMain( HINSTANCE hInst,HINSTANCE,LPWSTR,INT )
{
	WNDCLASSEX wc = { sizeof( WNDCLASSEX ),CS_CLASSDC,MsgProc,0,0,
                      GetModuleHandle( NULL ),NULL,NULL,NULL,NULL,
                      "Chili DirectX Framework Window",NULL };
    wc.hIconSm = (HICON)LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON16 ),IMAGE_ICON,16,16,0 );
	wc.hIcon   = (HICON)LoadImage( hInst,MAKEINTRESOURCE( IDI_APPICON32 ),IMAGE_ICON,32,32,0 );
	wc.hCursor = LoadCursor( NULL,IDC_ARROW );
    RegisterClassEx( &wc );
	
	//rectangular framework window settings
	RECT wr;
	//horizontal distance from top left hand corner of the screen
	wr.left = 10;
	//right end of the window
	wr.right = TOTALSCREENWIDTH + wr.left;
	//vertical distance from top left hand corner of the screen
	wr.top = 30;
	//lower end of the window
	wr.bottom = TOTALSCREENHEIGHT + wr.top;
	//.............................................................................................................//

	AdjustWindowRect( &wr,WS_OVERLAPPEDWINDOW,FALSE );
    HWND hWnd = CreateWindowW( L"Chili DirectX Framework Window",L"Chili DirectX Framework",
                              WS_OVERLAPPEDWINDOW,wr.left,wr.top,wr.right-wr.left,wr.bottom-wr.top,
                              NULL,NULL,wc.hInstance,NULL );

    ShowWindow( hWnd,SW_SHOWDEFAULT );
    UpdateWindow( hWnd );

	Game theGame( hWnd,kServ,mServ );
	
    MSG msg;
    ZeroMemory( &msg,sizeof( msg ) );
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage( &msg,NULL,0,0,PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
		{
			theGame.Go();
		}
    }

    UnregisterClass( "Chili DirectX Framework Window",wc.hInstance );
    return 0;
}