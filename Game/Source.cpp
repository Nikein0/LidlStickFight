#include "SFML/Graphics.hpp"

class Animation
{
public:
	Animation(int x, int y, int width, int height)
	{
		texture.loadFromFile("man.png");
			for (int i = 0; i < nframes; i++)
			{
				frames[i] = { x + i * width,y,width,height };
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
		rightWalk(0, 155, 55, 70),
		leftWalk(0, 245, 55, 70),
		stand(60, 145, 40, 85)
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
	void Standing(float dt)
	{
		rightWalk.Update(dt);
		stand.Apply(sprite);
		sprite.setPosition(pos);
	}
private:
	const float speed = 100.0f;
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Texture texture;
	sf::Sprite sprite;
	Animation rightWalk;
	Animation leftWalk;
	Animation stand;
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Test");
	window.setFramerateLimit(60);

	//sf::RectangleShape rect;
	//sf::Texture texture;
	//if (!texture.loadFromFile("man.png"))
		//return EXIT_FAILURE;
	//sf::Sprite sprite(texture);
	//sprite.setTextureRect({ 170,460,130,220 });

	//170, 460, 200, 300

	//sf::Vector2f rectanglePos(320, 240);

	//rect.setPosition(rectanglePos);
	//rect.setSize(sf::Vector2f(30, 30));
	Man dude({320.0f,240.0f});

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed) window.close();
		}
		//movement
		sf::Vector2f dir = { 0.0f, 0.0f };
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			dir.y -= 1.0f;
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			dir.y += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			dir.x -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			dir.x += 1.0f;
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
		}
		//rendering
		window.clear(sf::Color::Cyan);
		dude.Draw(window);
		window.display();
	}

	return 0;
}