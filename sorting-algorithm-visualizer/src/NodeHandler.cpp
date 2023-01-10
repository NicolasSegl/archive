#include "NodeHandler.h"

NodeHandler::NodeHandler()
{
	std::srand(time(0));
	numOfNodes = 0;
	arrayAccesses = 0;
	comparisons = 0;
	restart = false;
	repeatNodeValues = false;
	setupTimeDelays();
}

void NodeHandler::setupTimeDelays()
{
	timeDelays[(int)SortingAlgorithm::SELECTION_SORT] = (int)AlgoTimeDelays::SELECTION_SORT;
	timeDelays[(int)SortingAlgorithm::INSERTION_SORT] = (int)AlgoTimeDelays::INSERTION_SORT;
	timeDelays[(int)SortingAlgorithm::BUBBLE_SORT]    = (int)AlgoTimeDelays::BUBBLE_SORT;
	timeDelays[(int)SortingAlgorithm::HEAP_SORT]      = (int)AlgoTimeDelays::HEAP_SORT;
	timeDelays[(int)SortingAlgorithm::QUICK_SORT]     = (int)AlgoTimeDelays::QUICK_SORT;
	timeDelays[(int)SortingAlgorithm::MERGE_SORT]     = (int)AlgoTimeDelays::MERGE_SORT;
	timeDelays[(int)SortingAlgorithm::SHELL_SORT]     = (int)AlgoTimeDelays::SHELL_SORT;
	timeDelays[(int)SortingAlgorithm::BOGO_SORT]      = (int)AlgoTimeDelays::BOGO_SORT;
	timeDelays[(int)SortingAlgorithm::RADIX_SORT]     = (int)AlgoTimeDelays::RADIX_SORT;
}

int NodeHandler::getTimeDelay(SortingAlgorithm x)
{
	return timeDelays[(int)x] * (256 / nodes.size());
}

long NodeHandler::getArrayAccesses()
{
	return arrayAccesses;
}

long NodeHandler::getComparisons()
{
	return comparisons;
}

void NodeHandler::setArrayAccesses(long x)
{
	arrayAccesses = x;
}

void NodeHandler::setComparisons(long x)
{
	comparisons = x;
}

void NodeHandler::setNumOfNodes(int x)
{
	numOfNodes = x;
}

void NodeHandler::clearNodes()
{
	for (int i = 0; i < nodes.size(); i++)
		if (nodes[i])
			delete nodes[i];
	nodes.clear();
}

void NodeHandler::randomizeNodeValues()
{
	nodeValues.clear();
	// fill the vector
	if (repeatNodeValues)
		for (int i = 0; i < numOfNodes; i++)
			nodeValues.push_back(rand() % numOfNodes);
	else
		for (int i = 0; i < numOfNodes; i++)
			nodeValues.push_back(i);
	// randomize the order of the vector 
	for (int i = 0; i < numOfNodes; i++)
		std::swap(nodeValues[i], nodeValues[rand() % numOfNodes]);
}

void NodeHandler::createNodes(int width, int height)
{
	clearNodes();
	randomizeNodeValues();
	for (int i = 0; i < numOfNodes; i++)
	{
		int newWidth = width / numOfNodes;
		int newHeight = round((float)height / numOfNodes) * nodeValues[i];
		int newY = height - round((float)height / numOfNodes * nodeValues[i]);
		int newX = newWidth * i;

		Node* newNode = new Node(newX, newY, newWidth, newHeight, nodeValues[i], numOfNodes);
		nodes.push_back(newNode);
	}
}

void NodeHandler::nodeSwap(Node* n1, Node* n2)
{
	// the array is ordered, that's why y and nodevalue are correct in the list. i need only change the x
	int x = n1->rect.getPosition().x;
	n1->rect.setPosition(sf::Vector2f(n2->rect.getPosition().x, n1->rect.getPosition().y));
	n2->rect.setPosition(sf::Vector2f(x, n2->rect.getPosition().y));
}

