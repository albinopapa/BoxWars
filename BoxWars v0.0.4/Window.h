#pragma once

#include "Keyboard.h"
#include "Mouse.h"

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


class Window
{
public:
	Window();
	~Window();

	operator HWND()const noexcept;
	operator KeyboardServer&( )noexcept;
	operator MouseServer&( )noexcept;

	void Kill()const noexcept;
	bool ProcessMessages()noexcept;

	static LRESULT CALLBACK WindowProcSetup( HWND _handle, UINT _message, WPARAM _wparam, LPARAM _lparam );
	static LRESULT CALLBACK WindowProcThunk( HWND _handle, UINT _message, WPARAM _wparam, LPARAM _lparam );
	LRESULT WindowProc( UINT _message, WPARAM _wparam, LPARAM _lparam )noexcept;


private:
	HWND handle;
	KeyboardServer kServ;
	MouseServer mServ;
	bool isClosed = false;
};

