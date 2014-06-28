#pragma once

#include "Engine/System.h"
#include <vector>
#include <stdint.h>
#include <memory>

struct SDL_Renderer;
class GraphicsSystem;

class GeometryDrawer : public System
{
	struct LineSegment
	{
		float start_x, end_x;
		float start_y, end_y;

		float remaining_lifetime;

		uint32_t color_rgba;
	};


public:
	GeometryDrawer(shared_ptr<GraphicsSystem> graphics_system);
	~GeometryDrawer();

	virtual void step(float delta_time) override;

	void draw_line(float start_x, float start_y, float end_x, float end_y,
		uint32_t color_rgba = 0xFFFFFFFF, float duration_seconds = 0.0f);

private:
	void drawGeometry();

	uint64_t _graphics_callback_id;
	shared_ptr<GraphicsSystem> _graphics_system;
	SDL_Renderer* _renderer;
	std::vector<LineSegment> _lines;
};
