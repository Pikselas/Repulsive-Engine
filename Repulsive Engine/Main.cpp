#include"StandardWindow.h"

#include"CoreEngine.h"
#include"ImageSprite.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window;
	CoreEngine engine(window);
	
	//Sprite sprite(engine.GetGraphicsDevice(), {0 , 400 , 0 , 400} , window.GetWidth() , window.GetHeight());

	ImageSprite sprite(engine.GetGraphicsDevice(), Image(R"(C:\Users\Aritra Maji\Downloads\OIG-removebg-preview.png)"));

	auto sprite2 = sprite;

	sprite2.transformation = DirectX::XMMatrixTranslation(400.0f, 200.0f, 0.0f);

	window.mouse.OnMove = [&](const auto& event)
	{
		const auto [x, y] = event.mouse.GetXY();
		//const float x_ = (x / ((float)window.GetWidth() / 2)) - 1.0f;
		//const float y_ = (y / ((float)window.GetHeight() / 2)) - 1.0f;
		//const float aspect_ratio = (float)window.GetWidth() / (float)window.GetHeight();
		//const float x_ = ((float)x / (float)window.GetWidth()) - 0.5;
		//const float y_ = ((float)y / (float)window.GetHeight()) - 0.5;
		//sprite.transformation = DirectX::XMMatrixTranslation(x , 600 - y ,0.0f);
	};

	//sprite.transformation = DirectX::XMMatrixTranslation(400.0f, 200.0f, 0.0f);

	int d_x = 0;
	int d_y = 0;

	window.keyboard.EnableKeyRepeat();
	window.keyboard.OnKeyPress = [&](const auto& event)
	{
		if (event.KEY_CODE == 'W')
		{
			d_y += 5;
		}
		if (event.KEY_CODE == 'S')
		{
			d_y -= 5;
		}
		if (event.KEY_CODE == 'A')
		{
			d_x -= 5;
		}
		if (event.KEY_CODE == 'D')
		{
			d_x += 5;
		}

		sprite.transformation = DirectX::XMMatrixTranslation(d_x, d_y, 0.0f);

	};

	while (window.IsOpen())
	{
		engine.ClearFrame();
		engine.SetTransformation(sprite.transformation);
		sprite.Draw(engine.GetDeviceContext());
		engine.SetTransformation(sprite2.transformation);
		sprite2.Draw(engine.GetDeviceContext());
		engine.RenderFrame();
		Window::DispatchWindowEventsNonBlocking();
	}
	return 0;
}