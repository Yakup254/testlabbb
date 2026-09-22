#include <iostream>
#include <chrono>
#include "headersolver.h"

namespace {

    void swap(int& a, int& b) {
        int temp = a;
        a = b;
        b = temp;
    }

    void reverse(int* p, int start, int end) {
        while (start < end) {
            swap(p[start], p[end]);
            start++;
            end--;
        }
    }

    bool nextPermutation(int* p, int n) {
        int i;

        for (i = n - 2; i >= 0; i--) {
            if (p[i] < p[i + 1]) {
                break;
            }
        }

        if (i < 0) {
            return false;
        }

        int j;

        for (j = n - 1; j > i; j--) {
            if (p[i] < p[j]) {
                break;
            }
        }

        swap(p[i], p[j]);
        reverse(p, i + 1, n - 1);

        return true;
    }

    int calculateRouteCost(int** matrix, int startCity, const int* route, int routeSize) {
        int cost = 0;
        int currentCity = startCity;

        for (int i = 0; i < routeSize; i++) {
            cost += matrix[currentCity][route[i]];
            currentCity = route[i];
        }

        cost += matrix[currentCity][startCity];

        return cost;
    }

}

ExactResult solveBruteForce(int** matrix, int nCities, int startCity) {
    int routeSize = nCities - 1;

    int* route = new int[routeSize];
    int* bestRoute = new int[routeSize];
    int* worstRoute = new int[routeSize];

    int idx = 0;

    for (int i = 0; i < nCities; i++) {
        if (i != startCity) {
            route[idx] = i;
            bestRoute[idx] = i;
            worstRoute[idx] = i;
            idx++;
        }
    }

    const auto startTime = std::chrono::high_resolution_clock::now();

    int firstCost = calculateRouteCost(matrix, startCity, route, routeSize);

    int minCost = firstCost;
    int maxCost = firstCost;

    while (nextPermutation(route, routeSize)) {
        int currentCost = calculateRouteCost(matrix, startCity, route, routeSize);

        if (currentCost < minCost) {
            minCost = currentCost;

            for (int i = 0; i < routeSize; i++) {
                bestRoute[i] = route[i];
            }
        }

        if (currentCost > maxCost) {
            maxCost = currentCost;

            for (int i = 0; i < routeSize; i++) {
                worstRoute[i] = route[i];
            }
        }
    }

    const auto endTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsedTime = endTime - startTime;

    std::cout << "Best Path: " << startCity;

    for (int i = 0; i < routeSize; i++) {
        std::cout << "-" << bestRoute[i];
    }

    std::cout << "-" << startCity << " | ";

    std::cout << "Worst Path: " << startCity;

    for (int i = 0; i < routeSize; i++) {
        std::cout << "-" << worstRoute[i];
    }

    std::cout << "-" << startCity << " | ";

    delete[] route;
    delete[] bestRoute;
    delete[] worstRoute;

    return { minCost, maxCost, elapsedTime.count() };
}

GreedyResult solveGreedy(int** matrix, int nCities, int startCity) {
    int totalCost = 0;
    int currentCity = startCity;

    int routeSize = nCities - 1;
    int* route = new int[routeSize];

    int idx = 0;

    for (int i = 0; i < nCities; i++) {
        if (i != startCity) {
            route[idx++] = i;
        }
    }

    const auto startTime = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < routeSize; i++) {
        int bestIndex = i;
        int minCost = matrix[currentCity][route[i]];

        for (int j = i + 1; j < routeSize; j++) {
            int cost = matrix[currentCity][route[j]];

            if (cost < minCost) {
                minCost = cost;
                bestIndex = j;
            }
        }

        swap(route[i], route[bestIndex]);

        totalCost += minCost;
        currentCity = route[i];
    }

    totalCost += matrix[currentCity][startCity];

    const auto endTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsedTime = endTime - startTime;

    std::cout << "Greedy Path: " << startCity;

    for (int i = 0; i < routeSize; i++) {
        std::cout << "-" << route[i];
    }

    std::cout << "-" << startCity << " | ";

    delete[] route;

    return { totalCost, elapsedTime.count() };
}

double calculateQuality(int minCost, int maxCost, int greedyCost) {
    if (maxCost == minCost) {
        return 100.0;
    }

    return (static_cast<double>(maxCost - greedyCost) / (maxCost - minCost)) * 100.0;
}