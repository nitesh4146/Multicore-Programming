#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <vector>
#include <iostream>
#include <cmath>

struct Entity {
	sf::Vector2f position;
	sf::Vector2f velocity;
	sf::Color color;
};

int radius = 25;

class World {
	public:
	void step(float t);
	std::vector<Entity> entities;
	private:
	void kinematics(float t);
	void collisions(float t);
};

void World::step(float t) {
	kinematics(t);
	collisions(t);
}

bool circle_circle(Entity &a, Entity &b) {
	auto d = a.position - b.position;
	return d.x * d.x + d.y * d.y < 4 * radius * radius;
}

void reflection(sf::Vector2f r, Entity &a) {
	float len = sqrt(r.x * r.x + r.y * r.y);
	r.x /= len;
	r.y /= len;
}

void World::kinematics(float t) {
	for (auto &entity : entities) {
		entity.position += entity.velocity * t;
	}
}

void World::collisions(float t) {
	for (auto &entity : entities) {
		auto &position = entity.position;
		auto &velocity = entity.velocity;
		auto &color = entity.color;

		// check for collisions with the screen
		if (position.x > 800 - radius * 2) velocity.x *= -1.f;
		if (position.x < 0) velocity.x *= -1.f;
		if (position.y > 600 - radius * 2) velocity.y *= -1.f;
		if (position.y < 0) velocity.y *= -1.f;
	}

	for (int i = 0; i < entities.size(); i++) {
		for (int j = i + 1; j < entities.size(); j++) {
			auto &a = entities.at(i);
			auto &b = entities.at(j);
			if (circle_circle(a, b)) {
				a.velocity.x *= -1.f;
				a.velocity.y *= -1.f;

				b.velocity.x *= -1.f;
				b.velocity.y *= -1.f;
			}
		}
	}
}

int main() {
	// random number generator, 0 seeded
	std::mt19937 mt(0);
	std::uniform_real_distribution<float> dist(1.0, 10.0);
	std::uniform_int_distribution<int> dist2(0, 255);
	std::uniform_int_distribution<int> dist3(100, 500);

	// initialize entities
	World world;
	world.entities.resize(10);
	for (auto &entity : world.entities) {
		entity.position.x = dist3(mt);
		entity.position.y = dist3(mt);
		entity.velocity.x = dist(mt);
		entity.velocity.y = dist(mt);
		entity.color = sf::Color(dist2(mt), dist2(mt), dist2(mt));
	}

	// create window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Collision Test");
	window.setFramerateLimit(60);

	// main loop
	while (window.isOpen()) {
		sf::Event event;

		// check for window exit
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		// step physics engine
		world.step(1.f);

		// render
		window.clear(sf::Color::Black);
		for (auto &entity : world.entities) {
			// check for collisions on the screen
			auto &position = entity.position;
			auto &color = entity.color;

			auto shape = sf::CircleShape(radius);
			shape.setPosition(position);
			shape.setFillColor(color);
			window.draw(shape);
		}
		window.display();
	}
	return 0;
}
