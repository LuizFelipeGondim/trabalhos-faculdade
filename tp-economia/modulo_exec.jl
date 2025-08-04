# Dados de entrada
n= 35
a=(1/4+n/2000);b=(1/4+n/2000);
k0=4;af=1;bt=100;
f(k,l)=af*(k-k0)^a*l^b
isoqk(l,y)=k0+(y/af)^(1/a)/l^(b/a)
precis_deriv=10^(-12);
precis_grid=0.01
w=4;r=16;
kmax=10;lmax=20;
gridk=k0:precis_grid:kmax
gridl=0:precis_grid:lmax
y_max=f(kmax,lmax)
gridy=precis_grid:precis_grid:y_max
dem(p)=bt-af*p

#Arquivos de Execução

module principal
include("bloco0_derivada.jl")
include("bloco1_lucro_est.jl")
include("bloco2_cust_cond.jl")  
include("bloco3_lucro_ent.jl")       
end

#Comandos Extraídos do Módulo Principal
lucro_est=principal.lucro_est
lucro_ent=principal.lucro_ent
dem_cond=principal.dem_cond
custo_min=principal.custo_min
derivada=principal.derivada

#Execução dos Comandos

cmed(y)=custo_min(y, isoqk, w, r, gridl) / y
esc_min_ef_aux=argmin([cmed(y) for y in gridy])
esc_min_ef=gridy[esc_min_ef_aux]
prec_min=prec_min=minimum([cmed(y) for y in gridy])
dem_max=dem(prec_min)
num_firm=dem_max / esc_min_ef
p=prec_min

(cap_otimo,trab_otimo,prod_est,l_est)=lucro_est(f,p,w,r,gridk,gridl)
(prod_ent,l_ent)=lucro_ent(isoqk,p,w,r,gridl,gridy)
(dem_cond_cap,dem_cond_trab)=dem_cond(prod_est,isoqk,w,r,gridl)
custo_min_otimo=custo_min(prod_est,isoqk,w,r,gridl)
custo_min_aux(y)=custo_min(y,isoqk,w,r,gridl)
custo_marg=derivada(custo_min_aux,prod_est,precis_deriv)

pmgk_otimo=derivada(f,[cap_otimo,trab_otimo],[precis_deriv,0])
pmgl_otimo=derivada(f,[cap_otimo,trab_otimo],[0,precis_deriv])



#Exibição dos Resultados
println("")

println("Capital Ótimo da Firma Estabelecida = ",round(cap_otimo; digits = 3))
println("Trabalho Ótimo da Firma Estabelecida = ",round(trab_otimo; digits = 3))
println("Produção Ótima da Firma Estabelecida = ",round(prod_est; digits = 3))
println("Salário Real = ",round(w/p; digits = 3))
println("Produto Marginal do Trabalho = ",round(pmgl_otimo; digits = 3))
println("Aluguel Real do Capital = ",round(r/p; digits = 3))
println("Produto Marginal do Capital = ",round(pmgk_otimo; digits = 3))
println("Custo Marginal Ótimo = ",round(custo_marg; digits = 3))
println("Preço do Produto = ",round(p; digits = 3))

println("")

println("Lucro Máximo da Firma Estabelecida = ",round(l_est; digits = 3))
println("Lucro Máximo da Firma Entrante = ",round(l_ent; digits = 3))
println("Escala Mínima de Eficiência = ",round(esc_min_ef; digits = 3))
println("Preço mínimo de mercado = ",round(prec_min; digits = 3))
println("Demanda máxima de mercado = ",round(dem_max; digits = 3))
println("Número de firmas do mercado = ",round(num_firm; digits = 3))

println("")

println("Demanda condicional por Capital = ",round(dem_cond_cap; digits = 3))
println("Demanda condicional por Trabalho  = ",round(dem_cond_trab; digits = 3))

println("##############################################")
println("")

#Perguntas

#Calcule e explique a intuição econômica sobre o que acontece com o Preço mínimo, a Escala Mínima de Eficiência e o Número de firmas do mercado quando se aumenta na margem:

# Ajustando a elasticidade do produto do trabalho (valores dados no início desse arquivo)
a_original = a
b_original = b

# Função para calcular o preço mínimo, escala mínima de eficiência e número de firmas
function calc_metrics(a, b, w, r, k0)
    f(k, l) = af * (k - k0)^a * l^b
    isoqk(l, y) = k0 + (y / af)^(1 / a) / l^(b / a)
    cmed(y) = custo_min(y, isoqk, w, r, gridl) / y
    esc_min_ef_aux = argmin([cmed(y) for y in gridy])
    esc_min_ef = gridy[esc_min_ef_aux]
    prec_min = minimum([cmed(y) for y in gridy])
    dem_max = dem(prec_min)
    num_firm = dem_max / esc_min_ef
    return (prec_min, esc_min_ef, num_firm)
end

#1 - elasticidade produto do trabalho: a

