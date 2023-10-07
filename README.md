# Repulsive-Engine
## Minimal 2D graphics engine

```C++
#include"CoreEngine.h"
#include"StandardWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  StandardWindow window("Repulsive Engine");
  window.SetIcon(R"(logo.ico)");
  CoreEngine engine(window);
  
  auto sprite = engine.CreateSprite(Image("icon-image.png"));
  sprite.SetPosition(DirectX::XMVectorSet(window.GetWidth() / 2, window.GetHeight() / 2, 0.0f, 1.0f));
  
  while (window.IsOpen())
  {
    engine.ClearFrame();
    sprite.Draw(engine);
    engine.RenderFrame();
    Window::DispatchWindowEventsNonBlocking();
  }
return 0;
}
```
