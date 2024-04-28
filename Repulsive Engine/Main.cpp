#include"CoreEngine.h"
#include"StandardWindow.h"


class InGUI
{
private:
	CoreEngine& engine;
private:
	ImageSprite surface_window;
private:
	bool is_dragging = false;
	unsigned int offset_x = 0;
	unsigned int offset_y = 0;
public:
	InGUI(CoreEngine& engine) : engine(engine)
	{
		Image img(250, 250);
		img.Clear(ColorType{ 250 , 250 , 100 , 150 });
		surface_window = engine.CreateSprite(img);
		surface_window.SetPosition(DirectX::XMVectorSet(100, 100, 0, 1));
	}
public:
	void CaptureEvents(CustomWindow::Mouse& mouse)
	{

		mouse.OnMove = [&](auto&)
			{
				auto [x, y] = mouse.GetXY();

				OutputDebugStringA(std::string(is_dragging ? "dragging\n" : "not dragging\n").c_str());

				if (is_dragging)
				{

					surface_window.SetPosition(DirectX::XMVectorSet(x - offset_x, y - offset_y, 0, 1));
				}
			};
		mouse.OnLeftPress = [&](auto&)
			{
				auto [x, y] = mouse.GetXY();

				const int pos_x = DirectX::XMVectorGetX(surface_window.GetPosition());
				const int pos_y = DirectX::XMVectorGetY(surface_window.GetPosition());
				const int width = surface_window.GetWidth() / 2;
				const int height = surface_window.GetHeight() / 2;

				bool x_inside = (x >= pos_x - width && x <= (pos_x + width));
				bool y_inside = (y >= pos_y - height && y <= (pos_y + height));

				if (x_inside && y_inside)
				{
					is_dragging = true;
					offset_x = x - pos_x;
					offset_y = y - pos_y;
				}
			};
		mouse.OnLeftRelease = [&](auto&)
			{
				is_dragging = false;
			};
	}
public:
	void Draw()
	{
		surface_window.Draw(engine);
	}
};

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine;

	StandardWindow window("Repulsive Engine" , 800 , 600);
	window.SetIcon("Media/logo.ico");
	auto window_renderer = engine.CreateRenderer(window);

	engine.SetRenderDevice(window_renderer);

	InGUI gui(engine);
	gui.CaptureEvents(window.mouse);

	auto texture = engine.CreateTexture(Image("D:/ASSET/charmap.png"));

	auto character = engine.CreateSprite(texture , 47 , 47);
	character.SetTextureCoord(1 , 0);

	window.keyboard.EnableKeyRepeat();

	window.keyboard.OnKeyPress = [& , i = 0 ] (auto ev) mutable
	{
		if (ev.KEY_CODE == 'W')
		{
			i = (i + 1) % 3;
			character.SetTextureCoord(48 * i, 0);
		}
	};

	character.SetPosition(DirectX::XMVectorSet(100, 100, 0, 1));

	while (window.IsOpen())
	{
		engine.ClearFrame(window_renderer);

		character.Draw(engine);

		window_renderer.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}