// make the nodes green at the end of the algorithm
bool NodeHandler::finishAnimation()
{
	static int i = -1;
	i++;
	std::this_thread::sleep_for(std::chrono::milliseconds(255 / nodes.size())); // slow the animation a little, relative to the number of total nodes
	if (i < nodes.size())
		nodes[i]->rect.setFillColor(sf::Color::Green);
	else
	{
		if (i > nodes.size())
		{
			i = -1;
			return true;
		}
		else if (i == nodes.size())
		{
			// reset the colours of the nodes
			for (int i = 0; i < nodes.size(); i++)
				nodes[i]->rect.setFillColor(sf::Color::White);
		}
	}
	return false;
}

void NodeHandler::reset(int& i)
{
	if (restart)
	{
		i = 0;
		restart = false;
	}
}

bool NodeHandler::selectionSort()
{
	static int i = 0;
	reset(i);

	static int previousSwapIndex = 0;
	if (i < nodes.size() - 1)
	{
		int swapIndex = i;
		// see which node in front of the considered one is the smallest, and switch places with it
		for (int j = i + 1; j < nodes.size(); j++)
			if (nodes[j]->nodeValue < nodes[swapIndex]->nodeValue)
			{
				swapIndex = j;
				comparisons++;
				arrayAccesses += 2;
			}

		std::swap(nodes[i], nodes[swapIndex]);
		nodeSwap(nodes[i], nodes[swapIndex]);

		// give the nodes their appropriate colours
		nodes[i]->rect.setFillColor(sf::Color::Green);
		nodes[swapIndex]->rect.setFillColor(sf::Color::Red);
		if (i > 0)
		{
			nodes[i - 1]->rect.setFillColor(sf::Color::White);
			nodes[previousSwapIndex]->rect.setFillColor(sf::Color::White);
		}
		previousSwapIndex = swapIndex;
	}
	i++;
	// ensure all nodes are white when the algorithm is finished
	if (i == nodes.size())
	{
		nodes[previousSwapIndex]->rect.setFillColor(sf::Color::White);
		for (int j = 1; j <= 2; j++)
			nodes[i - j]->rect.setFillColor(sf::Color::White);
	}

	if (i > nodes.size())
		return finishAnimation();
	// slow down the algorithm so the visualization doesn't happen too fast
	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::SELECTION_SORT) * (512 / nodes.size())));
	return i > nodes.size();
}

bool NodeHandler::insertionSort()
{
	static int i = 1;
	reset(i);

	// make all the previously red nodes white
	static std::vector<Node*> toBeWhiteNodes;
	for (int i = 0; i < toBeWhiteNodes.size(); i++)
		toBeWhiteNodes[i]->rect.setFillColor(sf::Color::White);
	toBeWhiteNodes.clear();

	if (i < nodes.size())
	{
		Node* key = nodes[i];
		int j = i - 1;

		// compare the key with each node to the left, and move the one to the left to the right if the key is less than it
		while (j >= 0 && key->nodeValue < nodes[j]->nodeValue)
		{
			nodes[j + 1] = nodes[j];
			nodeSwap(nodes[i], nodes[j]);
			j--;

			arrayAccesses++;
			comparisons++;
		}
		toBeWhiteNodes.push_back(nodes[j + 1]);
		nodes[j + 1]->rect.setFillColor(sf::Color::Red);
		nodes[j + 1] = key;
		// make the nodes the appropriate colours
		nodeSwap(nodes[i], nodes[j + 1]);
		nodes[i]->rect.setFillColor(sf::Color::Green);
		toBeWhiteNodes.push_back(nodes[i]);

		arrayAccesses++;
	}
	i++;
	// ensure that the all the nodes are white by the time the algorithm is finished
	if (i == nodes.size())
		for (int i = 0; i < toBeWhiteNodes.size(); i++)
			toBeWhiteNodes[i]->rect.setFillColor(sf::Color::White);

	if (i > nodes.size())
		return finishAnimation();
	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::INSERTION_SORT) * (512 / nodes.size())));
	return i > nodes.size();
}

