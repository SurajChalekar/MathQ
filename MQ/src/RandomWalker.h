#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <random>

const float MIN_COORD = -0.75f;
const float MAX_COORD = 0.75f;

std::mt19937 rng(std::random_device{}());

std::vector<std::pair<float, float>> valid_moves(float x, float y) {
	std::vector<std::pair<float, float>> moves;

	if (x > MIN_COORD) moves.emplace_back(x - 0.25, y); // Left
	if (x < MAX_COORD) moves.emplace_back(x + 0.25, y); // Right
	if (y > MIN_COORD) moves.emplace_back(x, y - 0.25); // Down
	if (y < MAX_COORD) moves.emplace_back(x, y + 0.25); // Up

	return moves;
}

std::pair<float, float> random_walk_step(float x, float y) {
	std::vector<std::pair<float, float>> moves = valid_moves(x, y);
	std::uniform_int_distribution<int> dist(0, static_cast<int>(moves.size()) - 1);
	return moves[dist(rng)];
}

