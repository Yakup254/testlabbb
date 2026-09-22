#pragma once

struct ExactResult {
    int minCost;
    int maxCost;
    double timeInSeconds;
};

struct GreedyResult {
    int cost;
    double timeInSeconds;
};

ExactResult solveBruteForce(int** matrix, int nCities, int startCity = 0);
GreedyResult solveGreedy(int** matrix, int nCities, int startCity = 0);
double calculateQuality(int minCost, int maxCost, int greedyCost);