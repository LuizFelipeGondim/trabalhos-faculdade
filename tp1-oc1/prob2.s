.data

##### R1 START MODIFIQUE AQUI START #####
#
# Este espaço é para você definir as suas constantes e vetores auxiliares.
#

lista_precos: .word 7, 5, 1  # Vetor com os preços iniciais dos produtos
fator_aumento: .word 3       # Fator de aumentos do preço
taxa_50: .word 2             # Taxa adicional para ajustar o preço

##### R1 END MODIFIQUE AQUI END #####

.text

add s0, zero, zero     # Inicializa s0 com 0; s0 contará o número de testes que passaram

li a0, 0               # Inicializa a0
la a1, lista_precos    # a1 aponta para o início da lista de preços
li a2, 0               # Índice inicial do vetor
li a3, 3               # Tamanho do vetor

jal ra, soma_estoque   # Chama função para somar todos os preços ajustados
addi t5, zero, 78      # Esperado: (7*3*2) + (5*3*2) + (1*3*2) = 42 + 30 + 6 = 78
bne a0, t5, teste_2    # Se o resultado for diferente, pula para teste_3
addi s0,s0,1           # Incrementa s0 (teste passou)

teste_2:

li a0, 3               # Inicializa a0
lw a1, fator_aumento   # a1 recebe o fator de aumento

jal ra, ajustar_preco  # Chama a função ajustar_preco(a0 = 3)
addi t5, zero, 18      # Valor esperado do resultado = 3 * 3 * 2 = 18
bne a0, t5, teste_3    # Se o resultado for diferente, pula para teste_2
addi s0,s0,1           # Incrementa s0 (teste passou)

teste_3:
li a0, 2               # a0 recebe 2, elemento i do vetor vetor[i]
lw a1, taxa_50   	   # a1 recebe a taxa de aumento

jal ra, aplicar_taxa   # Chama a função aplicar_taxa (a0 = 2, a3 = 2)
addi t5, zero, 4       # Esperado: 2 * 2 = 4
bne a0, t5, FIM        # Se diferente de 4, pula para FIM
addi s0,s0,1           # Incrementa s0 (teste passou)
j FIM
                  
##### R2 START MODIFIQUE AQUI START #####

soma_estoque:
  addi sp, sp, -12         # Reserva espaço na pilha
  sw ra, 12(sp)            # Salva o endereço de retorno
  sw s0, 8(sp)             # Salva s0
  sw s1, 4(sp)             # Salva s1
  li s0, 0                 # Inicializa s0 com 0
  mv s1, a1                # s1 aponta para o vetor de preços  

.loop:
  beq a2, a3, .end         # Se a2 == a3, sai do loop
  slli t0, a2, 2           # Multiplica a2 por 4 (tamanho de um word)
  add t1, s1, t0           # t1 = s1 + a2 * 4
  lw a0, 0(t1)             # Carrega o preço do produto
  la t2, fator_aumento     # t2 aponta para o fator de aumento
  lw a1, 0(t2)             # Carrega o fator de aumento
  jal ra, ajustar_preco    # Chama a função ajustar_preco
  add s0, s0, a0           # Soma o preço ajustado
  addi a2, a2, 1           # Incrementa o índice
  j .loop                  # Volta para o loop

.end:
  mv a0, s0                # Retorna o valor total
  lw s1, 4(sp)             # Carrega s1
  lw s0, 8(sp)             # Carrega s0
  lw ra, 12(sp)            # Carrega o endereço de retorno
  addi sp, sp, 12          # Libera espaço na pilha
  jalr x0, ra, 0           # Retorna

ajustar_preco:
  addi sp, sp, -4          # Reserva espaço na pilha
  sw ra, 4(sp)             # Salva o endereço de retorno
  mul a0, a0, a1           # Multiplica o preço pelo fator de aumento
  la t0, taxa_50           # t0 aponta para a taxa de aumento
  lw a1, 0(t0)             # Carrega a taxa de aumento
  jal ra, aplicar_taxa     # Chama a função aplicar_taxa
  lw ra, 4(sp)             # Carrega o endereço de retorno
  addi sp, sp, 4           # Libera espaço na pilha
  jalr x0, ra, 0           # Retorna

aplicar_taxa:
  addi sp, sp, -4          # Reserva espaço na pilha
  sw ra, 4(sp)             # Salva o endereço de retorno
  mul a0, a0, a1           # Multiplica o preço pela taxa de aumento
  lw ra, 4(sp)             # Carrega o endereço de retorno
  addi sp, sp, 4           # Libera espaço na pilha
  jalr x0, ra, 0           # Retorna

##### R2 END MODIFIQUE AQUI END #####

FIM: