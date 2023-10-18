#include"CoreEngine.h"
#include"StandardWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	constexpr unsigned int width = 800;
	constexpr unsigned int height = 600;

	CoreEngine engine(800 , 600);
	StandardWindow window("Repulsive Engine" , width , height);
	window.SetIcon(R"(C:\Users\Aritra Maji\Downloads\logo.ico)");

	auto renderer = engine.CreateRenderer(window);
	engine.SetRenderer(renderer);

	auto sprite = engine.CreateSprite(Image(R"(C:\Users\Aritra Maji\Downloads\_5bb8c433-5b5c-42f1-8a31-4fc3a6b81f2d.jpg)"));

	sprite.SetTransformation(DirectX::XMMatrixScaling(0.5f , 0.5f , 0.0f));
	sprite.SetPosition(DirectX::XMVectorSet(window.GetWidth() / 2, window.GetHeight() / 2, 0.0f, 1.0f));

	while (window.IsOpen())
	{
		engine.ClearFrame(renderer);
		sprite.Draw(engine);
		renderer.RenderFrame();

		Window::DispatchWindowEventsNonBlocking();
	}
	return 0;
}