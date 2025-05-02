.data

##### R1 START MODIFIQUE AQUI START #####
#
# Este espaço é para você definir as suas constantes e vetores auxiliares.
#

vetor1: .word 2, 3, 4                                  
vetor2: .word 1, 5, 4, 6, 8                                                               

##### R1 END MODIFIQUE AQUI END #####

.text
  add s0, zero, zero       # s0 armazenará o número de testes que passaram
  la a0, vetor1
  addi a1, zero, 3                      
  jal e_crescente            
  addi t0, zero, 1
  bne a0, t0, teste_2     
  addi s0, s0, 1

teste_2: 	    
  la a0, vetor2
  addi a1, zero, 5
  jal e_crescente            
  addi t0, zero, 0
  bne a0, t0, FIM     
  addi s0, s0, 1
  j FIM

##### R2 START MODIFIQUE AQUI START #####

e_crescente:
  addi t1, a1, -1         
  beqz t1, crescente     
  mv t2, a0              

loop:
  lw t3, 0(t2)            
  lw t4, 4(t2)            
  bgt t3, t4, n_crescente  
  addi t2, t2, 4          
  addi t1, t1, -1         
  bnez t1, loop          

crescente:
  li a0, 1                
  jr ra                   

n_crescente:
  li a0, 0                
  jr ra                   

##### R2 END MODIFIQUE AQUI END #####

FIM: