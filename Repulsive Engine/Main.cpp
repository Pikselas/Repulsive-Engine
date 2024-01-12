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

	std::vector<Image> images;
	images.reserve(15);

	for (int i = 1; i <= 7; ++i)
	{
		images.emplace_back("D:/blast/" + std::to_string(i) + ".png");
	}

	using namespace std::chrono_literals;

	auto animated_sprite1 = engine.CreateSprite(images , 350ms , 5);
	animated_sprite1.SetTransformation(DirectX::XMMatrixScaling(0.2f, 0.2f, 1.0f));
	animated_sprite1.SetPosition(DirectX::XMVectorSet(200, 300, 0, 1));

	auto animated_sprite2 = animated_sprite1;
	animated_sprite2.SetInfiniteRepetition();
	animated_sprite2.SetPosition(DirectX::XMVectorSet(400, 150, 0, 1));
	animated_sprite2.SetDuration(150ms);

	auto animated_sprite3 = animated_sprite1;
	animated_sprite3.SetRepeatCount(10);
	animated_sprite3.SetPosition(DirectX::XMVectorSet(600, 300, 0, 1));
	animated_sprite3.SetDuration(500ms);

	images.clear();

	InGUI gui(engine);
	gui.CaptureEvents(window.mouse);

	while (window.IsOpen())
	{
		engine.ClearFrame(window_renderer);
		gui.Draw();
		animated_sprite1.Draw(engine);
		animated_sprite2.Draw(engine);
		animated_sprite3.Draw(engine);
		
		window_renderer.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}

	return 0;
}