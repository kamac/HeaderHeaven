#include <SFML/Graphics.hpp>
#include "Config.h"
#include "Generator.h"

int main(int argc, char* argv[])
{
	std::string cfgPath(argv[0]);
	std::replace(cfgPath.begin(), cfgPath.end(), '\\', '/');
	cfgPath = cfgPath.substr(0, cfgPath.find_last_of('/') + 1);
	Config cfg(cfgPath, "config.txt");

	Generator generator;
	const auto& files = cfg.GetFiles();
	generator.Generate(files[0].headerPath, files[0].sourcePath);

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	window.setFramerateLimit(60);
	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(50.0f, 50.0f));
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.display();
	}

	return 0;
}