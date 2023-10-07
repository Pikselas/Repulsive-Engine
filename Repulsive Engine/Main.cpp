#include"CoreEngine.h"
#include"StandardWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window("Repulsive Engine");
	window.SetIcon(R"(C:\Users\Aritra Maji\Downloads\logo.ico)");
	CoreEngine engine(window);

	auto sprite = engine.CreateSprite(Image(R"(C:\Users\Aritra Maji\Downloads\_5bb8c433-5b5c-42f1-8a31-4fc3a6b81f2d.jpg)"));

	sprite.SetTransformation(DirectX::XMMatrixScaling(0.5f , 0.5f , 0.0f));
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