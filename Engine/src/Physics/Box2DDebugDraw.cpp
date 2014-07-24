#include <stdint.h>
#include <vector>
#include <Box2D/Common/b2Math.h>
#include <Box2D/Common/b2Draw.h>
#include <Box2D/Dynamics/b2Body.h>
#include "External/SDL2_gfx/SDL2_gfxPrimitives.h"
#include "Engine/Physics/Box2DDebugDraw.h"
#include "GraphicsSystem.h"


uint8_t toHex(float value01)
{
	return (uint8_t)(value01 == 1.0f ? 255.0f : value01 * 256.0f);
}

uint32_t toColor32(b2Color color)
{
	uint8_t red = toHex(color.r);
	uint8_t green = toHex(color.g);
	uint8_t blue = toHex(color.b);
	uint8_t alpha = toHex(color.a);
	//uint32_t rgba = (red << 24) | (green << 16) | (blue << 8) | alpha;
	//todo figure out why this needs to be in the wrong order
	//SDL functions which take a uint32 as color parameter seems to expect ABGR,
	//even though a comment in the header say RGBA
	uint32_t rgba = red | (green << 8) | (blue << 16) | (alpha << 24);
	return rgba;
}

Box2DDebugDraw::Box2DDebugDraw(SDL_Renderer* renderer, GraphicsSystem& graphics, b2World& world)
	: _renderer(renderer)
	, world(world)
	, graphics(graphics)
{
	post_render_callback_id = graphics.registerPostRenderCallback(std::bind(&Box2DDebugDraw::draw, this));
}

Box2DDebugDraw::~Box2DDebugDraw()
{
	graphics.unregisterPostRenderCallback(post_render_callback_id);
}

void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	drawVertices(vertices, vertexCount, toColor32(color), polygonColor);
}

void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	drawVertices(vertices, vertexCount, toColor32(color), filledPolygonColor);
}

void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	aacircleColor(_renderer, (int16_t)center.x, (int16_t)center.y, (int16_t)radius, toColor32(color));
}

void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& /*axis*/, const b2Color& color)
{
	//todo figure out what the 'axis' parameter represents
	filledCircleColor(_renderer, (int16_t)center.x, (int16_t)center.y, (int16_t)radius, toColor32(color));
}

void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	aalineColor(_renderer, (int16_t)p1.x, (int16_t)p1.y, (int16_t)p2.x, (int16_t)p2.y, toColor32(color));
}

void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
{
	b2Vec2 up = b2Mul(xf, b2Vec2(0.0f, 50.0f));
	b2Vec2 right = b2Mul(xf, b2Vec2(50.0f, 0.0f));
	aalineRGBA(_renderer, (int16_t)xf.p.x, (int16_t)xf.p.y, (int16_t)up.x, (int16_t)up.y, 0, 255, 0, 255);
	aalineRGBA(_renderer, (int16_t)xf.p.x, (int16_t)xf.p.y, (int16_t)right.x, (int16_t)right.y, 255, 0, 0, 255);
}

void Box2DDebugDraw::drawVertices(const b2Vec2* vertices, uint32_t vertex_count, uint32_t color,
	const DrawFunction& draw_function)
{
	std::vector<int16_t> x(vertex_count);
	std::vector<int16_t> y(vertex_count);
	for (uint32_t i = 0; i < vertex_count; ++i)
	{
		x[i] = (int16_t)vertices[i].x;
		y[i] = (int16_t)vertices[i].y;
	}

	draw_function(_renderer, x.data(), y.data(), vertex_count, color);
}

void Box2DDebugDraw::draw()
{
	int mouse_x, mouse_y;
	uint32_t buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
	if (SDL_BUTTON(buttons) == SDL_BUTTON_LEFT)
	{
		b2Body& body = world.GetBodyList()[0];
		b2Vec2 dir = b2Vec2((float)mouse_x, (float)mouse_y) - body.GetPosition();
		dir *= 100.0f;
		b2Rot rot = body.GetTransform().q;
		body.ApplyForceToCenter(dir, true);
		aalineColor(_renderer, (int16_t)body.GetPosition().x, (int16_t)body.GetPosition().y, (int16_t)mouse_x, (int16_t)mouse_y, 0xFFFFFFFF);
	}
	world.DrawDebugData();
}
