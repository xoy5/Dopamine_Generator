/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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
#pragma once

#include <vector>
#include <random>

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Laser.h"
#include "FrameTimer.h"
#include "Font.h"
#include "Ball.h"
#include "Button.h"
#include "DrawOptimizer.h"

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ProcessInput();
	void UpdateModel(float dt);
	void ComposeFrame();
	/********************************/
	/*  User Functions              */
	void Add10Balls();
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */

	// symulation SHIT
	float angle = 15.0f;
	const float maxAngle = 100.0f;
	float speed = 30.0f;
	const float maxSpeed = 80.0f;
	const float someFuckingMathCharSybmol = 5.0f;
	int BALLS_MAX = 10000;
	bool BALLS_LIMIT = true;

	Laser laser;
	DrawOptimizer drawOpt;
	FrameTimer ft;
	Font font = Font("Images/Fonts/font16x28.bmp");
	const float precision = 0.025f;
	std::vector<Ball> balls;
	const int nColors = 7;
	Color ballsColors[7] = {Colors::Blue, Colors::Cyan, Colors::Green, Colors::Yellow, Colors::Grapefruit, Colors::Pink, Colors::Purple};

	// random
	std::mt19937 rng = std::mt19937(std::random_device{}());
	std::uniform_int_distribution<int> range100 = std::uniform_int_distribution<int>(-100, 100);

	// interface
	float speedSymulation = 0.7f;
	bool playSymulation = false;
	const int padding = 10;
	Button buttonPlay = Button(&font, "play", { 260, 540 });
	Button buttonUp = Button(&font, "up", { buttonPlay.GetPosEndX() + padding, 540 });
	Button buttonDown = Button(&font, "down", { buttonUp.GetPosEndX() + padding, 540 });
	Button buttonLimit = Button(&font, "yes limit", { 10, 50 });
	Button buttonAdd = Button(&font, "add", { 10, 110 });
	/********************************/
};