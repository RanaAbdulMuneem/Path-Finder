#ifdef _WIN32
#include <SFML\Graphics.hpp>
#endif
#ifdef linux
#include <SFML/Graphics.hpp>
#endif

#include <iostream>
#include <chrono>
#include "Maze.h"
#include "Queue.h"

using namespace std;

//pathFinder() takes a graph of vertices, and a priority queue to create a path from a start point to end point
//whether Djikstra or A* algorithm is used depends on the queue object passed to the function
//initially distance to all vertices is infinite(-1), except for start vertex whose distance is 0
//pointers to all vertices are stored in the priority queue. The priority is on the basis of this distance (priority criteria is different for djikstra and A*)
//																lower distance == higher priority
//basic algorithm:
//		-get vertex from top of queue
//		-calculate new distance to get to this vertex's neighbour
//		-if new distance is shorter than the previous distance of neighbour
//			update the distance of neighbour vertex
//			set the previous of neighbour vertex to this vertex
//		
//		-remove this vertex from queue and mark this vertex as visited
//	repeat until end point is reached, or the queue is empty, or an infinite distance vertex is reached (no path condition)
void pathFinder(maze& map, queue& q) {

	vector<bool> visitList;		//previously visited vertices
	visitList.resize(map.getSize(), false);//initialize visitList, no vertex has been visited
	
	cout << "Press any key to continue\n";
	auto start = chrono::high_resolution_clock::now();
	map.draw();//display map before solving

	//finding a path from start to end
	vertex* thisVertex = q.top();
	while (thisVertex->getId() != map.getEnd() && thisVertex->getShortestDist() != -1 && !q.empty()) {
		//stop searching if the end vertex is reached, or the queue is empty, or if an unreachable vertex is reached (vertex having infinite distance)

		list<vertex*> adjacent = thisVertex->adjacent;//get adjacent vertices
		while (!adjacent.empty()) {	//for all adjacent vertices
			vertex* neighbour = adjacent.front();	//get adjacent vertex
			adjacent.pop_front();

			if (!visitList[neighbour->getId()]) {			//if vertex has not been visited before
				int oldDistance = neighbour->getShortestDist();		//distance to neighbour
				int newDistance = thisVertex->getShortestDist() + 1;	//distance to neighbour through this vertex

				if (oldDistance == -1 || newDistance < oldDistance) {
					neighbour->setShortestDist(newDistance);//update shortest distance
					neighbour->previous = thisVertex;		//update path
				}
			}
		}

		visitList[thisVertex->getId()] = true;			//mark this vertex as visited
		if (thisVertex->getValue() == 1)//if tile is a path tile
			thisVertex->setValue(4);//make it into a visited tile
		q.pop();		//remove vertex from queue
		thisVertex = q.top();		//get vertex from queue
	}

	//creating the path from start to end
	int pathLength = 0;							//keep track of path length
	if (map.endVertex().previous != nullptr) {	//if there is a way of reacing endpoint
		thisVertex = map.endVertex().previous;	//start from just before endpoint
		while (thisVertex != nullptr) {			//until start of path is reached
			if (thisVertex->getValue() == 4)	//if tile is a visited tile
				thisVertex->setValue(5);		//turn it into a solution tile
			
			thisVertex = thisVertex->previous;
			pathLength++;
		}
	}
	else
		cout << "NO PATH EXISTS\n";

	//output path length and time
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

	cout << "Path length:\t" << pathLength << endl;
	cout << "Time taken:\t" << duration.count() << " milliseconds\n";
	
	cout << "Press any key to continue\n";
	map.draw();
}
//same pathFinder() function but with additional graphics
void pathFinderG(maze& map, queue& q) {
	//setup sfml window
	sf::RenderWindow window;
	sf::RectangleShape border;
	map.setWindow(window);
	map.setBorder(border);

	if (q.getSize() <= 500)
		window.setFramerateLimit(120);

	window.draw(border);
	map.draw(window);
	window.display();

	bool solved = false;	//has maze been solved
	bool solve = false;		//has user asked for maze to be solved


	cout << "Press any key to continue\n";
	while (window.isOpen()) {
		sf::Event eventMain;
		while (window.pollEvent(eventMain)) {
			//if window close button is pressed, close the window
			if (eventMain.type == sf::Event::Closed) {
				window.close();
			}
			if (eventMain.type == sf::Event::KeyPressed) {
				//if space is pressed, start path finding
				if (eventMain.key.code == sf::Keyboard::Space) {
					if (!solved)
						solve = true;
					else
						window.close();
				}
			}
		}

		if (!solved && solve) {
			auto start = chrono::high_resolution_clock::now();

			vector<bool> visitList;		//previously visited vertices


			visitList.resize(map.getSize(), false);//initialize freeList, no vertex has been visited

			//finding a path from start to end
			vertex* thisVertex = q.top();
			while (thisVertex->getId() != map.getEnd() && thisVertex->getShortestDist() != -1 && !q.empty()) {
				//stop searching if the end vertex is reached, or the queue is empty, or if an unreachable vertex is reached (vertex having infinite distance)

				while (window.pollEvent(eventMain)) {
					if (eventMain.type == sf::Event::Closed) {
						window.close();
					}
				}

				list<vertex*> adjacent = thisVertex->adjacent;//get adjacent vertices
				while (!adjacent.empty()) {	//for all adjacent vertices
					vertex* neighbour = adjacent.front();	//get adjacent vertex
					adjacent.pop_front();

					if (!visitList[neighbour->getId()]) {			//if vertex has not been visited before
						int oldDistance = neighbour->getShortestDist();		//distance to neighbour
						int newDistance = thisVertex->getShortestDist() + 1;	//distance to neighbour through this vertex

						if (oldDistance == -1 || newDistance < oldDistance) {
							neighbour->setShortestDist(newDistance);//update shortest distance
							neighbour->previous = thisVertex;		//update path
						}
					}
				}

				visitList[thisVertex->getId()] = true;			//mark this vertex as visited
				if (thisVertex->getValue() == 1)// if tile is a path tile
					thisVertex->setValue(4);//make tile into a visited tile
				q.pop();		//remove vertex from queue

				if(!q.empty())
					thisVertex = q.top();		//get vertex from queue

				//update window
				window.clear();
				window.draw(border);
				map.draw(window);
				window.display();

			}

			//creating the path from start to end
			int pathLength = 0;							//keep track of path length
			if (map.endVertex().previous != nullptr) {	//if there is a way of reacing endpoint
				thisVertex = map.endVertex().previous;	//start from just before end point
				while (thisVertex != nullptr) {			//until start of path is reached
					if (thisVertex->getValue() == 4)	//if tile is a visited tile
						thisVertex->setValue(5);		//turn it into a solution tile
					thisVertex = thisVertex->previous;	//move to previous vertex in path
					pathLength++;

					//update window
					window.clear();
					window.draw(border);
					map.draw(window);
					window.display();
				}
			}
			else
				cout << "NO PATH EXISTS\n";

			auto stop = chrono::high_resolution_clock::now();
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

			cout << "Path length:\t" << pathLength << endl;
			cout << "Time taken:\t" << duration.count() << " milliseconds\n";

			cout << "Press any key to continue\n";
			solved = true;
			solve = false;
		}
	}
}

int main() {
	string file;
	cout << "Enter Maze file: ";
	cin >> file;
	maze* map = new maze;

	while (file != "exit") {
		while (!map->loadFile(file) && file != "exit") {
			cout << "File not found\n";
			cout << "Enter Maze file: ";
			cin >> file;
		}
		if (file != "exit") {
			cout << "Using DJIKSTRA's algorithm\n\n";
			queue qD(*map);
			pathFinderG(*map, qD);

			cout << "\n\n";
			map->reset();		//reset graph for A* algorithm

			cout << "Using A* algorithm\n\n";
			queueA qA(*map);
			pathFinderG(*map, qA);
			delete map;
			map = new maze;
			system("pause");
			system("cls");
			cout << "Enter Maze file: ";
			cin >> file;
		}
	}
	delete map;
	return 0;
}