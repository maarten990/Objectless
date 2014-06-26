#include "GeometryDrawer.h"

#include <algorithm>
#include <SDL_render.h>

#include "External/SDL2_gfx/SDL2_gfxPrimitives.h"
#include "Engine/EntityManager.h"
#include "Engine/Component.h"
#include "Components.h"
#include "GraphicsSystem.h"


GeometryDrawer::GeometryDrawer(SDL_Renderer* renderer, GraphicsSystem* graphics_system)
	: _renderer(renderer)
	, _graphics_system(graphics_system)
{
	_graphics_callback_id = _graphics_system->registerPostRenderCallback(
		std::bind(&GeometryDrawer::drawGeometry, this));
}

GeometryDrawer::~GeometryDrawer()
{
	_graphics_system->unregisterPostRenderCallback(_graphics_callback_id);
}

void GeometryDrawer::step(float delta_time)
{
	if (_lines.empty()) {
		return;
	}
	for (LineSegment& line : _lines) {
		line.remaining_lifetime -= delta_time;
	}

	//Remove line segments whose lifetime has expired.
	auto erase_itr = std::remove_if(std::begin(_lines), std::end(_lines),
		[](const LineSegment& line) {return line.remaining_lifetime <= 0.0f; });
	if (erase_itr != std::end(_lines))
	{
		_lines.erase(erase_itr);
	}
}

void GeometryDrawer::draw_line(float start_x, float start_y, float end_x, float end_y,
	uint32_t color_rgba /*= 0xFFFFFFFF*/, float duration_seconds /*= 0.0f*/)
{
	LineSegment line{ start_x, end_x, start_y, end_y, duration_seconds, color_rgba, };
	line.color_rgba = color_rgba;
	_lines.push_back(line);
}

uint32_t rgbaToAgbr(uint32_t v)
{
	return ((v & 0xFF) << 24) | ((v & 0xFF00) << 8) | ((v & 0xFF0000) >> 8)
		| ((v & 0xFF000000) >> 24);
}

void GeometryDrawer::drawGeometry()
{
	for (const LineSegment& line : _lines) {
		//TODO figure out why this conversion is needed. According to the docs, SDL uses RGBA too.
		uint32_t color = rgbaToAgbr(line.color_rgba);
		aalineColor(_renderer, (int16_t)line.start_x, (int16_t)line.start_y, (int16_t)line.end_x,
			(int16_t)line.end_y, color);
	}
}
