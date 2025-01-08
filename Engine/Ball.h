#pragma once
#include "Graphics.h"
#include "Colors.h"
#include "Vec2.h"
#include "Xoy5Math.h"
#include "Rect.h"
#include <random>

class Ball
{
public:
	Ball(const Vec2& pos, const Vec2& vel, float radius, const Color& color, bool cooldown = false)
		:
		pos(pos),
		vel(vel),
		radius(radius),
		color(color),
		cooldown(cooldown)
	{}

	void Draw(Graphics& gfx) const
	{
		gfx.DrawCircle(Vei2(pos), radius, color);
	}

	void Update(float dt)
	{
		vel += xoy5::GRAVITATION * dt;
		pos += vel * dt;
	}

	
	bool BorderCollision(const Vec2& circlePos, float circleRadius) {
		bool collision = false;

		// fast way (collision false)
		const float halfLengthSquare = (circleRadius * 1.4142135f) / 2.0f; // r * sqrt(2) / 2

		if (GetRect().IsContainedBy(RectF::FromCenter(circlePos, halfLengthSquare, halfLengthSquare))) {
			collision = false;
		}
		// slow way (collision maybe)
		else {
			const Vec2 posInCircle = pos - circlePos;
			const Vec2 vecToPoint = posInCircle.GetNormalized();
			const float circleRadiusSqrt = std::pow(circleRadius, 2);
			const float ballRadiusSqrt = std::pow(radius, 2);

			for (Vec2 p = Vec2{ 0.0f, 0.0f }, pc = posInCircle; true; p += vecToPoint, pc += vecToPoint) {
				float lengthBallSqrt = std::pow(p.x, 2) + std::pow(p.y, 2);
				float lengthInCircleSqrt = std::pow(pc.x, 2) + std::pow(pc.y, 2);
				// (collision false)
				if (lengthBallSqrt > ballRadiusSqrt) {
					collision = false;
					break;
				}
				// (collision true)
				else if (lengthInCircleSqrt > circleRadiusSqrt) {
					collision = true;
					break;
				}
			}
		}

		if (cooldown == true && collision == true) {
			return false;
		}
		else if (cooldown == false && collision == true) {

			cooldown = true;

			std::mt19937 rng = std::mt19937(std::random_device{}());
			std::uniform_int_distribution<int> range100 = std::uniform_int_distribution<int>(-100, 100);
			const Vec2 vecToCircle = (circlePos - pos + Vec2(range100(rng), range100(rng))).Normalize();

			vel.x = vecToCircle.x * 400.0f;
			vel.y = vecToCircle.y * 400.0f;

			return true;
		}
		else {
			cooldown = false;
			return false;
		}
	}



	RectF GetRect() const {
		return RectF::FromCenter(pos, radius, radius);
	}
	Vec2 GetPos() const
	{
		return pos;
	}
	Vec2 GetVel() const
	{
		return vel;
	}
	Color GetColor() const
	{
		return color;
	}
	float GetRadius() const
	{
		return radius;
	}

private:
	Vec2 pos;
	Vec2 vel;
	bool cooldown;
	float radius;
	Color color;
	float MAXSPEED = 400.0f;
};