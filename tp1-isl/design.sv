`timescale 1ns / 1ps	// Define a resolução de tempo para a simulação
						// 1ns -> 1 nanosegundo, e 1ps é a precisão -> 1 picosegundo

module Resto5 (
  	input [3:0] sensor, // Entrada de 4 bits que representa a leitura dos sensores
    output R1, 			// Saída R1 (primeiro bit do resto da divisão por 5)
  	output R2, 			// Saída R2 (segundo bit do resto da divisão por 5)
  	output R3 			// Saída R3 (terceiro bit do resto da divisão por 5)
);
  	
  	// "Descompacta" os bits de entrada para facilitar a manipulação do código
  	wire A = sensor[3];
    wire B = sensor[2];
    wire C = sensor[1];
  	wire D = sensor[0];
	
  	// Expressões lógicas para calcular os valores de cada saída
    assign R1 = (~A & B & ~C & ~D) | (A & B & C & ~D) | (A & ~B & ~C & D);
    assign R2 = (~A & C & D) | (A & ~C & ~D) | (A & B & ~C) | (~A & ~B & C);
    assign R3 = (~B & C & D) | (~A & ~B & D) | (~A & B & C & ~D) | (A & B & ~C & D) | (A & ~B & ~C & ~D);

endmodule
