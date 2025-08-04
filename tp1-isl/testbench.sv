`timescale 1ns / 1ps	// Define a resolução de tempo para a simulação
						// 1ns -> 1 nanosegundo, e 1ps é a precisão -> 1 picosegundo

// Módulo de teste
module TesteResto5;

  	reg [3:0] sensor; 	// Registrador de 4 bits para simular entrada 
    wire R1, R2, R3;	// "Fios" para as saídas do circuito
	
  	// Instância do módulo Resto5
    Resto5 uut (
      .sensor(sensor),	// Conecta o registrador à entrada 'sensor'
      .R1(R1),			// Conecta a saída 'R1' do módulo à variável 'R1'
      .R2(R2),			// Conecta a saída 'R2' do módulo à variável 'R2'
      .R3(R3)			// Conecta a saída 'R3' do módulo à variável 'R3'
    );
	
    initial begin
      
      	// Gera o arquivo de simulação VCD para visualização de sinais
      	$dumpfile("dump.vcd");
      	$dumpvars(0, uut); 

		// Monitoramento das entradas e saídas
      	$monitor("Entrada = %b | Saída = %b%b%b", sensor, R1, R2, R3);
		
      	// Entradas especificadas no enunciado
        sensor = 4'b0110;
        #10;
        sensor = 4'b1101;
        #10;
        sensor = 4'b1010;
        #10;
        sensor = 4'b0011;
        #10;
        sensor = 4'b1001;  
        #10;

        $finish;
    end

endmodule
