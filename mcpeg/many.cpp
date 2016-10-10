#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <array>
#include <iostream>

struct Entity {
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Color color;
};

int main() {
	// random number generator
	std::mt19937 mt(0);
	std::uniform_real_distribution<float> dist(1.0, 10.0);
	std::uniform_int_distribution<int> dist2(0, 255);

	int radius = 10;

	auto entities = std::array<Entity, 100>();
	for (auto &entity : entities) {
		entity.velocity.x = dist(mt);
		entity.velocity.y = dist(mt);
		entity.color = sf::Color(dist2(mt), dist2(mt), dist2(mt));
	}

	sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Test");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		sf::Event event;

		// check for window exit
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::Black);

		for (auto &entity : entities) {
			entity.position += entity.velocity;

			// check for collisions on the screen
			auto &position = entity.position;
			auto &velocity = entity.velocity;
			auto &color = entity.color;
			if (position.x > 800 - radius * 2) velocity.x *= -1.f;
			if (position.x < 0) velocity.x *= -1.f;
			if (position.y > 600 - radius * 2) velocity.y *= -1.f;
			if (position.y < 0) velocity.y *= -1.f;

			auto shape = sf::CircleShape(radius);
			shape.setPosition(position);
			shape.setFillColor(color);
			window.draw(shape);
		}

		window.display();
	}
	return 0;
}
