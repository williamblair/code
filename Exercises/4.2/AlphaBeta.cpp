#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include "AlphaBeta.h"

float AlphaBetaMin(const BoardState* node, int depth, float alpha, float beta);

float AlphaBetaMax(const BoardState* node, int depth, float alpha, float beta)
{
	// If this is a leaf, return score
	if (depth == 0 || node->IsTerminal())
	{
		//return GetScore(node->mState);
        return node->GetScore();
	}

    // Assumes AI Player (Max) is always red
    std::vector<BoardState*> possibleMoves = node->GetPossibleMoves(BoardState::Red);

	float maxValue = -std::numeric_limits<float>::infinity();
	// Find the subtree with the maximum value
	for (const BoardState* child : possibleMoves)
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, depth - 1, alpha, beta));
		if (maxValue >= beta)
		{
			return maxValue; // Beta prune
		}
		alpha = std::max(maxValue, alpha);
	}

    for (BoardState* p : possibleMoves)
    {
        delete p;
    }

	return maxValue;
}

float AlphaBetaMin(const BoardState* node, int depth, float alpha, float beta)
{
	// If this is a leaf, return score
	if (depth == 0 || node->IsTerminal())
	{
        return node->GetScore();
	}

    // assumes human player (Min) is always yellow
    std::vector<BoardState*> possibleMoves = node->GetPossibleMoves(BoardState::Yellow);

	float minValue = std::numeric_limits<float>::infinity();
	// Find the subtree with the minimum value
	for (const BoardState* child : possibleMoves)
	{
		minValue = std::min(minValue, AlphaBetaMax(child, depth - 1, alpha, beta));
		if (minValue <= alpha)
		{
			return minValue; // Alpha prune
		}
		beta = std::min(minValue, beta);
	}

    for (BoardState* p : possibleMoves)
    {
        delete p;
    }

	return minValue;
}

const BoardState* AlphaBetaDecide(const BoardState* root, int maxDepth)
{
	// Find the subtree with the maximum value, and save the choice
	const BoardState* choice = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
    std::vector<BoardState*> possibleMoves = root->GetPossibleMoves(BoardState::Red);
	for (const BoardState* child : possibleMoves)
	{
		float v = AlphaBetaMin(child, maxDepth - 1, maxValue, beta);
		if (v > maxValue)
		{
			maxValue = v;
			choice = child;
		}
	}
    for (BoardState* p : possibleMoves)
    {
        if (p != choice)
        {
            delete p;
        }
    }
	return choice;
}

