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
    WS_OVERLAPPED|WS_SYSMENU,
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
  }

  return 0;
}

LRESULT CALLBACK WindowProc(
  HWND Window,
  UINT Message,
  WPARAM wParam,
  LPARAM lParam
) {
  switch(Message) {
    case WM_CLOSE: {
      Running = false;
      PostQuitMessage(0);
      return 0;
    }
    case WM_PAINT: {
      PAINTSTRUCT Paint;
      HDC DeviceContext = BeginPaint(Window, &Paint);
      // RenderPoorly(DeviceContext);
      // ClearToBlack(DeviceContext, &Paint);
      DrawWindow(
	DeviceContext,
	Paint.rcPaint.top,
	Paint.rcPaint.left,
	Paint.rcPaint.right,
	Paint.rcPaint.bottom
      );
      EndPaint(Window, &Paint);
      break;
    }
  }

  return DefWindowProc(Window, Message, wParam, lParam);
}

// Interesting but useless, so don't use.
internal void RenderPoorly(HDC DeviceContext) {
  int RedIndex = 0;
  for(int X = 0; X < ScreenWidth; ++X) {
    for(int Y = 0; Y < ScreenHeight; ++Y) {
      if(RedIndex > 255) {
	RedIndex = 0;
      }
      SetPixel(DeviceContext, X, Y, RGB(RedIndex, 0, 0));
      ++RedIndex;
    }
  }
}

internal void ClearToBlack(HDC DeviceContext, PAINTSTRUCT *Paint) {
  int X = Paint->rcPaint.left;
  int Y = Paint->rcPaint.top;
  int Width = Paint->rcPaint.right;
  int Height = Paint->rcPaint.bottom;
  PatBlt(DeviceContext, X, Y, Width, Height, BLACKNESS);
}

global_variable BITMAPINFO BitmapInfo;
global_variable void *BitmapMemory;
internal void DrawWindow(HDC DeviceContext, int X, int Y, int Width, int Height) {
  if(BitmapMemory) {
    VirtualFree(BitmapMemory, 0, MEM_RELEASE);
  }

  BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
  BitmapInfo.bmiHeader.biWidth = Width;
  // Height is negative to indicate it's a top-down rather than bottom-up bitmap
  BitmapInfo.bmiHeader.biHeight = -Height;
  BitmapInfo.bmiHeader.biPlanes = 1;
  BitmapInfo.bmiHeader.biBitCount = 32;
  BitmapInfo.bmiHeader.biCompression = BI_RGB;

  int BytesPerPixel = 4;
  int BitmapMemorySize = Width * Height * BytesPerPixel;
  BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

  int Pitch = Width * BytesPerPixel;
  uint8_t *Row = (uint8_t *)BitmapMemory;

  for(int BitY = 0; BitY < Height; ++BitY) {
    uint8_t *Byte = (uint8_t *)Row;
    for(int BitX = 0; BitX < Width; ++BitX) {
      *Byte = (uint8_t)BitX*BitY;
      ++Byte;
      *Byte = (uint8_t)BitX;
      ++Byte;
      *Byte = (uint8_t)BitY;
      ++Byte;
      *Byte = 0;
      ++Byte;
    }
    Row += Pitch;
  }

  StretchDIBits(
    DeviceContext,
    X, Y, Width, Height,
    X, Y, Width, Height,
    BitmapMemory,
    &BitmapInfo,
    DIB_RGB_COLORS,
    SRCCOPY
  );
}
