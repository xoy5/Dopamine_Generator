#pragma once
#include <math.h>

#include "Vec2.h"
#include "Surface.h"
#include "SpriteEffect.h"
#include "Colors.h"
#include "Ball.h"

#define ARRAY_EPSILON 40

class DrawOptimizer 
{
public:
	DrawOptimizer(int width, int height, Vei2 pos) 
		:
		s(width + ARRAY_EPSILON * 2, height + ARRAY_EPSILON * 2),
		pos(pos - Vei2{ARRAY_EPSILON, ARRAY_EPSILON})
	{}

	void Reset()
	{
		s.Reset();
	}

	void AddCircle(const Ball& ball) 
	{
		const float radius_float = ball.GetRadius();
		const int radius = int(radius_float);
		const float radiusSqrt = std::pow(radius_float, 2);

		for (int y = -radius; y <= radius; y++) {
			for (int x = -radius; x <= radius; x++) {
				if (x * x + y * y <= radiusSqrt) {
					Vei2 posSurf = Vei2(ball.GetPos()) + Vei2{ x, y } - pos;
					s.PutPixel(posSurf.x, posSurf.y, ball.GetColor());
				}
			}
		}
	}

	void Draw(Graphics& gfx)
	{
		gfx.DrawSprite(pos.x, pos.y, s, SpriteEffect::Copy{});
	}
private:
	Surface s;
	Vei2 pos;
};