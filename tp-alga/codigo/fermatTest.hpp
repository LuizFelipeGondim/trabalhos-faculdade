#ifndef FERMAT_TEST_HPP
#define FERMAT_TEST_HPP

// Calculo do máximo divisor comum entre dois números.
unsigned long long gcd(unsigned long long a, unsigned long long b);

// Realiza a exponenciação modular rápida para evitar overflow, calculando (base^exp) % mod.
unsigned long long powMod(unsigned long long base, unsigned long long exp, unsigned long long mod);

// Executa o teste probabilístico de Fermat testando 'k' bases aleatórias.
bool fermatTest(unsigned long long n, int k);

// Verifica se a^(n-1) ≡ 1 (mod n) para todas as bases 'a' coprimas a 'n'.
bool verifyAllCoprimeBases(unsigned long long n);

// Mostra números de Carmichael que falham no teste de Fermat (passando como falsos positivos).
void runCarmichaelExamples();

#endif