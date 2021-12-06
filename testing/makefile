exe:
	g++ -c testing.cpp -I/usr/local/SFML/include -o a.out
	g++ a.out -o sfml-app -L/usr/local/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
	rm a.out
run:
	export LD_LIBRARY_PATH=/usr/local/SFML/lib && ./sfml-app

quick:
	g++ -c testing.cpp -I/usr/local/SFML/include -o a.out
	g++ a.out -o sfml-app -L/usr/local/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
	rm a.out
	export LD_LIBRARY_PATH=/usr/local/SFML/lib && ./sfml-app

clean:
	rm sfml-app

c:
	rm sfml-app
