#ifndef UTILITY_H
#define UTILITY_H

#include "ONP.h"

struct Range {
	double begin;
	double end;
};

struct PopulationStruct {
	double* functionArguments;
	double fitness;

	PopulationStruct(const int numberOfArguments);
	~PopulationStruct();
};

struct Population {
	PopulationStruct** population;
	int numberOfArguments;
	int populationCount;

	Population(const int numberOfArguments, const int populationCount);
	~Population();
};

double GenerateRandom(double min, double max);

void CalculateFitness(Population* population, ONP onp);

double GetFitnessSum(Population* population);

double GetMinFitness(Population* population);

double GetMaxFitness(Population* population);

int GetBestPopulation(Population* population);

int RouletteSelection(Population* population);

int TournamentSelection(Population* population);

void NewGeneration(Population* population, Range* argumentRanges);

double* CrossOver(double* first, double* second, int numberOfValues);

void MutatePopulation(Population* population, Range* argumentRanges);
void Mutate(double* number, Range range);

#endif