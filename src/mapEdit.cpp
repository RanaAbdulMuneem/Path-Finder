#include <SFML\Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void printInst() {
	cout << "Press esc to exit\n";
	cout << "WASD TO MOVE CURSOR\n";
	cout << "1 2 3 TO SET TILE TYPE\n";
	cout << "SPACE TO EDIT TILE\n";
}
void generateStd(string path) {
	ofstream fout(path);
	int rows, cols, value;
	cout << "rows: ";
	cin >> rows;
	cout << "cols: ";
	cin >> cols;
	cout << "default value: ";
	cin >> value;
	int size = rows * cols;

	fout << rows << ' ' << cols << '\n';
	for (int i = 0; i < size; i++) {
		fout << value;
		if (i % cols != cols - 1)
			fout << ' ';
		else if (i != size - 1)
			fout << '\n';
	}
	cout << "Standard " << rows << 'x' << cols << " map created!\n";
	fout.close();
}

void setColor(sf::RectangleShape& tile, int code) {
	//tile.setFillColor(sf::Color(code*15, 0 , 0));
	if (code == 0)
		tile.setFillColor(sf::Color::Black);
	else if (code == 1)
		tile.setFillColor(sf::Color::White);
	else if (code == 2)
		tile.setFillColor(sf::Color::Green);
	else if (code == 3)
		tile.setFillColor(sf::Color::Red);
	else
		tile.setFillColor(sf::Color::Magenta);
}
int getTile(const sf::RectangleShape& tile) {
	sf::Color color = tile.getFillColor();
	if (color == sf::Color::Black)
		return 0;
	else if (color == sf::Color::White)
		return 1;
	else if (color == sf::Color::Green)
		return 2;
	else if (color == sf::Color::Red)
		return 3;
	else
		return 0;
}
void editMap(string path) {
	ifstream fin(path);
	if (!fin.is_open()) {
		generateStd(path);
		fin.open(path);
	}
	int rows;
	int cols;
	int size;
	fin >> rows >> cols;
	size = rows * cols;

	sf::RectangleShape* maze = new sf::RectangleShape[size];

	int x = 0;
	int y = 0;
	for (int i = 0; i < size; i++) {
		int val;
		fin >> val;
		setColor(maze[i], val);
		maze[i].setSize(sf::Vector2f(15, 15));
		maze[i].setOutlineThickness(1);
		maze[i].setOutlineColor(sf::Color::Black);
		maze[i].setPosition(sf::Vector2f(x, y));

		x += 15;
		if (i!=0 && i%cols==cols-1) {
			x = 0;
			y += 15;
		}
	}
	fin.close();

	sf::RectangleShape cursor;
	cursor.setPosition(sf::Vector2f(0,0));
	cursor.setSize(sf::Vector2f(15, 15));
	cursor.setFillColor(sf::Color::Transparent);
	cursor.setOutlineThickness(2);
	cursor.setOutlineColor(sf::Color::Blue);

	x = cols * 15;
	y = rows * 15;

	int mode = 0;
	int cursorIndex = 0;
	int selectX = 0;
	int selectY = 0;
	sf::RenderWindow window(sf::VideoMode(x,y), "MAP EDITOR", sf::Style::Close);

	window.setFramerateLimit(30);

	printInst();
	bool shiftHeld = false;
	while (window.isOpen()) {
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					window.close();
				}
				else if (event.key.code == sf::Keyboard::Space) {
					setColor(maze[cursorIndex], mode);
				}
				else if (event.key.code == sf::Keyboard::LShift) {
					shiftHeld = true;
					cout << "LShift held\n";
				}
				else if (event.key.code == sf::Keyboard::Num0) {
					mode = 0;
					system("cls");
					printInst();
					cout << "tile:\t" << "wall" << endl;
				}
				else if (event.key.code == sf::Keyboard::Num1) {
					mode = 1;
					system("cls");
					printInst();
					cout << "tile:\t" << "path" << endl;
				}
				else if (event.key.code == sf::Keyboard::Num2) {
					mode = 2;
					system("cls");
					printInst();
					cout << "tile:\t" << "start" << endl;
				}
				else if (event.key.code == sf::Keyboard::Num3) {
					mode = 3;
					system("cls");
					printInst();
					cout << "tile:\t" << "end" << endl;
				}
				else if (event.key.code == sf::Keyboard::W) {
					if (cursorIndex >= cols) {
						cursorIndex -= cols;
						cursor.move(sf::Vector2f(0, -15));
						//cout << "index:\t" << cursorIndex << endl;
					}
				}
				else if (event.key.code == sf::Keyboard::A) {
					if (cursorIndex%cols > 0) {
						cursorIndex--;
						cursor.move(sf::Vector2f(-15, 0));
						//cout << "index:\t" << cursorIndex << endl;
					}
				}
				else if (event.key.code == sf::Keyboard::S) {
					if (cursorIndex < size - cols) {
						cursorIndex += cols;
						cursor.move(sf::Vector2f(0, 15));
						//cout << "index:\t" << cursorIndex << endl;
					}
				}
				else if (event.key.code == sf::Keyboard::D) {
					if (cursorIndex%cols < cols - 1) {
						cursorIndex++;
						cursor.move(sf::Vector2f(15, 0));
						//cout << "index:\t" << cursorIndex << endl;
					}
				}
			}

			else if (event.type == sf::Event::KeyReleased) {
				if (event.key.code == sf::Keyboard::LShift) {
					shiftHeld = false;
					cout << "LShift released\n";
				}
			}
		}

		window.clear();

		for (int i = 0; i < size; i++)
			window.draw(maze[i]);

		window.draw(cursor);
		window.display();

	}

	ofstream fout(path);
	fout << rows << ' ' << cols << '\n';

	for (int i = 0; i < size; i++) {
		fout << getTile(maze[i]);

		if (i % cols != cols - 1)
			fout << ' ';
		else if (i != size - 1)
			fout << '\n';
	}
	cout << "Map edited and saved!\n";
}

int main() {
	string path;
	cout << "Enter file name: ";
	cin >> path;
	editMap(path);

	system("pause");
	return 0;
}