bool NodeHandler::bubbleSort()
{
	static int numOfSwaps = 0;
	static int i = 0;
	static std::vector<int> toBeWhites;
	static int green = 0;
	// this variable ensures that the algorithm doesn't reiterate over nodes already in their final spot
	static int largest = nodes.size();
	// reset largest if the user wants to restart. plus 1 because in the if it subtracts 1
	if (restart)
		largest = nodes.size() + 1;

	// < the final element placed
	if (i < largest - 1 && !restart)
	{
		// check to see if the adjacent node is less than the current node
		if (nodes[i + 1]->nodeValue < nodes[i]->nodeValue)
		{
			std::swap(nodes[i + 1], nodes[i]);
			nodeSwap(nodes[i + 1], nodes[i]);
			nodes[i + 1]->rect.setFillColor(sf::Color::Green);
			toBeWhites.push_back(i + 1);
			green = i + 1;
			numOfSwaps++;

			arrayAccesses += 2;
			comparisons++;
		}
		i++;
	}
	else
	{
		largest--;
		for (int i = 0; i < toBeWhites.size(); i++)
			nodes[toBeWhites[i]]->rect.setFillColor(sf::Color::White);
		toBeWhites.clear();

		// if no swaps occurred, finish the algorithm (as that would mean it's sorted)
		if (numOfSwaps == 0 && !restart)
			return finishAnimation();

		numOfSwaps = 0;
		restart = false;
		green = 0;
		i = 0;
	}
	// make all the nodes before the green node red
	for (int j = 0; j < green; j++)
	{
		nodes[j]->rect.setFillColor(sf::Color::Red);
		toBeWhites.push_back(j);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::BUBBLE_SORT) * (512 / nodes.size())));
	return false;
}

void NodeHandler::heapify(int size, int index)
{
	static int toBeWhite = 0; // for visualization

	int largest = index;       // initialize largest as the index passed in
	int left = index * 2 + 1;  // get the left node from the heap in an array format
	int right = index * 2 + 2; // get the right node fro mthe heap in an array format

	// if the left node is not already sorted and is greater than largest, switch it
	if (left < size && nodes[left]->nodeValue > nodes[largest]->nodeValue)
		largest = left;
	// if the right node is not already sorted and is greater than largest, switch it
	if (right < size && nodes[right]->nodeValue > nodes[largest]->nodeValue)
		largest = right;

	arrayAccesses += 4;
	comparisons += 2;
	// if the largest isn't the root, call the function recursively until a max heap is produced
	if (largest != index)
	{
		std::swap(nodes[index], nodes[largest]);
		// nodeswap here so the visualization works
		nodeSwap(nodes[index], nodes[largest]);
		heapify(size, largest);
	}
	else
	{
		nodes[index]->rect.setFillColor(sf::Color::Red);
		nodes[toBeWhite]->rect.setFillColor(sf::Color::White);
		toBeWhite = index;
	}
}

bool NodeHandler::heapSort()
{
	static int i = nodes.size() - 1;
	static bool start = true;
	static int j = nodes.size() / 2 - 1;

	if (restart)
	{
		i = nodes.size() - 1;
		j = nodes.size() / 2 - 1;
		start = true;
		restart = false;
	}
	// build the initial max heap (only occurs at the first iteration of the algorithm)
	if (start && j >= 0)
		heapify(nodes.size(), j);
	else
		start = false;

	if (i >= 0 && !start)
	{
		// i being the last index in the array that isn't sorted
		std::swap(nodes[0], nodes[i]);
		nodeSwap(nodes[0], nodes[i]);
		nodes[i]->rect.setFillColor(sf::Color::Green);
		if (i < nodes.size() - 1)
			nodes[i + 1]->rect.setFillColor(sf::Color::White);
		// 0 being the top of the heap
		heapify(i, 0);
	}
	else if (!start && i < 0)
		return finishAnimation();

	if (j < 0)
		i--;
	j--;
	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::HEAP_SORT) * (512 / nodes.size())));
	return false;
}

// redo quick sort()?

