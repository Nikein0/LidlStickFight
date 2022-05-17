#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include "Header.h"


class Animation
{
public:
	Animation(int x, int y, int width, int height, int count)
	{
		texture.loadFromFile("man.png");
		if (count > 1) {
			for (int i = 0; i < nframes; i++)
			{
				frames[i] = { x + i * width,y,width,height };
			}
		}
		else
		{
			frames[0] = { x + 0 * width,y,width,height };
		}

	}
	void Apply(sf::Sprite& s) const
	{
		s.setTexture(texture);
		s.setTextureRect(frames[curFrame]);
	}
	void Update(float dt)
	{
		time += dt;
		while (time >= hold)
		{
			time -= hold;
			Advance();
		}
	}
private:
	void Advance()
	{
		if (++curFrame >= nframes)
		{
			curFrame = 0;
		}
	}
private:
	static const int nframes = 2;
	const float hold = 0.5f;
	sf::Texture texture;
	sf::IntRect frames[nframes];
	int curFrame = 0;
	float time = 0.0f;
};
class Man
{
public:
	Man(const sf::Vector2f& pos)
		:
		pos(pos),
		rightWalk(0, 155, 55, 70, 2),
		leftWalk(0, 245, 55, 70, 2),
		stand(60, 145, 40, 85, 0),
		punch(170, 150, 80, 70, 0),
		punchLeft(255, 150, 80, 70, 0),
		dead(20, 325, 70, 55, 0)
	{
		//sprite.setTextureRect({ 55,155,55,70 });
		//sprite.setTextureRect({ 5,30,20,70 });
		//texture.loadFromFile("man.png");
		//sprite.setTextureRect({ 55,155,55,70 });
		//sprite.setScale();
	}
	void Draw(sf::RenderTarget& rt) const
	{
		rt.draw(sprite);
	}
	void Direction(const sf::Vector2f& dir) {
		vel = dir * speed;
	}
	void UpdateRight(float dt)
	{
		pos += vel * dt;
		rightWalk.Update(dt);
		rightWalk.Apply(sprite);
		sprite.setPosition(pos);
	}
	void UpdateLeft(float dt)
	{
		pos += vel * dt;
		leftWalk.Update(dt);
		leftWalk.Apply(sprite);
		sprite.setPosition(pos);
	}
	void Punch(float dt, bool side)
	{
		if (side == true) {
			punch.Apply(sprite);
		}
		else
		{
			punchLeft.Apply(sprite);
		}
		//sprite.setPosition(pos);
	}
	void Standing(float dt)
	{
		rightWalk.Update(dt);
		stand.Apply(sprite);
		sprite.setPosition(pos);
	}
	void Knockback(float dt, bool side)
	{
		if (side == false)
		{
			pos.x = pos.x + 30;
			sprite.setPosition(pos);
		}
		else {
			pos.x = pos.x - 30;
			sprite.setPosition(pos);
		}
	}
	void Death(float dt)
	{
			pos.y = pos.y + 40;
			dead.Update(dt);
			dead.Apply(sprite);
			sprite.setPosition(pos);
	}
	sf::Vector2f GetCoords()
	{
		return sprite.getPosition();
	}
private:
	const float speed = 150.0f;
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Texture texture;
	sf::Sprite sprite;
	Animation rightWalk;
	Animation leftWalk;
	Animation stand;
	Animation punch;
	Animation punchLeft;
	Animation dead;
};
class Enemy
{
public:
	Enemy(const sf::Vector2f& pos)
		:
		pos(pos),
		chasing(0, 245, 55, 70, 2),
		chasingLeft(0, 155, 55, 70, 2),
		dead(20, 325, 70, 55, 0)
	{

	}
	void Draw(sf::RenderTarget& rt) const
	{
		rt.draw(sprite);
	}
	void Direction(const sf::Vector2f& dir) {
		vel = dir * speed;
	}
	void Chase(sf::Vector2f& coords, float dt, sf::Vector2f& coordsEn)
	{
		if (coords.x < pos.x) {
			pos.x = pos.x - 2;
		}
		if (coords.x > pos.x) {
			pos.x = pos.x + 2;
		}
		if (coords.x > coordsEn.x)
		{
			chasingLeft.Update(dt);
			chasingLeft.Apply(sprite);
			sprite.setPosition(pos);
		}
		else {
			chasing.Update(dt);
			chasing.Apply(sprite);
			sprite.setPosition(pos);
		}
	}
	void Death(float dt)
	{
				pos.y = pos.y + 50;
				dead.Update(dt);
				dead.Apply(sprite);
				sprite.setPosition(pos);
	}
	sf::Vector2f GetCoords()
	{
		return sprite.getPosition();
	}
	void Knockback(float dt, bool side)
	{
		if (side == false)
		{
			pos.x = pos.x - 30;
			sprite.setPosition(pos);
		}
		else {
			pos.x = pos.x + 30;
			sprite.setPosition(pos);
		}
	}
	void Respawn(float dt, float x, float y)
	{
		pos.y = y;
		pos.x = x;
		chasing.Update(dt);
		chasing.Apply(sprite);
		sprite.setPosition(pos);
	}
private:
	const float speed = 100.0f;
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Sprite sprite;
	Animation chasing;
	Animation dead;
	Animation chasingLeft;
};