# Calculando para a = a_original + 0.01
a_new = a_original + 0.01
prec_min_a, esc_min_ef_a, num_firm_a = calc_metrics(a_new, b_original, w, r, k0)
println("Para a = ", a_new)
println("Preço mínimo: ", round(prec_min_a, digits=3))
println("Escala mínima de eficiência: ", round(esc_min_ef_a, digits=3))
println("Número de firmas: ", round(num_firm_a, digits=3))

# Intuição econômica

# Um aumento na elasticidade do produto do trabalho (a) geralmente significa que a produção é mais responsiva a mudanças no trabalho.
# Isso pode levar a uma menor necessidade de capital, reduzindo os custos e, portanto, o preço mínimo. A escala mínima de eficiência 
# pode diminuir, aumentando a possibilidade de entrada de novas firmas, resultando em um maior número de firmas no mercado.

# Se o aumento na elasticidade do trabalho resulta em custos de entrada mais altos ou maior competição, o número de firmas entrantes pode diminuir.

println("##############################################")
println("")

#2 - elasticidade produto do capital: b

# Calculando para b = b_original + 0.01
b_new = b_original + 0.01
prec_min_b, esc_min_ef_b, num_firm_b = calc_metrics(a_original, b_new, w, r, k0)
println("Para b = ", b_new)
println("Preço mínimo: ", round(prec_min_b, digits=3))
println("Escala mínima de eficiência: ", round(esc_min_ef_b, digits=3))
println("Número de firmas: ", round(num_firm_b, digits=3))

# Intuição econômica
# Aumentar a elasticidade do capital (b) torna o capital mais produtivo, reduzindo o custo marginal e o preço mínimo. Isso permite uma escala 
# mínima de eficiência menor, o que pode aumentar o número de firmas no mercado, pois mais empresas se tornam lucrativas com a redução dos custos. 

# Ainda pode levar a uma maior escala mínima de eficiência se a produção eficiente exigir mais capital e trabalho para aproveitar os ganhos 
# de produtividade. Consequentemente, o preço mínimo pode diminuir devido à redução dos custos de produção, mas a escala mínima de eficiência 
# e o número de firmas podem diminuir porque apenas firmas maiores conseguem operar de forma eficiente na nova escala. Firmas menores podem 
# ser excluídas do mercado, resultando em uma menor competição e um número reduzido de firmas.

println("##############################################")
println("")

#3 - o salário: w

# Calculando para w = w + 1
w_new = w + 1
prec_min_w, esc_min_ef_w, num_firm_w = calc_metrics(a_original, b_original, w_new, r, k0)
println("Para w = ", w_new)
println("Preço mínimo: ", round(prec_min_w, digits=3))
println("Escala mínima de eficiência: ", round(esc_min_ef_w, digits=3))
println("Número de firmas: ", round(num_firm_w, digits=3))

# Intuição econômica
# Um aumento no salário (w) aumenta os custos de produção, elevando o preço mínimo.
# A escala mínima de eficiência pode aumentar, reduzindo a entrada de novas firmas e diminuindo o número de firmas no mercado.

# Pode ocorrer de a escala mínima de eficiência diminuir enquanto o custo médio de produção não, com isso o número de firmas 
# pode cair devido a custos gerais mais altos e uma demanda insuficiente.

println("##############################################")
println("")

#4 - o aluguel do capital: r

# Calculando para r = r + 1
r_new = r + 1
prec_min_r, esc_min_ef_r, num_firm_r = calc_metrics(a_original, b_original, w, r_new, k0)
println("Para r = ", r_new)
println("Preço mínimo: ", round(prec_min_r, digits=3))
println("Escala mínima de eficiência: ", round(esc_min_ef_r, digits=3))
println("Número de firmas: ", round(num_firm_r, digits=3))

# Intuição econômica
# Um aumento no aluguel do capital (r) aumenta os custos de produção, elevando o preço mínimo.
# A escala mínima de eficiência pode aumentar, reduzindo a entrada de novas firmas e diminuindo o número de firmas no mercado.

println("##############################################")
println("")

#5 - o custo de oportunidade do capital: k0

# Calculando para k0 = k0 + 1
k0_new = k0 + 1
prec_min_k0, esc_min_ef_k0, num_firm_k0 = calc_metrics(a_original, b_original, w, r, k0_new)
println("Para k0 = ", k0_new)
println("Preço mínimo: ", round(prec_min_k0, digits=3))
println("Escala mínima de eficiência: ", round(esc_min_ef_k0, digits=3))
println("Número de firmas: ", round(num_firm_k0, digits=3))

# Intuição econômica
# Um aumento no custo de oportunidade do capital (k0) diminui a quantidade de capital disponível inicialmente.
# Isso pode aumentar os custos de produção, elevando o preço mínimo e a escala mínima de eficiência.
# Com custos mais altos, menos firmas podem lucrar, diminuindo o número de firmas no mercado.