#pragma once

#include <functional>
#include <stdint.h>
#include <Box2D/Common/B2Draw.h>
#include <Box2D/Dynamics/b2World.h>

struct SDL_Renderer;
class GraphicsSystem;

class Box2DDebugDraw : public b2Draw
{
public:
	Box2DDebugDraw(SDL_Renderer* renderer, GraphicsSystem& graphics, b2World& world);

	virtual ~Box2DDebugDraw();

	Box2DDebugDraw& operator=(const Box2DDebugDraw& other) = delete;

	virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	/// Draw a solid closed polygon provided in CCW order.
	virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

	/// Draw a circle.
	virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

	/// Draw a solid circle.
	virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

	/// Draw a line segment.
	virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	/// Draw a transform. Choose your own length scale.
	/// @param xf a transform.
	virtual void DrawTransform(const b2Transform& xf) override;

private:
	//renderer, vertex x array, vertex y array, num vertices, color
	typedef std::function<int(SDL_Renderer*, const int16_t*, const int16_t*, int, uint32_t)> DrawFunction;

	void drawVertices(const b2Vec2* vertices, uint32_t vertex_count, uint32_t color,
		const DrawFunction& draw_function);
		//int (drawFunction*) (SDL_Renderer* /*renderer*/, const int16_t * /*vx*/, const int16_t * /*vy*/, int n, uint32_t /*color*/));

	void draw();

	uint64_t post_render_callback_id;

	SDL_Renderer* _renderer;
	b2World& world;
	GraphicsSystem& graphics;
};