int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Test");
	window.setFramerateLimit(60);
	Man dude({330.0f,360.0f});
	Enemy man({ 550.0f,360.0f });
	sf::Font font;
	sf::Text scoreText;
	sf::Text StartText;
	int scorenumber = 0;
	bool isDeadEn = false;
	bool isDead = false;
	bool Start = false;
	int healthEn = 0;
	int health = 0;
	int respawn = 0;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Font error" << std::endl;
	}
	while (window.isOpen())
	{
		sf::Event e;
		while (Start != true) {
			window.clear(sf::Color::Cyan);
			StartText.setString("       StickFighter\nClick 'space' to begin");
			StartText.setCharacterSize(24);
			StartText.setFont(font);
			StartText.setStyle(sf::Text::Regular);
			StartText.setPosition(240, 200);
			StartText.setFillColor(sf::Color::Red);
			window.draw(StartText);
			window.display();
			while (window.pollEvent(e))
			{
				if (e.type == sf::Event::Closed) window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				Start = true;
			}
		}
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) window.close();
		}
		

			//movement
			sf::Vector2f dir = { 0.0f, 0.0f };
			/*
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				dir.y -= 1.0f;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				dir.y += 1.0f;
			}
			*/

			if (isDead == true) {

			}
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					dir.x -= 1.0f;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					dir.x += 1.0f;
				}

				sf::Vector2f coords = dude.GetCoords();
				sf::Vector2f coordsEn = man.GetCoords();
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				//std::cout << "Mouse: " << mousePos.x << " " << mousePos.y << std::endl;

				if (coords.x >= 600 || coords.x <= 0)
				{
					if (coords.x >= 600)
						dir.x -= 1.0f;
					if (coords.x <= 0)
						dir.x += 1.0f;
				}
				if (coords.y >= 400 || coords.y <= -10)
				{
					if (coords.y >= 400)
						dir.y -= 1.0f;
					if (coords.y <= -10)
						dir.y += 1.0f;
				}
				else if (coords.y == 470) {
					dir.y = 0.0f;
				}
				if (dir.y != 0 || dir.x != 0) {
					dude.Direction(dir);
					if (dir.x < 0.0f) {
						dude.UpdateLeft(1.0f / 60.0f);
					}
					else {
						dude.UpdateRight(1.0f / 60.0f);
					}
				}
				else {
					dude.Standing(1.0f / 60.0f);
					int attackcoordsX, attackcoordsY;
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
						dude.Punch(1.0f / 60.0f, false);
						if (coords.x > coordsEn.x)
						{
							attackcoordsX = coords.x - coordsEn.x;
							attackcoordsY = coords.y - coordsEn.y;
							if (attackcoordsX < 40 && attackcoordsY < 40)
							{
								health++;
								if (health != 3)
									man.Knockback(1.0f / 60.0f, false);
								else {
									isDeadEn = true;
									man.Death(1.0f / 60.0f);
									health = 0;
									scorenumber++;
									scoreText.setString(std::to_string(scorenumber));
									scoreText.setCharacterSize(24);
									scoreText.setFont(font);
									scoreText.setStyle(sf::Text::Regular);
									scoreText.setPosition(320, 200);
									scoreText.setFillColor(sf::Color::Red);
								}
							}
						}
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
						dude.Punch(1.0f / 60.0f, true);
						if (coords.x < coordsEn.x)
						{
							attackcoordsX = coordsEn.x - coords.x;
							attackcoordsY = coordsEn.y - coords.y;

							if (attackcoordsX < 40 && attackcoordsY < 40)
							{
								health++;
								if (health != 3)
									man.Knockback(1.0f / 60.0f, true);
								else {
									isDeadEn = true;
									man.Death(1.0f / 60.0f);
									health = 0;
									scorenumber++;
									scoreText.setString(std::to_string(scorenumber));
									scoreText.setCharacterSize(24);
									scoreText.setFont(font);
									scoreText.setStyle(sf::Text::Regular);
									scoreText.setPosition(320, 200);
									scoreText.setFillColor(sf::Color::Red);
								}
							}
						}
					}
				}
				bool restart = false;
				if (coordsEn.x > coords.x) {
					if (coordsEn.x - coords.x < 10)
					{
						if (isDeadEn != true) {
							health++;
							if (health != 3)
							{
								dude.Knockback(1.0f / 60.0f, true);
							}
							else
							{
								isDead = true;
								dude.Death(1.0f / 60.0f);
								scoreText.setString("You died");
								scoreText.setPosition(320, 200);
							}
						}
					}
				}
				else
				{
					if (coords.x - coordsEn.x < 10)
					{
						if (isDeadEn != true) {
							health++;
							if (health != 3)
							{
								dude.Knockback(1.0f / 60.0f, true);
							}
							else
							{
								isDead = true;
								dude.Death(1.0f / 60.0f);
								scoreText.setString("You died");
								scoreText.setPosition(320, 200);
							}
						}
					}
				}
				if (isDeadEn != true) {
					man.Chase(coords, 1.0f / 60.0f, coordsEn);
				}
				else
				{
					respawn++;
					if (respawn == 100)
					{
						if (scorenumber % 2 == 0)
						{
							man.Respawn(1.0f / 60.0f, -50.0f, 360.0f);
						}
						else
						{
							man.Respawn(1.0f / 60.0f, 740.0f, 360.0f);
						}
						isDeadEn = false;
						respawn = 0;
					}
				}

			}
			//rendering
			window.clear(sf::Color::Cyan);
			window.draw(scoreText);
			dude.Draw(window);
			man.Draw(window);
			window.display();

			//std::cout << "Player: " << coords.x << " " << coords.y << std::endl;
			//std::cout << "Enemy: " << coordsEn.x << " " << coordsEn.y << std::endl;

	}

	return 0;
}