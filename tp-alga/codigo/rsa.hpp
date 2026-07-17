#ifndef RSA_HPP
#define RSA_HPP

// Encontra os coeficientes x e y da identidade de Bézout, além de retornar o MDC.
unsigned long long extendedEuclidean(unsigned long long a, unsigned long long b, long long *x, long long *y);

// Calcula o inverso modular de 'a' módulo 'm' utilizando o algoritmo de Euclides estendido.
unsigned long long modularInverse(unsigned long long a, unsigned long long m);

// Realiza o teste de primalidade forte de Miller-Rabin, mais rigoroso que o de Fermat.
bool millerRabinTest(unsigned long long n, int k);

// Gera um número primo ímpar usando Fermat como filtro rápido e Miller-Rabin para validação rigorosa.
unsigned long long generatePrime(int bits);

// Orquestra a geração de chaves RSA, cifração, decifração e verificação de assinaturas digitais.
void runRSACryptosystem();

#endif