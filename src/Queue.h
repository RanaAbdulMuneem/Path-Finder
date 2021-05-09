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
#include "Maze.h"

class queue {
protected:
	vector<vertex*> heap;	//array of vertex pointers
	int size;				//size of queue

	void swap(vertex*& v1, vertex*& v2) {//function to swap two entries of heap
		vertex* temp = v1;
		v1 = v2;
		v2 = temp;
	}
public:
	//constructors
	queue() {
		//cout << "queue() called\n";
		size = 0;
	}
	queue(maze& m):queue(){
		loadMaze(m);
	}
	void loadMaze(maze& m) {//loads path vertices from given maze
		heap.resize(m.getSize());
		size = 0;
		for (int i = 0; i < m.getSize(); i++) {//for all vertices in maze
			if (m[i].getValue() != 0) {//if it is a non-wall vertex
				heap[size] = &m[i];//place it in queue
				size++;
			}
		}
		heap.resize(size);
		buildHeap();
	}

	//heap functions
	void buildHeap() {
		int startIndex = size / 2 - 1;//start from last non leaf node
		for (int i = startIndex; i >= 0; i--)
			heapify(size, i);
	}
	virtual bool isLessThan(vertex* v1, vertex* v2) {//compares two vertices on basis of shortestDist (v1 < v2)
		
		//-1 represents a very large number
		if (v1->getShortestDist() == -1)
			return false;
		else if (v2->getShortestDist() == -1)
			return true;
		else
			return (v1->getShortestDist() < v2->getShortestDist());

	}
	void heapify(int currentSize, int start) {
		int smallest = start;
		int left = start * 2 + 1;
		int right = start * 2 + 2;

		if (left < size && isLessThan(heap[left], heap[smallest]))
			smallest = left;
		if (right < size && isLessThan(heap[right], heap[smallest]))
			smallest = right;

		if (smallest != start) {
			swap(heap[start], heap[smallest]);
			heapify(currentSize, smallest);
		}
	}
	vertex* top() {
		return heap[0];
	}
	void pop() {
		if (size != 0) {
			size--;
			swap(heap[0], heap[size]);
			buildHeap();
		}
	}
	bool empty() {
		return (size == 0);
	}

	//getters
	int getSize() {
		return size;
	}

	//utility
	virtual void print() {//prints details of all vertices in queue
		cout << "index\tdist\tid\n";
		for (int i = 0; i < size; i++) {
			cout << i << '\t' << heap[i]->getShortestDist() << '\t';
			cout << heap[i]->getId() << '\n';
		}
	}

	~queue() {
		//cout << "~queue() called\n";
	}
};

class queueA :public queue {//queue used for A* algorithm
public:
	queueA() {
		cout << "queueA() called\n";
	}
	queueA(maze& m):queue(m) {
	}

	bool isLessThan(vertex* v1, vertex* v2) {//compares two vertices on basis of totalDist (v1 < v2)
		
		//-1 represents a very large number
		if (v1->getTotalDist() == -1)
			return false;
		else if (v2->getTotalDist() == -1)
			return true;
		else
			return (v1->getTotalDist() < v2->getTotalDist());
	}
	void print() {
		cout << "index\tdist\tid\tvalue\n";//prints details of all vertices in queue
		for (int i = 0; i < size; i++) {
			cout << i << '\t' << heap[i]->getTotalDist() << '\t';
			cout << heap[i]->getId() << '\t' << heap[i]->getValue();
			cout << endl;
		}
	}
};