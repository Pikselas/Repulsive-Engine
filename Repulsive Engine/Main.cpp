#include"CoreEngine.h"
#include"StandardWindow.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;

	auto ship	= engine.CreateSprite(Image("Media/ship.png"));
	auto tree_1 = engine.CreateSprite(Image("Media/tree_1.png"));
	auto tree_2 = engine.CreateSprite(Image("Media/tree_2.png"));
	auto tree_3 = engine.CreateSprite(Image("Media/tree_3.png"));

	Image image_target(800, 600);
	StandardWindow window("Repulsive Engine" , 800 , 600);
	window.SetIcon("Media/logo.ico");

	auto image_renderer	 = engine.CreateRenderer(image_target);
	auto window_renderer = engine.CreateRenderer(window);

	ship.SetPosition(DirectX::XMVectorSet(400 , 300 , 0 , 1));
	tree_1.SetPosition(DirectX::XMVectorSet(100 , 100 , 0 , 1));
	tree_2.SetPosition(DirectX::XMVectorSet(700 , 100 , 0 , 1));
	tree_3.SetPosition(DirectX::XMVectorSet(400 , 500 , 0 , 1));

	engine.SetRenderer(image_renderer);

	ship.Draw(engine);
	tree_1.Draw(engine);
	tree_2.Draw(engine);
	tree_3.Draw(engine);

	window_renderer.CopyFrame(image_renderer);
	window_renderer.RenderFrame();

	image_target.Clear();
	image_renderer.RenderFrame();
	image_target.Save("Media/RENDERED.png");

	while (window.IsOpen())
	{
		Window::DispatchWindowEvents();
	}

	return 0;
}