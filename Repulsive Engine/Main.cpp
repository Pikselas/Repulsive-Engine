#include"StandardWindow.h"

#include"CoreEngine.h"
#include"ImageSprite.h"

#include<queue>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	StandardWindow window("Repulsive Engine" , 1000 , 800);
	CoreEngine engine(window);
	
	//Sprite sprite(engine.GetGraphicsDevice(), {0 , 400 , 0 , 400} , window.GetWidth() , window.GetHeight());

	ImageSprite sprite(engine, Image(R"(C:\Users\Aritra Maji\Downloads\OIG-removebg-preview.png)"));
	ImageSprite sprite2(engine, Image(R"(C:\Users\Aritra Maji\Downloads\oig_dk.png)"));

	sprite.SetTransformation(DirectX::XMMatrixTranslation(200, 300, 0.0f));
	sprite2.SetTransformation(DirectX::XMMatrixTranslation(350, 350, 0.0f));

	struct Bullet
	{
		ImageSprite sprite;
		float vel_x, vel_y;
		float pos_x, pos_y;
		float angle_z;
	};

	std::queue<Bullet> bullets;

	ImageSprite bullet_sprite(engine, Image(R"(C:\Users\Aritra Maji\Downloads\Sperm-Donasyonu-removebg-preview_2.png)"));

	window.mouse.OnMove = [&](auto&)
	{
		auto [x, y] = window.mouse.GetXY();

		auto pos = DirectX::XMVectorSet(350, 350, 0.0f , 1.0f);
		auto pos2 = DirectX::XMVectorSet(x, y, 0.0f, 1.0f);

		auto diff = DirectX::XMVectorSubtract(pos2, pos);

		auto angle = std::atan2(DirectX::XMVectorGetY(diff), DirectX::XMVectorGetX(diff));

		sprite2.SetTransformation(DirectX::XMMatrixTranslation(120, 0, 0.0f) * DirectX::XMMatrixRotationZ(angle) * DirectX::XMMatrixTranslation(340 - 120, 350, 0.0f));

	};

	window.mouse.OnLeftPress = [&](auto&)
	{
		auto [x, y] = window.mouse.GetXY();

		auto pos = DirectX::XMVectorSet(350, 350, 0.0f, 1.0f);
		auto pos2 = DirectX::XMVectorSet(x, y, 0.0f, 1.0f);

		auto diff = DirectX::XMVectorSubtract(pos2, pos);

		auto angle = std::atan2(DirectX::XMVectorGetY(diff), DirectX::XMVectorGetX(diff));

		sprite2.SetTransformation(DirectX::XMMatrixTranslation(120, 0, 0.0f)* DirectX::XMMatrixRotationZ(angle)* DirectX::XMMatrixTranslation(340 - 120, 350, 0.0f));

		Bullet bullet{ bullet_sprite };

		bullet.pos_x = 350 - 120;
		bullet.pos_y = 350;

		diff = DirectX::XMVector2Normalize(diff);

		bullet.vel_x = DirectX::XMVectorGetX(diff) * 3.5f;
		bullet.vel_y = DirectX::XMVectorGetY(diff) * 3.5f;

		bullet.angle_z = angle;

		bullets.push(bullet);
	};

	while (window.IsOpen())
	{
		engine.ClearFrame();

		sprite2.Draw(engine);
		sprite.Draw(engine);

		int size = bullets.size();

		while (size--)
		{
			auto bullet = bullets.front();
			bullets.pop();

			bullet.pos_x += bullet.vel_x;
			bullet.pos_y += bullet.vel_y;

			auto Transform = DirectX::XMMatrixTranslation(180.0f, 0.0f, 0.0f) * DirectX::XMMatrixRotationZ(bullet.angle_z) * DirectX::XMMatrixTranslation(bullet.pos_x, bullet.pos_y , 0.0f);

			bullet.sprite.SetTransformation(Transform);
			bullet.sprite.Draw(engine);

			if (!(bullet.pos_x < 0 || bullet.pos_x > window.GetWidth() || bullet.pos_y < 0 || bullet.pos_y > window.GetHeight()))
			{
				bullets.push(bullet);
			}
		}

		engine.RenderFrame();

		Window::DispatchWindowEventsNonBlocking();
	}
	return 0;
}