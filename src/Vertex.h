#pragma once

#ifdef _WIN32
#include <SFML\Graphics.hpp>
#endif
#ifdef linux
#include <SFML/Graphics.hpp>
#endif

#include <iostream>
#include <list>
#include <math.h>
#include <iterator>

using namespace std;

//vertex types
//value = 0	wall tile		(black)
//value = 1	path tile		(white)
//value = 2	start tile		(green)
//value = 3	end tile		(red)
//value = 4	visited tile	(yellow)
//value = 5	solution tile	(light green)


class vertex{
	int id;					//id of vertex
	int value;				//vertex type
	sf::RectangleShape tile;//object displayed on screen

	int shortestDist;		//shortest distance from start vertex
	int remainingDist;		//euclidean distance from end vertex
	int totalDist;			//sum of above distances. Heuristic for A* algorithm

public:
	list<vertex*> adjacent;	//adjacent vertices
	vertex* previous;		//previous vertex in solution path
	
	//constructors
	vertex() {
		//cout << "vertex() called\n";
		id = -1;
		shortestDist = -1;
		remainingDist = -1;
		totalDist = -1;
		previous = nullptr;

		setValue(-1);
		tile.setSize(sf::Vector2f(15,15));
		tile.setFillColor(sf::Color::Transparent);
		tile.setOutlineThickness(1);
		tile.setOutlineColor(sf::Color::Black);
	}
	vertex(int newId, int val):vertex() {
		setId(newId);
		setValue(val);
	}

	//setters
	void setId(int newId) {
		id = newId;
	}
	void setValue(int val) {
		value = val;
		setColor();
	}
	void setColor() {//sets color of tile depending upong value
		switch (value) {
		case 0:
			tile.setFillColor(sf::Color::Black);
			break;
		case 1:
			tile.setFillColor(sf::Color::White);
			break;
		case 2:
			tile.setFillColor(sf::Color::Green);
			break;
		case 3:
			tile.setFillColor(sf::Color::Red);
			break;
		case 4:
			tile.setFillColor(sf::Color::Yellow);
			break;
		case 5:
			tile.setFillColor(sf::Color(90, 240, 90));
			break;
		default:
			tile.setFillColor(sf::Color::Transparent);
			break;
		}
	}
	void setPosition(int x, int y) {//sets position of tile on a window
		tile.setPosition((float)x, (float)y);
	}
	void setShortestDist(int d) {
		shortestDist = d;
		totalDist = shortestDist + remainingDist;//update totalDist
	}
	void setRemainingDist(const vertex& end) {
		int x = end.tile.getPosition().x;
		int y = end.tile.getPosition().y;
		x -= tile.getPosition().x;
		y -= tile.getPosition().y;

		remainingDist = sqrt(x*x + y*y);
	}

	//getters
	int getValue() {
		return value;
	}
	int getId() {
		return id;
	}
	int getShortestDist() {
		return shortestDist;
	}
	int getTotalDist() {
		return totalDist;
	}

	//edge functions
	void addEdge(vertex& vertex2) {//add an edge
		adjacent.push_back(&vertex2);		//create edge from v1 to v2
		vertex2.adjacent.push_back(this);	//create edge from v2 to v1
	}
	void removeEdge(vertex& vertex2) {//remove an edge
		adjacent.remove(&vertex2);		//remove edge from v1 to v2
		vertex2.adjacent.remove(this);	//remove edge from v2 to v1
	}
	void clearEdges() {//remove all edges
		while (!adjacent.empty())
			removeEdge(*adjacent.front());
	}

	//operators
	friend ostream& operator<<(ostream& out, vertex& obj) {
		out << obj.id << '\t' << obj.value << '\t';//		output id and value of this vertex
		list<vertex*>::iterator i = obj.adjacent.begin();
		for (i; i != obj.adjacent.end(); i++) {
			out << (*i)->getId() << '\t';//					output ids of all adjacent vertices
		}
		out << endl;
		return out;
	}

	//graphics functions
	void draw(sf::RenderWindow& window) {//draw tile on given window
		window.draw(tile);
	}

	//utility
	void reset() {//resets distances and value of a vertex
		shortestDist = -1;
		remainingDist = -1;
		totalDist = -1;
		if (value == 4 || value == 5)
			setValue(1);
		previous = nullptr;
	}

	//destructor
	~vertex() {
		//cout << "~vertex() called\n";
	}
};
