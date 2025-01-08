/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include <math.h>
Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	laser(Vec2{ Graphics::ScreenWidth / 2, Graphics::ScreenHeight / 2 }, 240.0f, 200.0f, angle, speed, 1.0f),
	drawOpt(int(laser.GetRadius() * 2), int(laser.GetRadius() * 2), Vei2(laser.GetPos() - Vec2{laser.GetRadius(), laser.GetRadius()}))
{
	buttonLimit.SetBorder(true, 5, Colors::Green);
	Add10Balls();
	Add10Balls();
}

void Game::Go()
{
	gfx.BeginFrame();	
	ProcessInput();
	if (playSymulation) {
		float elapsedTime = ft.Mark();
		while (elapsedTime > 0.0f) 
		{
			const float dt = std::min(precision, elapsedTime);
			UpdateModel(dt * speedSymulation);
			elapsedTime -= dt;
		}
	}
	else {
		ft.Mark();
	}
	ComposeFrame();
	gfx.EndFrame();
}

void Game::ProcessInput()
{
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		buttonUp.ProcessMouse(e);
		buttonDown.ProcessMouse(e);
		buttonPlay.ProcessMouse(e);
		buttonAdd.ProcessMouse(e);
		buttonLimit.ProcessMouse(e);

		if (buttonUp.IsClicked()) {
			speedSymulation += 0.05f;
		}
		else if (buttonDown.IsClicked()) {
			speedSymulation = std::max(0.0f, speedSymulation - 0.05f);
		}
		else if (buttonPlay.IsClicked()) {
			playSymulation = !playSymulation;
		}
		else if (buttonAdd.IsClicked()) {
			Add10Balls();
		}
		else if (buttonLimit.IsClicked()) {
			if (BALLS_LIMIT) {
				buttonLimit.ChangeText("no limit");
				buttonLimit.SetBorder(true, 5, Colors::Red);
			}
			else {
				buttonLimit.ChangeText("yes limit");
				buttonLimit.SetBorder(true, 5, Colors::Green);
			}
			BALLS_LIMIT = !BALLS_LIMIT;
		}
	}
}

void Game::UpdateModel(float dt)
{
	laser.Update(dt);
	const float delta = dt * someFuckingMathCharSybmol;
	if (angle + delta < maxAngle) {
		angle += delta;
		laser.AddToAngle(delta);
	}
	if (speed + delta < maxSpeed) {
		speed += delta;
		laser.AddToSpeed(delta);
	}

	for (int i = 0; i < balls.size(); i++) {
		balls[i].Update(dt);

		if (balls[i].BorderCollision(laser.GetPos(), laser.GetRadius())){
			if (BALLS_LIMIT && balls.size() >= BALLS_MAX) continue;
			Vec2 vel = balls[i].GetVel();
			balls.emplace_back(Ball{ balls[i].GetPos(), vel * 0.99f, 10.0f, ballsColors[balls.size() % nColors], true });
		}
	}

	balls.erase(
		std::remove_if(
			balls.begin(),
			balls.end(),
			[&](const Ball& ball) {
				return (laser.Exterminate(RectI{ ball.GetRect() }));
			}
		),
		balls.end()
	);
}

void Game::ComposeFrame()
{
	for (const auto& ball : balls) {
		drawOpt.AddCircle(ball);
	}
	drawOpt.Draw(gfx);
	drawOpt.Reset();

	laser.Draw(gfx);
	buttonUp.Draw(gfx);
	buttonDown.Draw(gfx);
	buttonPlay.Draw(gfx);
	buttonAdd.Draw(gfx);
	buttonLimit.Draw(gfx);
	font.DrawText("Balls: " + std::to_string(balls.size()), {10,10}, Colors::White, gfx);
}

void Game::Add10Balls()
{
	constexpr int nBalls = 10;
	for (int i = 0; i < nBalls; i++) {
		balls.emplace_back(Ball(Vec2(Graphics::GetScreenCenter()) + Vec2{ 0.0f, 10.0f }, Vec2(range100(rng), std::abs(range100(rng))), 10.0f, ballsColors[i % nBalls]));
	}
}
