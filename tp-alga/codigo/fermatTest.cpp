#include "fermatTest.hpp"
#include <iostream>
#include <random>

// Calcula o máximo divisor comum entre dois números usando o algoritmo de Euclides.
unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Realiza a exponenciação modular rápida para evitar overflow, calculando (base^exp) % mod.
unsigned long long powMod(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long res = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            res = (unsigned long long)(((unsigned __int128)res * base) % mod);
        }

        base = (unsigned long long)(((unsigned __int128)base * base) % mod);
        exp /= 2;
    }

    return res;
}

// Executa o teste probabilístico de Fermat testando 'k' bases aleatórias.
bool fermatTest(unsigned long long n, int k) {
    if (n <= 1) return false;
    if (n <= 3) return true;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dist(2, n - 2);

    for (int i = 0; i < k; ++i) {
        unsigned long long a = dist(gen);

        if (powMod(a, n - 1, n) != 1) {
            return false;
        }
    }

    return true;
}

// Verifica se a^(n-1) ≡ 1 (mod n) para todas as bases 'a' coprimas a 'n'.
bool verifyAllCoprimeBases(unsigned long long n) {
    for (unsigned long long a = 2; a < n; ++a) {

        if (gcd(a, n) == 1) {

            if (powMod(a, n - 1, n) != 1) {
                return false;
            }

        }
    }

    return true;
}

// Demonstra que os números de Carmichael falham no teste de Fermat (passando como falsos positivos).
void runCarmichaelExamples() {

    unsigned long long carmichael[] = {561, 1105, 1729};

    std::cout << "\n=== Números de Carmichael ===\n\n";

    for (unsigned long long n : carmichael) {

        std::cout << "n = " << n << "\n";

        if (n == 561)
            std::cout << "Fatoração: 3 * 11 * 17\n";
        else if (n == 1105)
            std::cout << "Fatoração: 5 * 13 * 17\n";
        else if (n == 1729)
            std::cout << "Fatoração: 7 * 13 * 19\n";

        if (verifyAllCoprimeBases(n)) {
            std::cout << "Passa no teste de Fermat para TODAS as bases coprimas.\n";
        } else {
            std::cout << "Falhou para alguma base coprima.\n";
        }

        std::cout << "Conclusão: " << n << " é COMPOSTO.\n";
    }
}