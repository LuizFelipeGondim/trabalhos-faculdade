#include "Solver.hpp"

// Retorna o menor valor entre dois inteiros.
int Solver::min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * Realiza uma busca em largura (BFS) no tabuleiro para calcular o número mínimo de movimentos que a 
 * rainha precisa fazer para ir de uma posição inicial (start) até uma posição alvo (target), podendo 
 * ela ir em qualquer direção e evitando obstáculos.
 */
int Solver::bfs(const std::vector<std::string>& board, Position start, Position target) {
    int numRows = static_cast<int>(board.size());
    int numCols = static_cast<int>(board[0].size());

    std::vector<std::vector<int>> distance(numRows, std::vector<int>(numCols, INT_MAX));
    std::queue<Position> positionsQueue;
    
    int deltaRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int deltaCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    distance[start.row][start.col] = 0;
    positionsQueue.push(start);

    while (!positionsQueue.empty()) {
        auto [currentRow, currentCol] = positionsQueue.front();
        positionsQueue.pop();
        
        for (int direction = 0; direction < 8; direction++) {
            int nextRow = currentRow;
            int nextCol = currentCol;

            while (true) {
                nextRow += deltaRow[direction];
                nextCol += deltaCol[direction];

                if (nextRow < 0 || nextRow >= numRows || nextCol < 0 || nextCol >= numCols || board[nextRow][nextCol] == '-') break;

                if (distance[nextRow][nextCol] > distance[currentRow][currentCol] + 1) {
                    distance[nextRow][nextCol] = distance[currentRow][currentCol] + 1;
                    positionsQueue.push({nextRow, nextCol});
                }
            }
        }
    }

    return distance[target.row][target.col] == INT_MAX ? -1 : distance[target.row][target.col];
}

/**
 * Calcula a matriz de distâncias entre todos os pontos de interesse. Esses pontos incluem a posição inicial 
 * da rainha e as posições de todos os peõesn sendo que cada célula dist[i][j] armazena o número mínimo de 
 * movimentos da rainha para ir do ponto i até o ponto j.
 */
std::vector<std::vector<int>> Solver::computeDistanceMatrix(const std::vector<std::string>& board, const std::vector<Position>& points) {
    int numPoints = static_cast<int>(points.size());
    std::vector<std::vector<int>> distanceMatrix(numPoints, std::vector<int>(numPoints, -1));

    for (int fromPoint = 0; fromPoint < numPoints; fromPoint++) {
        for (int toPoint = 0; toPoint < numPoints; toPoint++) {
            
            if (fromPoint != toPoint) {
                distanceMatrix[fromPoint][toPoint] = bfs(board, points[fromPoint], points[toPoint]);
            } else {
                distanceMatrix[fromPoint][toPoint] = 0;
            }
        }
    }

    return distanceMatrix;
}

/**
 * Resolve o problema de forma exata usando o algoritmo Held-Karp para TSP. Primeiro, calcula a matriz de distâncias entre a rainha e 
 * todos os peões, em seguida, aplica programação dinâmica (DP com máscara de bits) para encontrar o menor número de movimentos necessários 
 * para capturar todos os peões, considerando todas as ordens possíveis.
 *
 * Utiliza o estado dp[mask][u], onde:
 * - mask: um subconjunto de peões já visitados (em formato de bits).
 * - u: o último peão visitado.
 */
int Solver::exactSolver(const std::vector<std::string>& board, const Position& queenPosition, const std::vector<Position>& pawnPositions) {
    int numPawns = static_cast<int>(pawnPositions.size());
    std::vector<Position> allPoints = {queenPosition};
    allPoints.insert(allPoints.end(), pawnPositions.begin(), pawnPositions.end());

    auto distanceMatrix = computeDistanceMatrix(board, allPoints);

    int totalStates = 1 << numPawns;
    std::vector<std::vector<int>> dp(totalStates, std::vector<int>(numPawns, INT_MAX));

    for (int firstPawn = 0; firstPawn < numPawns; firstPawn++) {
        if (distanceMatrix[0][firstPawn + 1] >= 0) {
            dp[1 << firstPawn][firstPawn] = distanceMatrix[0][firstPawn + 1];
        }
    }

    for (int visitedMask = 0; visitedMask < totalStates; visitedMask++) {
        for (int lastPawn = 0; lastPawn < numPawns; lastPawn++) {

            if (!(visitedMask & (1 << lastPawn)) || dp[visitedMask][lastPawn] == INT_MAX) continue;

            for (int nextPawn = 0; nextPawn < numPawns; nextPawn++) {

                if (visitedMask & (1 << nextPawn)) continue;
                
                if (distanceMatrix[lastPawn + 1][nextPawn + 1] >= 0) {
                    int newMask = visitedMask | (1 << nextPawn);
                    dp[newMask][nextPawn] = min(dp[newMask][nextPawn], dp[visitedMask][lastPawn] + distanceMatrix[lastPawn + 1][nextPawn + 1]);
                }
            }
        }
    }

    int bestAnswer = INT_MAX;
    for (int lastPawn = 0; lastPawn < numPawns; lastPawn++) {
        bestAnswer = min(bestAnswer, dp[totalStates - 1][lastPawn]);
    }

    return bestAnswer == INT_MAX ? -1 : bestAnswer;
}

/**
 * Resolve o problema de forma aproximada usando uma heurística gulosa (vizinho mais próximo). A cada iteração, a rainha 
 * sempre vai até o peão mais próximo (em número de movimentos), considerando o estado atual do tabuleiro. O processo 
 * continua até que todos os peões sejam capturados.
 */
int Solver::approximateSolver(const std::vector<std::string>& board, const Position& queenPosition, const std::vector<Position>& pawnPositions) {
    int totalMoves = 0;
    std::vector<bool> pawnVisited(pawnPositions.size(), false);
    Position currentPosition = queenPosition;
    int pawnPositionsSize = static_cast<int>(pawnPositions.size());

    for (int moveCount = 0; moveCount < pawnPositionsSize; moveCount++) {
        int shortestDistance = INT_MAX;
        int nearestPawnIndex = -1;

        for (int pawnIndex = 0; pawnIndex < pawnPositionsSize; pawnIndex++) {
            if (!pawnVisited[pawnIndex]) {
                int distanceToPawn = bfs(board, currentPosition, pawnPositions[pawnIndex]);

                if (distanceToPawn >= 0 && distanceToPawn < shortestDistance) {
                    shortestDistance = distanceToPawn;
                    nearestPawnIndex = pawnIndex;
                }
            }
        }

        if (nearestPawnIndex == -1) return -1;

        totalMoves += shortestDistance;
        currentPosition = pawnPositions[nearestPawnIndex];
        pawnVisited[nearestPawnIndex] = true;
    }

    return totalMoves;
}