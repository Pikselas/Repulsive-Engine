#include"CoreEngine.h"
#include"StandardWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;
	StandardWindow window("Repulsive Engine");
	window.SetIcon(R"(C:\Users\Aritra Maji\Downloads\logo.ico)");
	Image image(window.GetWidth(), window.GetHeight());

	auto renderer_window = engine.CreateRenderer(window);
	auto renderer_memory = engine.CreateRenderer(image);

	//Image prt(R"(C:\Users\Aritra Maji\Downloads\_5bb8c433-5b5c-42f1-8a31-4fc3a6b81f2d.jpg)");
	//prt.Save("media/shot.jpg");

	auto sprite = engine.CreateSprite(Image(R"(C:\Users\Aritra Maji\Downloads\_5bb8c433-5b5c-42f1-8a31-4fc3a6b81f2d.jpg)"));

	sprite.SetTransformation(DirectX::XMMatrixScaling(0.5f , 0.5f , 0.0f));
	sprite.SetPosition(DirectX::XMVectorSet(window.GetWidth() / 2, window.GetHeight() / 2, 0.0f, 1.0f));

	engine.SetRenderer(renderer_memory);
	sprite.Draw(engine);

	renderer_memory.RenderFrame();

	image.Save("media/file.png");

	//renderer_window.CopyFrame(renderer_memory);
	//renderer_window.RenderFrame();

	while (window.IsOpen())
	{
		Window::DispatchWindowEventsNonBlocking();
	}
	return 0;
}