#include "SFML/Graphics.hpp"
#include <iostream>
#include "Header.h"


void Inventory::addtoinv(int a) 
{
	
}
void Inventory::display()
{

}
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
	void Punch(float dt)
	{
		punch.Apply(sprite);
		//sprite.setPosition(pos);
	}
	void Standing(float dt)
	{
		rightWalk.Update(dt);
		stand.Apply(sprite);
		sprite.setPosition(pos);
	}
	void Knockback(float dt)
	{
		pos.x = pos.x - 30;
		sprite.setPosition(pos);
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
	Animation dead;
};
class Enemy
{
public:
	Enemy(const sf::Vector2f& pos)
		:
		pos(pos),
		chasing(0, 245, 55, 70, 2),
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
	void Chase(sf::Vector2f& coords, float dt)
	{
		if (coords.x < pos.x) {
			pos.x = pos.x - 2;
		}
		if (coords.x > pos.x) {
			pos.x = pos.x + 2;
		}
			
		chasing.Update(dt);
		chasing.Apply(sprite);
		sprite.setPosition(pos);
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
	void Knockback(float dt)
	{
		pos.x = pos.x + 30;
		sprite.setPosition(pos);
	}
private:
	const float speed = 100.0f;
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Sprite sprite;
	Animation chasing;
	Animation dead;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Test");
	window.setFramerateLimit(60);

	Man dude({330.0f,360.0f});
	Enemy man({ 660.0f,360.0f});
	bool isDeadEn = false;
	bool isDead = false;
	int healthEn = 0;
	int health = 0;
	while (window.isOpen())
	{
		sf::Event e;
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
			if (isDeadEn != true) {
				man.Chase(coords, 1.0f / 60.0f);
			}
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
				if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
					int attackcoordsX, attackcoordsY;
					dude.Punch(1.0f / 60.0f);
					if (coords.x < coordsEn.x)
					{
						attackcoordsX = coordsEn.x - coords.x;
						attackcoordsY = coordsEn.y - coords.y;

						if (attackcoordsX < 40 && attackcoordsY < 40)
						{
							health++;
							if (health != 3)
								man.Knockback(1.0f / 60.0f);
							else {
								isDeadEn = true;
								man.Death(1.0f / 60.0f);
							}
						}
					}
				}
			}
			if (coordsEn.x - coords.x < 10)
			{
				if (isDeadEn != true) {
					health++;
					if (health != 3)
					{
						dude.Knockback(1.0f / 60.0f);
					}
					else
					{
						isDead = true;
						dude.Death(1.0f / 60.0f);
					}
				}
			}

		}
			//rendering
			window.clear(sf::Color::Cyan);
			dude.Draw(window);
			man.Draw(window);
			window.display();
			//d::cout << "Player: " << coords.x << " " << coords.y << std::endl;
			//d::cout << "Enemy: " << coordsEn.x << " " << coordsEn.y << std::endl;
		
	}

	return 0;
}