function lucro_ent(isoqk,p,w,r,gridl,gridy)
  luc(y)= begin
   
    custo(l) = w * l + r * isoqk(l, y)
    c_aux = [custo(l) for l in gridl]
    c_min = minimum(c_aux)
    
    p * y - c_min
  end

  l_aux=[luc(y) for y in gridy]
  l_max=maximum(l_aux)
  y_otimo=gridy[argmax(l_aux)]
  return [y_otimo,l_max]
end

println("Lucro em Dois Estágios- Executado com Sucesso");