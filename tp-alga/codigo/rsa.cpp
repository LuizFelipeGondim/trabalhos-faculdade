#include "rsa.hpp"
#include "fermatTest.hpp"
#include <iostream>
#include <random>

// Encontra os coeficientes x e y da identidade de Bézout, além de retornar o MDC.
unsigned long long extendedEuclidean(unsigned long long a, unsigned long long b, long long *x, long long *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    long long x1, y1;
    unsigned long long gcdResult = extendedEuclidean(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcdResult;
}

// Calcula o inverso modular de 'a' módulo 'm' utilizando o algoritmo de Euclides estendido.
unsigned long long modularInverse(unsigned long long a, unsigned long long m) {
    long long x, y;
    unsigned long long gcdResult = extendedEuclidean(a, m, &x, &y);
    if (gcdResult != 1) return 0;
    return (unsigned long long)((x % (long long)m + m) % m);
}

// Realiza o teste de primalidade de Miller-Rabin.
bool millerRabinTest(unsigned long long n, int k) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    unsigned long long d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d /= 2;
        s++;
    }

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<unsigned long long> dist(2, n - 2);

    for (int i = 0; i < k; ++i) {
        unsigned long long a = dist(gen);
        unsigned long long x = powMod(a, d, n);

        if (x == 1 || x == n - 1) continue;

        bool passedR = false;
        for (int r = 1; r < s; ++r) {
            x = (unsigned long long)(((unsigned __int128)x * x) % n);
            if (x == n - 1) {
                passedR = true;
                break;
            }
        }
        if (!passedR) return false;
    }
    return true;
}

// Gera um número primo ímpar usando Fermat como filtro rápido e Miller-Rabin para validação.
unsigned long long generatePrime(int bits) {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    
    unsigned long long minVal = 1ULL << (bits - 1);
    unsigned long long maxVal = (1ULL << bits) - 1;
    std::uniform_int_distribution<unsigned long long> dist(minVal, maxVal);

    while (true) {
        unsigned long long candidate = dist(gen);
        if (candidate % 2 == 0) continue; 

        if (!fermatTest(candidate, 5)) {
            continue;
        }

        if (millerRabinTest(candidate, 10)) {
            return candidate;
        }
    }
}

// Orquestra a geração de chaves RSA, cifração, decifração e verificação de assinaturas digitais.
void runRSACryptosystem() {
    std::cout << "--- Gerando Chaves RSA ---\n";

    unsigned long long p = generatePrime(32);
    unsigned long long q = generatePrime(32);

    while (p == q) {
        q = generatePrime(32);
    }

    unsigned long long n = p * q;
    unsigned long long phi_n = (p - 1) * (q - 1);
    unsigned long long e = 65537;

    // Garantir que e seja coprimo com phi(n)
    while (gcd(e, phi_n) != 1) {
        e += 2;
    }

    unsigned long long d = modularInverse(e, phi_n);

    std::cout << "Primos Gerados: p = " << p << ", q = " << q << "\n";
    std::cout << "Parâmetros: n = " << n << ", e = " << e << ", d = " << d << "\n\n";

    int option;
    std::cout << "Escolha o modo de teste:\n";
    std::cout << "[1] Automático\n";
    std::cout << "[2] Manual\n";
    std::cout << "Opção: ";
    std::cin >> option;

    if (option == 1) {
        int numTests;
        std::cout << "Quantas mensagens aleatórias deseja testar? (Mínimo 20): ";
        std::cin >> numTests;
        if (numTests < 20) {
            numTests = 20;
            std::cout << "O número de testes foi ajustado para 20.\n";
        } 

        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<unsigned long long> dist(1, n - 1);

        for (int i = 0; i < numTests; i++) {
            unsigned long long m = dist(gen);
            unsigned long long c = powMod(m, e, n);
            unsigned long long m_dec = powMod(c, d, n);

            std::cout << "\nTeste " << (i + 1) << "\n";
            std::cout << "Mensagem original: " << m << "\n";
            std::cout << "Cifrada: " << c << "\n";
            std::cout << "Decifrada: " << m_dec << "\n";
            std::cout << "Recuperação: " << (m_dec == m ? "Sucesso" : "Falha (m mod n)") << "\n";
        }
    } 
    else if (option == 2) {
        char continueTesting = 's';
        while (continueTesting == 's' || continueTesting == 'S') {
            unsigned long long m;
            std::cout << "\nDigite a mensagem (menor que " << n << "): ";
            std::cin >> m;

            if (m >= n) {
                std::cout << "Mensagem maior ou igual a n, ocorrerá perda de dados.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }

            unsigned long long c = powMod(m, e, n);
            unsigned long long m_dec = powMod(c, d, n);

            std::cout << "Cifrada: " << c << "\n";
            std::cout << "Decifrada: " << m_dec << "\n";
            std::cout << "Recuperação: " << (m_dec == m ? "Sucesso" : "Falha (m mod n)") << "\n";

            std::cout << "\nTestar outra mensagem? (s/n): ";
            std::cin >> continueTesting;
        }
    } else {
        std::cout << "Opção inválida.\n";
    }
}