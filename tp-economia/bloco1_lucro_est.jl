
function lucro_est(f,p,w,r,gridk,gridl)
  luc(k,l)=p * f(k, l) - w * l - r * k
  
  l_aux=[luc(x...) for x in Iterators.product(gridk,gridl)]
  l_max=maximum(l_aux)
  kl_otimo=argmax(l_aux)
  k_otimo=gridk[kl_otimo[1]]
  l_otimo=gridl[kl_otimo[2]]
  y_otimo=f(k_otimo,l_otimo)
  return [k_otimo,l_otimo,y_otimo,l_max]
end

println("Lucro da firma estabelecida - Executado com Sucesso");