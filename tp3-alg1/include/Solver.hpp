#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <string>
#include <vector>
#include <queue>
#include <climits>

struct Position {
    int row, col;
};

class Solver {
    private:
        int min(int a, int b);
        int bfs(const std::vector<std::string>& board, Position start, Position target);
        std::vector<std::vector<int>> computeDistanceMatrix(const std::vector<std::string>& board, const std::vector<Position>& points);

    public:
        int exactSolver(const std::vector<std::string>& board, const Position& queen, const std::vector<Position>& pawns);
        int approximateSolver(const std::vector<std::string>& board, const Position& queen, const std::vector<Position>& pawns);
};

#endif