int NodeHandler::partition(int low, int high)
{
	// get the pivot, and make it the last element of the considered array
	int pivot = nodes[high]->nodeValue;
	static int j = low - 1;
	static int i = low - 1;
	static bool reset = false;

	static std::vector<int> toBeWhites;

	if (reset)
	{
		i = low - 1;
		j = i;
		reset = false;
	}
	for (int i = 0; i < toBeWhites.size(); i++)
		nodes[toBeWhites[i]]->rect.setFillColor(sf::Color::White);


	i++;
	if (i < high)
	{
		if (nodes[i]->nodeValue < pivot)
		{
			// place the number lower than the pivot in front of the last index that a lower number was moved to (starting at 0)
			j++;
			std::swap(nodes[i], nodes[j]);
			nodeSwap(nodes[i], nodes[j]);

			// add colours to the nodes
			nodes[i]->rect.setFillColor(sf::Color::Red);
			nodes[j]->rect.setFillColor(sf::Color::Red);
			toBeWhites.clear();
			toBeWhites.push_back(i);
			toBeWhites.push_back(j);

			arrayAccesses += 2;
			comparisons++;
		}
	}
	else
	{
		std::swap(nodes[j + 1], nodes[high]); // move the pivot to the roughly the middle of the array
		nodeSwap(nodes[j + 1], nodes[high]);
		reset = true;
		toBeWhites.clear();
		toBeWhites.push_back(i);
		toBeWhites.push_back(j + 1);

		return j + 1; // return the index of the pivot
	}

	return -3;
}

// the recursive version of quick sort is much simpler, but to visualize it with recursion is fucking hard
// so wubba lubba dub dub
bool NodeHandler::quickSort()
{
	static int low = 0;
	static int high = nodes.size() - 1;
	static int* stack = new int[high - low + 1]; // create a stack
	static int topOfStack = -1;
	static bool firstTimeSetup = true;
	static bool insideBool = true;

	if (restart)
	{
		low = 0;
		high = nodes.size() - 1;
		delete[] stack;
		stack = new int[high - low + 1];
		topOfStack = -1;
		firstTimeSetup = true;
		insideBool = true;
		restart = false;
	}

	if (firstTimeSetup)
	{
		stack[++topOfStack] = low;
		stack[++topOfStack] = high;
		firstTimeSetup = false;
	}

	// keep popping from stack while not empty
	if (topOfStack >= 0 || !insideBool)
	{
		if (insideBool)
		{
			high = stack[topOfStack--];
			low = stack[topOfStack--];
			insideBool = false;
		}

		int pivot = partition(low, high);
		if (pivot != -3)
		{
			// if there are elements to the right of the pivot
			if (pivot + 1 < high)
			{
				stack[++topOfStack] = pivot + 1;
				stack[++topOfStack] = high;
			}

			// if there are elements to the left of the pivot
			if (pivot - 1 > low)
			{
				stack[++topOfStack] = low;
				stack[++topOfStack] = pivot - 1;
			}
			insideBool = true;
		}
	}
	else
		return finishAnimation();
	return false;
}

