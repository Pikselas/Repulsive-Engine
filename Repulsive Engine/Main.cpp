#include"CoreEngine.h"
#include"StandardWindow.h"
#include <fstream>
#include <array>
#include <random>

#include "SpatialHashGrid.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;

	StandardWindow window("Repulsive Engine" , 800 , 600);
	window.SetIcon("Media/logo.ico");
	auto window_renderer = engine.CreateRenderer(window);
	engine.SetRenderDevice(window_renderer);
	
	Image scroll_window(400 , 350);
	scroll_window.Clear({ .b = 100 , .g = 50 , .r = 150 });
	auto scroll_sp = engine.CreateSprite(scroll_window);

	Image button{ 100 , 50 };
	button.Clear({ .b = 150 , .g = 100 , .r = 50 });
	auto bsp = engine.CreateSprite(button);
	auto bsp2 = bsp;

	bsp.SetPosition(DirectX::XMVectorSet(150, 250, 1, 1));
	bsp2.SetPosition(DirectX::XMVectorSet(550, 100, 1, 1));
	scroll_sp.SetPosition(DirectX::XMVectorSet(350, 250, 1, 1));

	auto stencil_buffer = engine.CreateStencilBuffer(800, 600);
	engine.SetStencilBuffer(stencil_buffer);

	auto sheet_pos = DirectX::XMVectorZero();

	window.mouse.OnWheel = [&](CustomWindow& me) 
		{
			int offset = me.mouse.GetWheelDelta();
			auto pos = DirectX::XMVectorSet(0, offset * 5, 0, 0);

			bsp.SetPosition(DirectX::XMVectorAdd(bsp.GetPosition(), pos));
			bsp2.SetPosition(DirectX::XMVectorAdd(bsp2.GetPosition(), pos));
		};

	while (window.IsOpen())
	{
		engine.ClearFrame(window_renderer);
		engine.ClearStencilBuffer(stencil_buffer);
		scroll_sp.Draw(engine);
		
		engine.BeginStencilClipping(1);
		bsp.Draw(engine);
		bsp2.Draw(engine);
		engine.EndStencilClipping(1);
		
		window_renderer.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}