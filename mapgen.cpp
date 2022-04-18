#include "mapgen.h"

int WINAPI WinMain(
  HINSTANCE Instance,
  HINSTANCE PrevInstance,
  LPSTR CommandLine,
  int ShowCommand
) {
  const char* Name = "Map Generator";

  WNDCLASS WindowClass = {};
  WindowClass.lpfnWndProc = WindowProc;
  WindowClass.hInstance = Instance;
  WindowClass.lpszClassName = Name;

  RegisterClass(&WindowClass);

  // CreateWindowEx takes a bunch of random crap so set some descriptive variables
  int PosX = CW_USEDEFAULT;
  int PosY = CW_USEDEFAULT;
  HWND ParentWindow = NULL;
  HMENU Menu = NULL;
  LPVOID AdditionalParams = NULL;

  HWND Window = CreateWindowEx(
    0,
    Name,
    Name,
    WS_OVERLAPPEDWINDOW,
    PosX,
    PosY,
    ScreenWidth,
    ScreenHeight,
    ParentWindow,
    Menu,
    Instance,
    AdditionalParams
  );

  if(Window == NULL) {
    return 0;
  }

  ShowWindow(Window, ShowCommand);

  while(Running) {
    MSG Message = {};
    while(GetMessage(&Message, NULL, 0, 0) > 0) {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }

    UpdateAndRender();
  }

  return 0;
}

internal void UpdateAndRender() {

}

LRESULT CALLBACK WindowProc(
  HWND Window,
  UINT Message,
  WPARAM wParam,
  LPARAM lParam
) {
  switch(Message) {
    case WM_DESTROY: {
      Running = false;
      PostQuitMessage(0);
      return 0;
    }
  }

  return DefWindowProc(Window, Message, wParam, lParam);
}
