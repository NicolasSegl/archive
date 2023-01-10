#include "Application.h"

Application::Application(int windowWidth, int windowHeight)
{
	renderer.create(windowWidth, windowHeight);
	currentAlgorithm = SortingAlgorithm::SELECTION_SORT; // default to selection sort
	close = false;
}

// returns true if the window should close
bool Application::handleEvents()
{
	sf::Event event;
	while (renderer.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			renderer.close();
			return true;
		}
	}
	return false;
}

void Application::getUserInput()
{
	int input;
	while (true)
	{
		std::cout << "How many nodes would you like to sort?\n";
		std::cout << "2, 4, 8, 16, 32, 64, 128, 256, 512, 1024\n";
		std::cin >> input;
		if (std::cin.fail() || !(renderer.getWindowWidth() % input == 0) || input == 1)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(256, '\n');
			continue;
		}
		else
		{
			std::cout << std::endl;
			break;
		}
	}

	char repeatNodesInput;
	while (true)
	{
		std::cout << "Would you like each node value to be unique? (Y/N)\n";
		std::cin >> repeatNodesInput;
		if (std::cin.fail() || (repeatNodesInput != 'Y' && repeatNodesInput != 'N'))
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(256, '\n');
			continue;
		}
		else
		{
			std::cout << std::endl;
			break;
		}
	}
	if (repeatNodesInput == 'N')
		nodeHandler.repeatNodeValues = true;
	else
		nodeHandler.repeatNodeValues = false;

	nodeHandler.setNumOfNodes(input);
	nodeHandler.createNodes(renderer.getWindowWidth(), renderer.getWindowHeight());

	while (true)
	{
		std::cout << "Which sorting algorithm do you want to use?\n";
		std::cout << "1. Selection Sort\n";
		std::cout << "2. Insertion Sort\n";
		std::cout << "3. Bubble Sort\n";
		std::cout << "4. Heap Sort\n";
		std::cout << "5. Quick Sort\n";
		std::cout << "6. Merge Sort\n";
		std::cout << "7. Shell Sort\n";
		std::cout << "8. Bogo Sort\n";
		std::cout << "9. Radix Sort\n";
		std::cin >> input;
		if (std::cin.fail() || input < 1 || input >= (int)SortingAlgorithm::MAX_ALGORITHMS)
		{
			std::cout << "Invalid input\n";
			std::cin.clear();
			std::cin.ignore(256, '\n');
			continue;
		}
		else
			break;
	}
	system("cls"); // to clear stdout. Application::clearConsole() wasn't working for some reason
	currentAlgorithm = (SortingAlgorithm)input;
}

void Application::restart()
{
	char input;
	while (true)
	{
		std::cout << "Would you like to run the visualizer again? (Y/N): ";
		std::cin >> input;
		if (std::cin.fail() || (input != 'Y' && input != 'N'))
		{
			std::cin.clear();
			std::cin.ignore();
			continue;
		}
		else
			break;
	}
	if (input == 'N')
		close = true;
	else if (input == 'Y')
	{
		// reset stats and make sure the node handler knows it has restarted
		nodeHandler.restart = true;
		nodeHandler.setArrayAccesses(0);
		nodeHandler.setComparisons(0);
		getUserInput();
	}
}

void Application::displayStats()
{
	std::string delay = "Delay: " + std::to_string(nodeHandler.getTimeDelay(currentAlgorithm)) + " milliseconds";
	renderer.displayText(0, 0, delay);
	std::string arrayAccesses = "Array Accesses: " + std::to_string(nodeHandler.getArrayAccesses());
	renderer.displayText(0, 25, arrayAccesses);
	std::string comparisons = "Comparisons: " + std::to_string(nodeHandler.getComparisons());
	renderer.displayText(0, 50, comparisons);
}

void Application::sort()
{
	// each algorithm returns true if it's done sorting

	switch (currentAlgorithm)
	{
	case SortingAlgorithm::SELECTION_SORT:
		if (nodeHandler.selectionSort())
			restart();
		break;
	case SortingAlgorithm::INSERTION_SORT:
		if (nodeHandler.insertionSort())
			restart();
		break;
	case SortingAlgorithm::BUBBLE_SORT:
		if (nodeHandler.bubbleSort())
			restart();
		break;
	case SortingAlgorithm::HEAP_SORT:
		if (nodeHandler.heapSort())
			restart();
		break;
	case SortingAlgorithm::QUICK_SORT:
		if (nodeHandler.quickSort())
			restart();
		break;
	case SortingAlgorithm::MERGE_SORT:
		if (nodeHandler.mergeSort())
			restart();
		break;
	case SortingAlgorithm::SHELL_SORT:
		if (nodeHandler.shellSort())
			restart();
		break;
	case SortingAlgorithm::BOGO_SORT:
		if (nodeHandler.bogoSort())
			restart();
		break;
	case SortingAlgorithm::RADIX_SORT:
		if (nodeHandler.radixSort())
			restart();
		break;
	}
}

// comment code

void Application::run()
{
	getUserInput();

	while (true)
	{
		if (handleEvents())
			break;

		sort();

		if (close)
			break;

		renderer.clear();

		for (int i = 0; i < nodeHandler.nodes.size(); i++)
			renderer.render(nodeHandler.nodes[i]);

		displayStats();
		renderer.update();
	}
}