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
  addi sp, sp, -12
  sw ra, 12(sp)
  sw s0, 8(sp)
  sw s1, 4(sp)
  li s0, 0           
  mv s1, a1          

.loop:
  beq a2, a3, .end   
  slli t0, a2, 2     
  add t1, s1, t0     
  lw a0, 0(t1)       
  la t2, fator_aumento
  lw a1, 0(t2)       
  jal ra, ajustar_preco  
  add s0, s0, a0     
  addi a2, a2, 1     
  j .loop

.end:
  mv a0, s0          
  lw s1, 4(sp)
  lw s0, 8(sp)
  lw ra, 12(sp)
  addi sp, sp, 12
  jalr x0, ra, 0

ajustar_preco:
  addi sp, sp, -4
  sw ra, 4(sp)
  mul a0, a0, a1     
  la t0, taxa_50
  lw a1, 0(t0)       
  jal ra, aplicar_taxa  
  lw ra, 4(sp)
  addi sp, sp, 4
  jalr x0, ra, 0

aplicar_taxa:
  addi sp, sp, -4
  sw ra, 4(sp)
  mul a0, a0, a1
  lw ra, 4(sp)
  addi sp, sp, 4
  jalr x0, ra, 0

##### R2 END MODIFIQUE AQUI END #####

FIM: