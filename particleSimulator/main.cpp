#include <iostream>
#include "Simulator.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 500), "Particle Simulation");

	Simulator simulator(window.getSize());

	bool in_focus = true;

	while (window.isOpen())
	{
		sf::Event e;

		while (window.pollEvent(e))
		{
			//Close window if closed or Escape key is pressed while in focus.
			if (e.type == sf::Event::Closed || (in_focus && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
				window.close();

			//Adjust window focus if changed.
			if (in_focus && e.type == sf::Event::LostFocus)
				in_focus = false;
			else if (!in_focus && e.type == sf::Event::GainedFocus)
				in_focus = true;
		}

		//Only update and draw to window if it is in focus.
		if (in_focus)
		{
			//DEBUG SPACEBAR SLOW MO
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				window.setFramerateLimit(10);
			else
				window.setFramerateLimit(200);

			window.clear();

			simulator.update(window);
			simulator.draw_simulator(window);

			window.display();
		}
	}

	return 0;
}
