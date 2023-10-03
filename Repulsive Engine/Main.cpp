#include"StandardWindow.h"

#include"CoreEngine.h"
#include"ImageSprite.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window;
	CoreEngine engine(window);
	
	//Sprite sprite(engine.GetGraphicsDevice(), {0 , 400 , 0 , 400} , window.GetWidth() , window.GetHeight());

	ImageSprite sprite(engine.GetGraphicsDevice(), Image(R"(C:\Users\Aritra Maji\Downloads\OIG-removebg-preview.png)"));

	sprite.SetTransformation(DirectX::XMMatrixTranslation(400,300, 0.0f));

	while (window.IsOpen())
	{
		engine.ClearFrame();
		engine.Draw(sprite);
		engine.RenderFrame();

		Window::DispatchWindowEventsNonBlocking();
	}
	return 0;
}