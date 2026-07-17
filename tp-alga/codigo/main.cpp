#include <iostream>
#include <limits>
#include "fermatTest.hpp"
#include "rsa.hpp"

// Exibe o menu principal de opções para o usuário no terminal.
void displayMenu() {
    std::cout << "-------------------------------------------------\n";
    std::cout << "Selecione a atividade que deseja executar:\n\n";
    std::cout << "[1] Testar Tópico 5: Teste de Fermat e Números de Carmichael\n";
    std::cout << "[2] Testar Tópico 9: Implementação Completa do RSA\n";
    std::cout << "[0] Sair\n";
    std::cout << "Escolha uma opção: ";
}

// Ponto de entrada do programa que gerencia o loop principal e a seleção de atividades.
int main() {
    int choice = -1;

    while (choice != 0) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cout << "\n";

        switch (choice) {
            case 1: {
                long long n;
                int k;
                std::cout << "Digite o número n para testar a primalidade: ";
                std::cin >> n;
                std::cout << "Digite o número de bases k: ";
                std::cin >> k;

                if (fermatTest(n, k)) {
                    std::cout << "\nResultado: " << n << " é provavelmente primo.\n\n";
                } else {
                    std::cout << "\nResultado: " << n << " é composto.\n\n";
                }

                std::cout << "Executando exemplos obrigatórios de Carmichael\n";
                runCarmichaelExamples();
                break;
            }
            case 2:
                runRSACryptosystem();
                break;
            case 0:
                std::cout << "Encerrando o programa.\n";
                break;
            default:
                std::cout << "Opção inválida! Tente novamente.\n";
        }
        std::cout << "\n";
    }

    return 0;
}