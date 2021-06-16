#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int size = 16;
int wSize = 30;
int hSize = 20;
int hHUD = 30;
int width = wSize * size;
int height = hSize * size + hHUD;

int snakeSize = 3;
int dir = 0;
int score = 0;
int highscore = 0;
float difficulty = 0.1;
float maxDifficulty = 0.02;
float delay;

struct Snake {
	int x;
	int y;
} snake[100];

struct Fruit {
	int x;
	int y;
} fruit;

RenderWindow window;
Font font;
Text text;




void CalcFruitPosition()
{
	fruit.x = rand() % wSize;
	fruit.y = rand() % hSize;
}


void Tick()
{
	// Movemos una posición los valores del array de la serpiente
	for (int i = snakeSize; i > 0; --i) {
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	// Movemos la cabeza de la serpiente según
	// la dirección actual
	if (dir == 0) { snake[0].y += 1; } // Abajo
	if (dir == 1) { snake[0].x += 1; } // Derecha
	if (dir == 2) { snake[0].y -= 1; } // Arriba
	if (dir == 3) { snake[0].x -= 1; } // Izquierda

	if (snake[0].x < 0) { snake[0].x = wSize - 1; }
	if (snake[0].x > wSize - 1) { snake[0].x = 0; }
	if (snake[0].y < 0) { snake[0].y = hSize - 1; }
	if (snake[0].y > hSize - 1) { snake[0].y = 0; }

	// Comprobamos si nos comemos la fruta
	if (snake[0].x == fruit.x && snake[0].y == fruit.y) {
		// Hacemos más grande a la serpiente
		snakeSize++;

		// Sumamos puntos
		score++;

		// Calculamos la posición de la nueva fruta
		CalcFruitPosition();

		// Aumentamos dificultad
		if ((score % 5 == 0) && (difficulty > maxDifficulty)) {
			difficulty -= 0.01;
			delay = difficulty;
		}
	}

	// Comprobamos si nos chocamos con nosotros mismos
	for (int i = 1; i < snakeSize - 1; i++) {
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {

			snakeSize = i;

			// Comprobamos si hay nuevo record
			if (score > highscore) {
				highscore = score;
			}

			// Reseteamos puntuación
			score = 0;

			// Reseteamos dificultad
			difficulty = 0.1;
			delay = difficulty;
		}
	}
}



int main()
{
	srand(time(0));
	RenderWindow window(VideoMode(width, height), "Snake Game!");

	Texture t1, t2, t3;
	t1.loadFromFile("./images/floor.png");
	t2.loadFromFile("./images/snake.png");
	t3.loadFromFile("./images/fruit.png");

	Sprite spriteFloor(t1);
	Sprite spriteSnake(t2);
	Sprite spriteFruit(t3);

	// Creamos la fuente
	if (!font.loadFromFile("./fonts/Minercraftory.ttf")) {
		throw("NO SE HA PODIDO CARGAR LA FUENTE");
	}

	Clock clock;
	float timer = 0;
	delay = difficulty;

	// Calcular la posición de la fruta
	CalcFruitPosition();

	// Este bucle se ejecuta mientras la ventana
	// esté abierta, y salimos cuando el usuario
	// pincha en el botón de cerrar la ventana.
	while (window.isOpen()) {

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		// Comprobamos si el usuario ha cerrado la ventana
		Event e;
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
		}

		// Entrada de teclado
		if (Keyboard::isKeyPressed(Keyboard::Down)) { dir = 0; }
		if (Keyboard::isKeyPressed(Keyboard::Right)) { dir = 1; }
		if (Keyboard::isKeyPressed(Keyboard::Up)) { dir = 2; }
		if (Keyboard::isKeyPressed(Keyboard::Left)) { dir = 3; }

		// Ejecutamos el TICK
		if (timer > delay) {
			timer = 0;
			Tick();
		}

		window.clear();

		// Dibujamos el mapa recorriendo el ancho y el alto
		// según las dimensiones configuradas
		for (int i = 0; i < wSize; i++) {
			for (int j = 0; j < hSize; j++) {
				spriteFloor.setPosition(i * size, j * size);
				window.draw(spriteFloor);
			}
		}

		// Dibujamos la serpiente
		for (int i = 0; i < snakeSize; i++) {
			spriteSnake.setPosition(snake[i].x * size, snake[i].y * size);
			window.draw(spriteSnake);
		}

		// Dibujamos la fruta
		spriteFruit.setPosition(fruit.x * size, fruit.y * size);
		window.draw(spriteFruit);

		// Mostramos récord
		text.setFont(font);
		text.setString("Highscore..." + std::to_string(highscore));
		text.setCharacterSize(16);
		text.setFillColor(Color::White);
		text.setPosition(width - 170, height - hHUD + 5);
		window.draw(text);

		// Mostramos puntuación
		text.setFont(font);
		text.setString("Score..." + std::to_string(score));
		text.setCharacterSize(16);
		text.setFillColor(Color::White);
		text.setPosition(5, height - hHUD + 5);
		window.draw(text);

		window.display();
	}

	return 0;
}
