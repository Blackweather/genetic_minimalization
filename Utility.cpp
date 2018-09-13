#include "Utility.h"

PopulationStruct::PopulationStruct(const int numberOfArguments) {
	functionArguments = new double[numberOfArguments];
}

PopulationStruct::~PopulationStruct() {
	delete[] functionArguments;
}

Population::Population(const int numberOfArguments, const int populationCount) {
	this->numberOfArguments = numberOfArguments;
	this->populationCount = populationCount;

	this->population = new PopulationStruct*[populationCount];

	for (int i = 0; i < populationCount; i++) {
		this->population[i] = new PopulationStruct(numberOfArguments);
	}
}

Population::~Population() {
	delete this->population;
}

double GenerateRandom(double min, double max) {
	double range = max - min;
	double div = RAND_MAX / range;
	return min + (rand() / div);
}

void CalculateFitness(Population* population, ONP onp) {
	for (int i = 0; i < population->populationCount; i++) {
		population->population[i]->fitness = onp.CalculateFunction(population->population[i]->functionArguments,
			population->numberOfArguments);
	}
}

double GetFitnessSum(Population* population) {
	double sum = 0.0;
	for (int i = 0; i < population->populationCount; i++) {
		sum += population->population[i]->fitness;
	}
	return sum;
}

double GetMinFitness(Population* population) {
	double minFit = population->population[0]->fitness;

	if (population->populationCount > 1) {
		for (int i = 1; i < population->populationCount; i++) {
			if (population->population[i]->fitness < minFit) {
				minFit = population->population[i]->fitness;
			}
		}
	}

	return minFit;
}

double GetMaxFitness(Population* population) {
	double maxFit = population->population[0]->fitness;

	if (population->populationCount > 1) {
		for (int i = 1; i < population->populationCount; i++) {
			if (population->population[i]->fitness > maxFit) {
				maxFit = population->population[i]->fitness;
			}
		}
	}

	return maxFit;
}

int GetBestPopulation(Population* population) {

	int popNumber = 0;

	if (population->populationCount > 1) {
		for (int i = 1; i < population->populationCount; i++) {
			if (population->population[i]->fitness < population->population[popNumber]->fitness) {
				popNumber = i;
			}
		}
	}

	return popNumber;
}

int RouletteSelection(Population* population) {
	double fitSum = GetFitnessSum(population);
	double minFit = GetMinFitness(population);
	double maxFit = GetMaxFitness(population);

	double p = (double)rand() / (double)RAND_MAX;
	p *= fitSum;

	double t = minFit + maxFit;

	int result = 0;
	for (int i = 0; i < population->populationCount; i++) {
		p -= (t - population->population[i]->fitness);
		if (p < 0) {
			result = i;
			break;
		}
	}
	return result;

}

int TournamentSelection(Population * population) {
	const int TOURNAMENT_SIZE = 8;
	int contestantsLeft = TOURNAMENT_SIZE;

	int* contestants = new int[TOURNAMENT_SIZE];

	// get random contestants
	for (int i = 0; i < TOURNAMENT_SIZE; i++) {
		contestants[i] = rand() % population->populationCount;
	}

	do {
		contestantsLeft /= 2;
		int* winners = new int[contestantsLeft];
		for (int i = 0; i < contestantsLeft; i++) {
			// contenstant[2*i] vs contestant[2*i+1]
			if (population->population[contestants[2 * i]]->fitness < population->population[contestants[(2 * i) + 1]]->fitness) {
				winners[i] = contestants[2 * i];
			}
			else {
				winners[i] = contestants[(2 * i) + 1];
			}
		}
		// new round
		delete[] contestants;
		contestants = new int[contestantsLeft];
		for (int i = 0; i < contestantsLeft; i++) {
			contestants[i] = winners[i];
		}
		delete[] winners;
	} while (contestantsLeft != 1);

	int winner = contestants[0];

	delete[] contestants;

	return winner;
}

void NewGeneration(Population* population) {
	// SELECTION:
	// half population through roulette select
	// half population - crossover from selected parents from first half
	// mutation

	Population* firstHalf = new Population(population->numberOfArguments,
		population->populationCount / 2);

	// fill with roulette select
	for (int i = 0; i < firstHalf->populationCount; i++) {
		//int selected = RouletteSelection(population);
		int selected = TournamentSelection(population);

		// copy array
		for (int j = 0; j < population->numberOfArguments; j++) {
			firstHalf->population[i]->functionArguments[j] =
				population->population[selected]->functionArguments[j];
		}
	}


	Population* secondHalf = new Population(population->numberOfArguments,
		population->populationCount - (population->populationCount / 2));

	for (int i = 0; i < secondHalf->populationCount; i++) {
		// fill with cross over
		int firstParent = rand() % firstHalf->populationCount;
		int secondParent;
		do {
			secondParent = rand() % firstHalf->populationCount;
		} while (secondParent == firstParent);

		double* child = CrossOver(firstHalf->population[firstParent]->functionArguments,
			firstHalf->population[secondParent]->functionArguments,
			firstHalf->numberOfArguments);

		// add child to second half
		secondHalf->population[i]->functionArguments = child;
	}

	// create new population from two halves
	int firstHalfLen = population->populationCount / 2;
	int secondHalfLen = population->populationCount - (population->populationCount / 2);

	// copy first half
	for (int i = 0; i < firstHalfLen; i++) {
		for (int j = 0; j < population->numberOfArguments; j++) {
			population->population[i]->functionArguments[j] =
				firstHalf->population[i]->functionArguments[j];
		}
	}

	// copy second half
	for (int i = 0; i < secondHalfLen; i++) {
		for (int j = 0; j < population->numberOfArguments; j++) {
			population->population[i + firstHalfLen]->functionArguments[j] =
				secondHalf->population[i]->functionArguments[j];
		}
	}

	// END OF SELECTION/CROSSOVER

	MutatePopulation(population);
}

double* CrossOver(double* first, double* second, int numberOfValues) {
	double* result = new double[numberOfValues];

	if (numberOfValues == 1) {
		// one point cross over makes no sense with two double values
		// would result in completely different number

		// get the average
		result[0] = (first[0] + second[0]) / 2;
		// add a small mutation
		double mutation = GenerateRandom(0.9, 1.0);

		result[0] *= mutation;
	}
	else {
		// one point cross over:
		// choose random point in array 

		int crossPoint = rand() % (numberOfValues - 1);

		// create a new offspring

		// first part - values from first parent
		for (int i = 0; i <= crossPoint; i++) {
			result[i] = first[i];
		}

		// second part - values from second parent
		for (int i = crossPoint + 1; i < numberOfValues; i++) {
			result[i] = second[i];
		}
	}
	return result;
}

void MutatePopulation(Population* population) {
	// MUTATION
	const int MUTATION_PROBABILITY = 0.02;

	for (int i = 0; i < population->populationCount; i++) {
		// each argument has probability to mutate
		for (int j = 0; j < population->numberOfArguments; j++) {

			double mutationTry = (double)rand() / RAND_MAX;

			if (mutationTry <= MUTATION_PROBABILITY) {
				Mutate(&population->population[i]->functionArguments[j]);
			}
		}
	}
}

void Mutate(double* number) {
	// generate a small number
	const double MIN = 0.1;
	const double MAX = 0.2;
	double mutation = GenerateRandom(MIN, MAX);

	// decide if addition or substraction
	double random = (double)rand() / RAND_MAX;

	if (random < 0.5) {
		mutation *= -1;
	}

	*number += mutation;
}