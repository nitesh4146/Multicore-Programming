#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "TEST WINDOW");
	window.setFramerateLimit(60);

	int radius = 20;

	auto shape = sf::CircleShape(radius);
	auto velocity = sf::Vector2f(1.f, 5.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen()) {
		sf::Event event;

		// check for window exit
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear(sf::Color::Black);

		shape.move(velocity);

		// check for collisions on the screen
		auto position = shape.getPosition();
		if (position.x > 800 - radius * 2) velocity.x *= -1.f;
		if (position.x < 0) velocity.x *= -1.f;
		if (position.y > 600 - radius * 2) velocity.y *= -1.f;
		if (position.y < 0) velocity.y *= -1.f;

		window.draw(shape);
		window.display();
	}
	return 0;
}
