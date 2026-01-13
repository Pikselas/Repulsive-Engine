#include"CoreEngine.h"
#include"StandardWindow.h"
#include <fstream>
#include <array>
#include <random>

#include "SpatialHashGrid.h"
#include "Object3DRenderContext.h"
#include "ImageSpriteRenderContext.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;

	char buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, 100);
	std::filesystem::path path = buffer;
	auto program_dir = path.parent_path();

	RenderAction::Object3DRenderContext o3d_ctx(engine.GetDevice(), program_dir);
	EngineAdapter::Object3DRenderingAdapter o3d_adapter(engine, o3d_ctx);

	RenderAction::ImageSpriteRenderContext ims_ctx(engine.GetDevice(), program_dir);
	EngineAdapter::ImageSpriteRenderingAdapter ims_adapter(engine, ims_ctx);

	StandardWindow window("Repulsive Engine", 800, 600);
	window.SetIcon("Media/logo.ico");
	auto window_renderer = engine.CreateRenderer(window);

	engine.SetRenderDevice(window_renderer);

	o3d_ctx.SetSurfaceSize(engine.GetDeviceContext(), window_renderer.GetWidth(), window_renderer.GetHeight());
	ims_ctx.SetSurfaceSize(engine.GetDeviceContext(), window_renderer.GetWidth(), window_renderer.GetHeight());

	Cube cube(engine.GetDevice());
	cube.SetPosition(0.5f, -0.5f, 7.0f);

	//while (window.IsOpen())
	//{
	//	engine.ClearFrame(window_renderer);	
	//	auto dist = std::chrono::duration<float>(std::chrono::steady_clock::now() - last_time);
	//	cube.RotateFixedPoint(0.0f , dist.count(), dist.count());
	//	cube.Draw(o3d_adapter);
	//	window_renderer.RenderFrame();
	//	Window::DispatchWindowEventsNonBlocking();
	//}
	
	Image scroll_window(400 , 350);
	scroll_window.Clear({ .b = 100 , .g = 50 , .r = 150 , .a = 100});
	auto scroll_sp = engine.CreateSprite(scroll_window);

	Image button{ 100 , 50 };
	button.Clear({ .b = 150 , .g = 100 , .r = 50 , .a = 150 });
	auto bsp = engine.CreateSprite(button);
	auto bsp2 = bsp;

	bsp.SetPosition(DirectX::XMVectorSet(150, 250, 1, 1));
	bsp2.SetPosition(DirectX::XMVectorSet(550, 100, 1, 1));
	scroll_sp.SetPosition(DirectX::XMVectorSet(350, 250, 1, 1));

	auto stencil_buffer = engine.CreateStencilBuffer(800, 600);

	auto sheet_pos = DirectX::XMVectorZero();

	window.mouse.OnWheel = [&](CustomWindow& me) 
		{
			int offset = me.mouse.GetWheelDelta();
			auto pos = DirectX::XMVectorSet(0, offset * 5, 0, 0);

			bsp.SetPosition(DirectX::XMVectorAdd(bsp.GetPosition(), pos));
			bsp2.SetPosition(DirectX::XMVectorAdd(bsp2.GetPosition(), pos));
		};
	
	std::chrono::steady_clock::time_point last_time = std::chrono::steady_clock::now();

	RenderCommandEngine& cmd_engine = ims_adapter;

	while (window.IsOpen())
	{
		engine.SetRenderDevice(window_renderer);
		engine.SetStencilBuffer(stencil_buffer);

		engine.ClearFrame(window_renderer);
		cmd_engine.ClearStencilBuffer(stencil_buffer);
		
		// Drawing ImageSprite
		engine.SetRenderContext(ims_ctx);
		scroll_sp.Draw(cmd_engine);

		cmd_engine.BeginStencilClipping(1);
		
		bsp.Draw(cmd_engine);
		bsp2.Draw(cmd_engine);
		
		// Drawing Object3D
		engine.SetRenderContext(o3d_ctx);
		auto dist = std::chrono::duration<float>(std::chrono::steady_clock::now() - last_time);
		cube.RotateFixedPoint(0.0f , dist.count(), dist.count());
		cube.Draw(o3d_adapter);
		
		cmd_engine.EndStencilClipping(1);

		window_renderer.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}