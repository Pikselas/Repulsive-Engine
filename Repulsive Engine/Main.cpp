#include"CoreEngine.h"
#include"StandardWindow.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;

	StandardWindow window("Repulsive Engine" , 800 , 600);
	window.SetIcon("Media/logo.ico");
	auto window_renderer = engine.CreateRenderer(window);

	engine.SetRenderDevice(window_renderer);

	std::vector<Image> images;
	images.reserve(15);

	for (int i = 1; i <= 15; ++i)
	{
		images.emplace_back(R"(C:\Users\Aritra Maji\Downloads\Blue Butterfly Animation\Blue Butterfly Animation\butterfly blue 1200x1160\butterfly blue animation )" + std::to_string(i) + " 1200.png");
	}

	using namespace std::chrono_literals;

	auto animated_sprite1 = engine.CreateSprite(images , 350ms);
	animated_sprite1.SetTransformation(DirectX::XMMatrixScaling(0.2f, 0.2f, 1.0f));
	animated_sprite1.SetPosition(DirectX::XMVectorSet(200, 300, 0, 1));

	auto animated_sprite2 = animated_sprite1;
	animated_sprite2.SetPosition(DirectX::XMVectorSet(400, 150, 0, 1));
	animated_sprite2.SetDuration(150ms);

	auto animated_sprite3 = animated_sprite1;
	animated_sprite3.SetPosition(DirectX::XMVectorSet(600, 300, 0, 1));
	animated_sprite3.SetDuration(500ms);

	images.clear();

	while (window.IsOpen())
	{
		engine.ClearFrame(window_renderer);
		
		animated_sprite1.Draw(engine);
		animated_sprite2.Draw(engine);
		animated_sprite3.Draw(engine);
		
		window_renderer.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}