int NodeHandler::merge(int leftStart, int middle, int rightEnd)
{
	static bool reset = true;
	static int i = 0;
	static int j = 0;
	static int k = leftStart;
	static int toBeWhite = 0;

	// get the amount of elements from the start to the middle
	static int firstHalfSize = middle - leftStart + 1;
	// get the amount of elements from the middle to the end
	static int secondHalfSize = rightEnd - middle;

	// create the subarrays
	static Node* firstHalf = new Node[firstHalfSize];
	static Node* secondHalf = new Node[secondHalfSize];

	if (reset)
	{
		i = 0;
		j = 0;
		k = leftStart;
		reset = false;
		toBeWhite = 0;

		firstHalfSize = middle - leftStart + 1;
		secondHalfSize = rightEnd - middle;

		delete[] firstHalf;
		delete[] secondHalf;
		firstHalf = new Node[firstHalfSize];
		secondHalf = new Node[secondHalfSize];

		// fill the subarrays
		for (int i = 0; i < firstHalfSize; i++)
			firstHalf[i] = *nodes[i + leftStart];
		for (int i = 0; i < secondHalfSize; i++)
			secondHalf[i] = *nodes[middle + i + 1];

		arrayAccesses += firstHalfSize + secondHalfSize;
	}

	nodes[k]->rect.setFillColor(sf::Color::Red);
	nodes[toBeWhite]->rect.setFillColor(sf::Color::White);
	toBeWhite = k;

	// change the values in the base array (NodeHandler::nodes)
	if (i < firstHalfSize && j < secondHalfSize)
	{
		if (firstHalf[i].nodeValue <= secondHalf[j].nodeValue)
		{
			*nodes[k] = firstHalf[i];
			i++;
		}
		else
		{
			*nodes[k] = secondHalf[j];
			j++;
			nodes[k]->rect.setFillColor(sf::Color::Red);
		}
		arrayAccesses += 2;
		comparisons++;
		k++;
	}
	else
	{
		// fill the rest of the array if the first half wasn't finished
		if (i < firstHalfSize)
		{
			*nodes[k] = firstHalf[i];
			i++;
			k++;
		}

		// fill the rest of the array if the second half wasn't finished
		if (j < secondHalfSize)
		{
			*nodes[k] = secondHalf[j];
			j++;
			k++;
		}
	}

	if (i >= firstHalfSize && j >= secondHalfSize)
	{
		reset = true;
		return 0; // the algorithm is finished
	}
	return 1;
}

int NodeHandler::getMinimum(int x, int y)
{
	if (x < y)
		return x;
	return y;
}

// creates a shit ton of subarrays then merges them together
bool NodeHandler::mergeSort()
{
	// current size of sub arrays to be merged. ranges from 1 to nodes.size() / 2
	static int sizeOfSubArrays = 1;
	// for picking the starting index of the left sub array to be merged
	static int leftSubArrayStart = 0;

	if (restart)
	{
		sizeOfSubArrays = 1;
		leftSubArrayStart = 0;
		restart = false;
	}

	// merge sub arrays in a bottom up manner
	// first merge sub arrays of size 1 to create sorted sub arryas of size 2, then merge
	// sub arrays of size 2 to create sorted sub arrays of size 4, and so on and so forth.
	if (sizeOfSubArrays <= nodes.size() - 1)
	{
		// pick the starting point of different subarrays of the current size
		if (leftSubArrayStart < nodes.size() - 1)
		{
			// find ending point of the left subarray
			int middle = getMinimum(leftSubArrayStart + sizeOfSubArrays - 1, nodes.size() - 1);
			// find the ending point of the right subarray
			int rightEnd = getMinimum(leftSubArrayStart + 2 * sizeOfSubArrays - 1, nodes.size() - 1);

			// merge subarrays from leftSubArrayStart to middle, and subarrays from middle + 1 to end of right subarray
			int result = merge(leftSubArrayStart, middle, rightEnd);

			// if the algorithm has completely finished
			if (result == 0)
				leftSubArrayStart += 2 * sizeOfSubArrays;
		}
	}
	else
		return finishAnimation();

	if (leftSubArrayStart == nodes.size())
	{
		leftSubArrayStart = 0;
		sizeOfSubArrays *= 2;
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::MERGE_SORT) * (512 / nodes.size())));
	return false;
}

bool NodeHandler::shellSort()
{
	// Rearrange elements at each n/2, n/4, n/8, ... intervals
	static int gap = nodes.size() / 2;
	static int i = gap;
	static int toBeWhite = 0;
	static Node* temp = nodes[i];
	static int j = i;

	if (restart)
	{
		toBeWhite = 0;
		gap = nodes.size() / 2;
		i = gap;
		j = i;
		temp = nodes[i];
		restart = false;
	}

	nodes[toBeWhite]->rect.setFillColor(sf::Color::White);
	if (gap > 0)
	{
		if (i < nodes.size())
		{
			if (j >= gap && nodes[j - gap]->nodeValue > temp->nodeValue)
			{
				std::swap(nodes[j], nodes[j - gap]);
				nodeSwap(nodes[j], nodes[j - gap]);
				nodes[j]->rect.setFillColor(sf::Color::Red);
				toBeWhite = j;

				j -= gap;
				arrayAccesses += 2;
				comparisons += 2;
			}
			else
			{
				nodes[j] = temp;
				j = i;
				temp = nodes[i];
				arrayAccesses++;

				i++;
			}
		}
		else
		{
			gap /= 2;
			i = gap;
		}
	}
	else
		return finishAnimation();
	return false;
}

