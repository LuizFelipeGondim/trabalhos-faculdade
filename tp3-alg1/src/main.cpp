#include <iostream>
#include <chrono>
#include "Solver.hpp"

/**
 * Função principal do programa que lê o tabuleiro, a posição da rainha e dos peões. Em seguida, 
 * resolve o problema usando os algoritmos exato e aproximado, e imprime o número mínimo de movimentos
 * necessários junto com o tempo de execução de cada abordagem.
 */
int main() {
    Solver solver;

    int N, M, K;
    std::cin >> N >> M >> K;
    
    std::vector<std::string> board(N);
    Position queen;
    std::vector<Position> pawns;

    for (int i = 0; i < N; i++) {
        std::cin >> board[i];
        for (int j = 0; j < M; j++) {
            if (board[i][j] == 'R') queen = {i, j};
            if (board[i][j] == 'P') pawns.push_back({i, j});
        }
    }

 
    //auto startExact = std::chrono::high_resolution_clock::now();
    int exact = solver.exactSolver(board, queen, pawns);
    //auto endExact = std::chrono::high_resolution_clock::now();
    //auto durationExact = std::chrono::duration_cast<std::chrono::milliseconds>(endExact - startExact);

    //auto startApprox = std::chrono::high_resolution_clock::now();
    //int approx = solver.approximateSolver(board, queen, pawns);
    //auto endApprox = std::chrono::high_resolution_clock::now();
    //auto durationApprox = std::chrono::duration_cast<std::chrono::milliseconds>(endApprox - startApprox);

    std::cout << exact;

    //std::cout << exact << " (" << durationExact.count() << " ms)" << std::endl;
    //std::cout << approx << " (" << durationApprox.count() << " ms)" << std::endl;

    return 0;
}