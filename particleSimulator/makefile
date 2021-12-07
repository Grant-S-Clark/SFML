exe:
	g++ -c *.cpp -I/usr/local/SFML/include
	g++ *.o -o sfml-app -L/usr/local/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
	rm *.o
run:
	export LD_LIBRARY_PATH=/usr/local/SFML/lib && ./sfml-app

quick:
	g++ -c *.cpp -I/usr/local/SFML/include
	g++ *.o -o sfml-app -L/usr/local/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system
	rm *.o
	export LD_LIBRARY_PATH=/usr/local/SFML/lib && ./sfml-app

clean:
	rm sfml-app

c:
	rm sfml-app