bool NodeHandler::bogoSort()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::BOGO_SORT)));
	static bool sorted = false;
	if (restart)
	{
		sorted = false;
		restart = false;
	}

	if (!sorted)
	{
		// randomize the array
		for (int i = 0; i < nodes.size(); i++)
		{
			int randomIndex = rand() % nodes.size();
			std::swap(nodes[i], nodes[randomIndex]);
			nodeSwap(nodes[i], nodes[randomIndex]);
			arrayAccesses += 2;
		}

		// check if the array got sorted
		for (int i = 0; i < nodes.size() - 1; i++)
		{
			comparisons++;
			if (nodes[i]->nodeValue > nodes[i + 1]->nodeValue)
				return false;
		}
		sorted = true;
	}
	if (sorted)
		return finishAnimation();
}

bool NodeHandler::countingSort(int place)
{
	static int* aux = new int[nodes.size() + 1];
	static Node* newNodes = new Node[nodes.size()];
	static bool reset = true;
	static int i = nodes.size() - 1;
	static int j = 0;
	static int toBeWhite = 0;
	
	if (restart || reset)
	{
		reset = true;
		i = nodes.size() - 1;
		j = 0;
		toBeWhite = 0;
		restart = false;

		delete[] aux;
		delete[] newNodes;
		aux = new int[nodes.size() + 1];
		newNodes = new Node[nodes.size()];
	}

	if (reset)
	{
		// initialize each value in the auxillary array with 0
		for (int i = 0; i < nodes.size(); i++)
			aux[i] = 0;

		// store the "count" of each element (the number of times it appears in the array)
		for (int i = 0; i < nodes.size(); i++)
			aux[(nodes[i]->nodeValue / place) % 10]++;

		// store the cummulative sum of each array
		for (int i = 1; i <= nodes.size(); i++)
			aux[i] += aux[i - 1];
		reset = false;
		arrayAccesses += nodes.size() * 3;
	}

	// ensure the nodes' colours are appropriate
	nodes[toBeWhite]->rect.setFillColor(sf::Color::White);
	if (i >= 0)
	{
		nodes[i]->rect.setFillColor(sf::Color::Red);
		toBeWhite = i;
	}

	// sort the elements into the output array
	if (i >= 0)
	{
		newNodes[aux[(nodes[i]->nodeValue / place) % 10] - 1] = *nodes[i];
		aux[(nodes[i]->nodeValue / place) % 10]--;
		i--;
		arrayAccesses += 2;
		return false;
	}
	else
	{
		// copy the sorted elements into NodeHandler::nodes
		if (j < nodes.size())
		{
			*nodes[j] = newNodes[j];
			arrayAccesses += 2;
			j++;
			nodes[j - 1]->rect.setFillColor(sf::Color::Red);
			toBeWhite = j - 1;
			return false;
		}
	}

	reset = true;
	return true;
}

int NodeHandler::getMax()
{
	int max = nodes[0]->nodeValue;
	for (int i = 1; i < nodes.size(); i++)
		if (nodes[i]->nodeValue > max)
			max = nodes[i]->nodeValue;
	
	return max;
}

// calls counting sort using the values of the digits
bool NodeHandler::radixSort()
{
	static int max = getMax();
	static int place = 1;
	if (restart)
	{
		place = 1;
		max = getMax();
		restart = false;
	}

	if (max / place > 0)
	{
		if (countingSort(place))
		{
			place *= 10;
			return false;
		}
	}
	else
		return finishAnimation();

	std::this_thread::sleep_for(std::chrono::milliseconds(getTimeDelay(SortingAlgorithm::RADIX_SORT) * (512 / nodes.size())));
	return false;
}