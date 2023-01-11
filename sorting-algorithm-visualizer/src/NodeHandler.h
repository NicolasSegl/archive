#pragma once

#include <vector>
#include <time.h>
#include <random>
#include <algorithm>
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

#include "Node.h"
#include "Renderer.h"

#include <windows.h> 

enum class SortingAlgorithm
{
	SELECTION_SORT = 1,
	INSERTION_SORT = 2,
	BUBBLE_SORT = 3,
	HEAP_SORT = 4,
	QUICK_SORT = 5,
	MERGE_SORT = 6,
	SHELL_SORT = 7,
	BOGO_SORT = 8,
	RADIX_SORT = 9,
	MAX_ALGORITHMS,
};

enum class AlgoTimeDelays
{
	SELECTION_SORT = 50,
	INSERTION_SORT = 50,
	BUBBLE_SORT = 0,
	HEAP_SORT = 50,
	QUICK_SORT = 2,
	MERGE_SORT = 1,
	SHELL_SORT = 0,
	BOGO_SORT = 100,
	RADIX_SORT = 20,
};

using namespace std::chrono;

class NodeHandler
{
private:
	int numOfNodes;
	std::vector<int> nodeValues;
	long comparisons, arrayAccesses;
	int timeDelays[(int)SortingAlgorithm::MAX_ALGORITHMS];

public:
	NodeHandler();
	std::vector<Node*> nodes;
	bool restart;
	bool repeatNodeValues;

	void setNumOfNodes(int x);
	int getTimeDelay(SortingAlgorithm x);
	long getComparisons();
	long getArrayAccesses();
	void setComparisons(long x);
	void setArrayAccesses(long x);

	void setupTimeDelays();
	void createNodes(int width, int height);
	void clearNodes();
	void randomizeNodeValues();
	void nodeSwap(Node* n1, Node* n2);
	bool finishAnimation();
	void reset(int& i);

	bool selectionSort();
	bool insertionSort();
	bool bubbleSort();

	void heapify(int size, int index);
	bool heapSort();

	int partition(int low, int high);
	bool quickSort();

	int merge(int leftStart, int middle, int rightEnd);
	int getMinimum(int x, int y);
	bool mergeSort();

	bool shellSort();
	bool bogoSort();

	bool countingSort(int place);
	int getMax();
	bool radixSort();
};