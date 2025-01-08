#pragma once
#include "Graphics.h"
#include "Vec2.h"
#include "Colors.h"
#include <algorithm>
#include "Xoy5Math.h"

class Laser
{
public:
	Laser(const Vec2& pos, float radius = 100.0f, float degrees = 0.0f, float angle = 20.0f, float speed = 50.0f, float duration = 2.0f, float cooldown = 2.0f)
		:
		pos(pos),
		radius(radius),
		degrees(degrees),
		angle(angle),
		speed(speed),
		duration(duration),
		cooldown(cooldown)
	{}

	void Draw(Graphics& gfx) const
	{
		// Laser
		if (active) gfx.DrawCircle({ 400, 300 }, radius, Colors::Red, degrees, degrees + angle);

		// Border Circle
		gfx.DrawCircleOutline(Vei2(pos), radius, Colors::LightGray, 5, 50);

		// Rect
		constexpr int rectSize = 8;
		gfx.DrawRect({ Graphics::ScreenWidth / 2 - rectSize, Graphics::ScreenWidth / 2 + rectSize, Graphics::ScreenHeight / 2 - rectSize, Graphics::ScreenHeight / 2 + rectSize }, Colors::DarkGray);

		// Turett
		const float degreesCenter = degrees + angle / 2.0f;
		const int touretSize = (int)(angle * 0.25f);

		gfx.DrawCircle({ Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2 }, rectSize * 2, Colors::Gray,
			degreesCenter - std::min(angle / 2.0f, 180.0f),
			degreesCenter + std::min(angle / 2.0f, 180.0f)
		);
	}

	void Update(float dt)
	{
		degrees += dt * speed;
		degrees = xoy5::deg360(degrees);

		time += dt;

		if (active && time >= duration) {
			time -= duration;
			active = false;
		}
		else if (!active && time >= cooldown) {
			time -= cooldown;
			active = true;
		}
	}

	void AddToSpeed(float val)
	{
		speed += val;
	}

	void AddToAngle(float val)
	{
		angle = (angle + val > 360.0f ? 360.0f : angle + val);
	}

	Vec2 GetPos() const
	{
		return pos;
	}

	float GetRadius() const
	{
		return radius;
	}

	bool Exterminate(const RectI& rect) {
		if (!active) return false;

		const Vei2 laserPos = Vei2(pos);
		const Vei2 topLeft = Vei2(rect.left, rect.top) - laserPos;
		const Vei2 topRight = Vei2(rect.right, rect.top) - laserPos;
		const Vei2 bottomLeft = Vei2(rect.left, rect.bottom) - laserPos;
		const Vei2 bottomRight = Vei2(rect.right, rect.bottom) - laserPos;

		float angleStart = xoy5::deg360(degrees);
		float angleEnd = xoy5::deg360(degrees + angle);

		const float epsilon = 0.0001f;

		// Wide range support
		if (std::abs(angleEnd - angleStart) < epsilon || (angleEnd - angleStart) == 360.0f) {
			angleStart = 0.0f;
			angleEnd = 360.0f;
		}

		auto isAngleInRange = [&](float angle) {
			if (angleStart <= angleEnd) {
				return angle >= angleStart && angle <= angleEnd;
			}
			return angle >= angleStart || angle <= angleEnd;
			};

		// For each quarter
		float alfa1 = xoy5::deg360(xoy5::rtod(std::atan2(topLeft.y, topLeft.x)));
		if (isAngleInRange(alfa1)) {
			return true;
		}

		float alfa2 = xoy5::deg360(xoy5::rtod(std::atan2(topRight.y, topRight.x)));
		if (isAngleInRange(alfa2)) {
			return true;
		}

		float alfa3 = xoy5::deg360(xoy5::rtod(std::atan2(bottomLeft.y, bottomLeft.x)));
		if (isAngleInRange(alfa3)) {
			return true;
		}

		float alfa4 = xoy5::deg360(xoy5::rtod(std::atan2(bottomRight.y, bottomRight.x)));
		if (isAngleInRange(alfa4)) {
			return true;
		}

		return false;
	}


private:
	Vec2 pos;
	float degrees;
	float angle;
	float speed;
	float radius;

	float time = 0.0f;
	float duration;
	float cooldown;
	bool active = true;
};