#include <iostream>
#include <random>
#include <iomanip>
#include "headersolver.h"

int** createMatrix(int nCities) {
    int** matrix = new int* [nCities];

    for (int i = 0; i < nCities; i++) {
        matrix[i] = new int[nCities];
    }

    return matrix;
}

void destroyMatrix(int** matrix, int nCities) {
    for (int i = 0; i < nCities; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

void fillRandomMatrix(int** matrix, int nCities, int minCost, int maxCost) {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(minCost, maxCost);

    for (int i = 0; i < nCities; i++) {
        for (int j = 0; j < nCities; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            }
            else {
                matrix[i][j] = distribution(generator);
            }
        }
    }
}

void runExperiment(int nCities, int runIndex, int minRange, int maxRange) {
    int** matrix = createMatrix(nCities);

    fillRandomMatrix(matrix, nCities, minRange, maxRange);

    int startCity = 0;

    ExactResult exact = solveBruteForce(matrix, nCities, startCity);
    GreedyResult greedy = solveGreedy(matrix, nCities, startCity);

    double quality = calculateQuality(
        exact.minCost,
        exact.maxCost,
        greedy.cost
    );

    std::cout << "Run #" << runIndex
        << " | Exact [Min: " << exact.minCost
        << ", Max: " << exact.maxCost
        << ", Time: " << std::scientific
        << std::setprecision(2) << exact.timeInSeconds << "s]"
        << " | Greedy [Cost: " << std::defaultfloat << greedy.cost
        << ", Time: " << std::scientific
        << std::setprecision(2) << greedy.timeInSeconds << "s]"
        << " | Quality: " << std::fixed
        << std::setprecision(1) << quality << "%"
        << std::endl;

    destroyMatrix(matrix, nCities);
}

int main() {
    int testSizes[] = { 4, 6, 8, 10, 12 };

    int testSizesCount = sizeof(testSizes) / sizeof(testSizes[0]);

    int runsPerSize = 3;

    int minCost1 = 10;
    int maxCost1 = 100;

    std::cout << "    EXPERIMENT 1: Cost Range "
        << minCost1 << " - " << maxCost1
        << " ===" << std::endl;

    for (int i = 0; i < testSizesCount; i++) {
        int nCities = testSizes[i];

        std::cout << "\nDimension: "
            << nCities << "x" << nCities
            << std::endl;

        for (int run = 1; run <= runsPerSize; run++) {
            runExperiment(nCities, run, minCost1, maxCost1);
        }
    }

    int minCost2 = 10;
    int maxCost2 = 1000;

    std::cout << "\n EXPERIMENT 2  Cost Range "
        << minCost2 << " - " << maxCost2
        << " ===" << std::endl;

    for (int i = 0; i < testSizesCount; i++) {
        int nCities = testSizes[i];

        std::cout << "\nDimension: "
            << nCities << "x" << nCities
            << std::endl;

        for (int run = 1; run <= runsPerSize; run++) {
            runExperiment(nCities, run, minCost2, maxCost2);
        }
    }

    return 0;
}