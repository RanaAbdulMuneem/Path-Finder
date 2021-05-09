#pragma once

#ifdef _WIN32
#include <SFML\Graphics.hpp>
#endif
#ifdef linux
#include <SFML/Graphics.hpp>
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include "Vertex.h"

using namespace std;

class maze {
	void setRemainingDistances() {//sets remainingDist for all non wall vertices
		for (int i = 0; i < size; i++) {
			if (vertices[i].getValue() != 0)
				vertices[i].setRemainingDist(vertices[end]);
		}
	}
	int start;		//start point of maze
	int end;		//end point of maze
	int rows;		//rows of maze
	int cols;		//columns of maze
	int size;		//size of maze (rows*cols)
public:
	vector<vertex> vertices;

	//constructors
	maze() {
		//cout << "map() called\n";
		start = -1;
		end = -1;
		rows = 0;
		cols = 0;
		size = 0;
	}
	maze(string file):maze() {
		loadFile(file);
	}

	//getters
	int getStart() {
		return start;
	}
	int getEnd() {
		return end;
	}
	int getRows() {
		return rows;
	}
	int getCols() {
		return cols;
	}
	int getSize() {
		return size;
	}

	//filehandling functions
	bool loadFile(string file) {
		ifstream fin(file);
		if (!fin.is_open())
			return false;

		fin >> rows >> cols;	//read rows and cols from file
		size = rows * cols;		//set size
		vertices.resize(size);	//set size of vertices

		for (int i = 0; i < size; i++) {
			//get value of vertex
			int val;
			fin >> val;

			vertices[i].setId(i);
			vertices[i].setValue(val);

			//update start and end points of map
			if (vertices[i].getValue() == 2) {
				start = i;
				vertices[i].setShortestDist(0);//shortest distance to startPoint is 0
			}
			else if (vertices[i].getValue() == 3)
				end = i;

			//set vertex position on screen
			int x = (i % cols + 1) * 15;
			int y = (i / cols + 1) * 15;
			vertices[i].setPosition((float) x, (float) y);

			//create edges if tile is a path tile
			if (vertices[i].getValue() != 0) {
				if (i % cols != cols - 1)					//if not in the last column
					vertices[i].addEdge(vertices[i + 1]);	//create edge with right neighbour
				if (i + cols < size)						//if not in the last row
					vertices[i].addEdge(vertices[i + cols]);//create edge with down neightbour
			}
			//remove all edges if tile is a wall tile
			else {
				vertices[i].clearEdges();
			}
		}
		fin.close();
		setRemainingDistances();
		return true;

	}

	//access functions
	vertex& operator[](int n) {
		if (n < size)
			return vertices[n];

		return vertices[0];
	}
	vertex startVertex() {
		return vertices[start];
	}
	vertex endVertex() {
		return vertices[end];
	}

	//graphics functions
	void draw() {//creates a window and draws the maze onto it
		//setup window and border
		int dimx = (cols + 2) * 15;
		int dimy = (rows + 2) * 15;
		sf::RenderWindow window(sf::VideoMode(dimx, dimy), "maze");
		sf::RectangleShape border;
		border.setPosition({ 0,0 });
		border.setSize(sf::Vector2f((float)dimx, (float)dimy));
		border.setFillColor(sf::Color(50, 50, 50));

		while (window.isOpen()) {

			sf::Event event;
			while (window.pollEvent(event)) {
				switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::KeyPressed:
					window.close();
				default:
					break;
				}
			}
			window.clear();
			window.draw(border);
			for (int i = 0; i < (int)vertices.size(); i++)//draw all vertices on window
				vertices[i].draw(window);

			window.display();
		}
	}
	void draw(sf::RenderWindow& window) {//draws maze on given window
		for (int i = 0; i < size; i++) {
			vertices[i].draw(window);
		}
	}
	void setWindow(sf::RenderWindow& window) {//setup given window according to maze dimensions
		int dimx = (cols + 2) * 15;
		int dimy = (rows + 2) * 15;
		window.create(sf::VideoMode(dimx, dimy), "maze", sf::Style::Close);
	}
	void setBorder(sf::RectangleShape& border) {//setup border for window according to maze dimensions
		int dimx = (cols + 2) * 15;
		int dimy = (rows + 2) * 15;
		border.setPosition({ 0,0 });
		border.setSize(sf::Vector2f((float)dimx, (float)dimy));
		border.setFillColor(sf::Color(50, 50, 50));
	}
	
	//utility functions
	void printAll() {//prints details of all vertices in maze
		cout << "id\tvalue\tendDist\tadjacent\n";
		for (int i = 0; i < (int)vertices.size(); i++)
			cout << vertices[i];
	}
	void print() {//prints details of all non wall vertices in maze
		cout << "id\tvalue\tendDist\tadjacent\n";
		for (int i = 0; i < (int)vertices.size(); i++)
			if (vertices[i].getValue() != 0)
				cout << vertices[i];
	}
	void reset() {//resets all vertices
		for (int i = 0; i < size; i++) {
			vertices[i].reset();
			vertices[i].setRemainingDist(vertices[end]);
		}
		vertices[start].setShortestDist(0);
	}

	~maze() {
		//cout << "~maze() called\n";
	}
};