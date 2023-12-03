#pragma once
#include <vector>
#include <chrono>
#include <optional>
#include "ImageSprite.h"

class AnimatedSprite : public Sprite
{
private:
	mutable std::vector<ImageSprite> frames;
	mutable unsigned int current_frame = 0;
private:
	std::chrono::milliseconds per_frame_duration;
	mutable std::optional<std::chrono::steady_clock::time_point> last_draw_time;
public:
	AnimatedSprite(const std::vector<ImageSprite>& frames, std::chrono::milliseconds duration)
		: frames(frames), per_frame_duration(duration / frames.size())
	{}
public:
	void SetDuration(std::chrono::milliseconds duration)
	{
		per_frame_duration = duration / frames.size();
	}
private:
	unsigned int GetFrameIndex() const
	{
		if (last_draw_time)
		{
			auto now = std::chrono::steady_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - *last_draw_time);
			if (elapsed >= per_frame_duration)
			{
				last_draw_time = now;
				current_frame += elapsed.count() / per_frame_duration.count();
				if (current_frame >= frames.size())
				{
					current_frame = 0;
				}
			}
		}
		else
		{
			last_draw_time = std::chrono::steady_clock::now();
		}
		return current_frame;
	}
public:
	void Restart()
	{
		current_frame = 0;
		last_draw_time = std::nullopt;
	}
public:
	void DrawNonTransformed(RenderCommandEngine& engine) const override
	{
		auto current_frame = GetFrameIndex();
		frames[current_frame].Draw(engine);
	}
	void Draw(RenderCommandEngine& engine) const override
	{
		auto current_frame = GetFrameIndex();
		frames[current_frame].SetTransformation(GetTransformation());
		frames[current_frame].Draw(engine);
	}
};