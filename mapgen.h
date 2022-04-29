#include <cstdint>
#include <windows.h>

#define internal static;
#define local_persist static;
#define global_variable static;

internal const uint16_t ScreenWidth = 800;
internal const uint16_t ScreenHeight = 600;
internal bool Running = true;

LRESULT CALLBACK WindowProc(
  HWND Window,
  UINT Message,
  WPARAM wParam,
  LPARAM lParam
);

internal void RenderPoorly(HDC DeviceContext);
internal void ClearToBlack(HDC DeviceContext, PAINTSTRUCT *Paint);
internal void DrawWindow(HDC DeviceContext, int X, int Y, int Width, int Height);
