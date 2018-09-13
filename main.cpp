#include <iostream>
#include "Utility.h"
#include "ONP.h"
#include <time.h>

using namespace std;

int main() {
	srand(time(NULL));

	// read input
	char* line;
	char* currentLineInput;

	bool endOfComments = false;

	size_t lineSize = 1000;

	int timeLimit;
	int numberOfArguments;

#pragma region BASIC_READ

	// skip x lines beginning with #
	while (!endOfComments) {
		line = (char*)malloc(lineSize * sizeof(char));

		cin.getline(line, lineSize);

		if (line != NULL && line[0] != '#') {
			endOfComments = true;

			// first line of proper input
			currentLineInput = strtok(line, " ");
			numberOfArguments = atoi(currentLineInput);
			currentLineInput = strtok(NULL, " ");
			timeLimit = atoi(currentLineInput);

			currentLineInput = NULL;
			free(currentLineInput);
		}

		line = NULL;
		free(line);
	}

	Range* argumentRanges = new Range[numberOfArguments];

	// read argument ranges
	for (int i = 0; i < numberOfArguments; i++) {
		cin >> argumentRanges[i].begin >> argumentRanges[i].end;
	}

#pragma endregion

	// read rpn function
	ONP* onp = new ONP();
	onp->ReadOperations();

#pragma region POPULATION_INIT

	// randomly create initial population
	// population is array of all variables
	const int POPULATION_NUMBER = 20;

	Population* population = new Population(numberOfArguments, POPULATION_NUMBER);

	// fill initial population with random values
	for (int i = 0; i < population->populationCount; i++) {
		for (int j = 0; j < population->numberOfArguments; j++) {
			population->population[i]->functionArguments[j] = GenerateRandom(argumentRanges[j].begin, argumentRanges[j].end);
		}
	}

#pragma endregion

	CalculateFitness(population, *onp);

	int win = GetBestPopulation(population);
	double bestFit = population->population[win]->fitness;
	cout << "Fitness: " << bestFit << endl;

	// MAIN LOOP:
	// - SELECTION WITH CROSSOVER
	// - MUTATION

	// TERMINATION:
	// - fixed number of generations reached
	// - next x iterations no longer produce better results

	int noImprovement = 0;

	cout << "GENERATION 1\n";
	for (int i = 0; i < population->populationCount; i++) {
		for (int j = 0; j < population->numberOfArguments; j++) {
			cout << population->population[i]->functionArguments[j] << " ";
		}
		cout << endl;
	}
	cout << endl;

	// new population
	for (int i = 0; i < 1000 * timeLimit; i++) {


		NewGeneration(population, argumentRanges);

		cout << "GENERATION " << i + 2 << endl;
		for (int i = 0; i < population->populationCount; i++) {
			for (int j = 0; j < population->numberOfArguments; j++) {
				cout << population->population[i]->functionArguments[j] << " ";
			}
			cout << endl;
		}

		CalculateFitness(population, *onp);

		win = GetBestPopulation(population);
		double currentFit = population->population[win]->fitness;
		cout << "Fitness: " << currentFit << endl;

		if (abs(currentFit - bestFit) < 0.001) {
			if (noImprovement > 15) {
				break;
			}
			else {
				noImprovement++;
			}
		}
		else {
			bestFit = currentFit;
			noImprovement = 0;
		}
	}

	cout << "FINAL: " << bestFit << endl;

	int tmp = (int)(bestFit * 100);
	double answer = tmp;
	answer /= 100;

	cout << "ANSWER: " << answer << endl;

	delete onp;
	delete population;

	return 0;
}