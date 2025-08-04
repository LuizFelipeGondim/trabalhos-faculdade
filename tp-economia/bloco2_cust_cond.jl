function dem_cond(y,isoqk,w,r,gridl)
  custo(l)=w * l + r * isoqk(l, y)
  c_aux=[custo(l) for l in gridl]
  l_otimo=gridl[argmin(c_aux)]
  k_otimo=isoqk(l_otimo,y)
  return [k_otimo,l_otimo]
end
function custo_min(y,isoqk,w,r,gridl)
  custo(l)=w * l + r * isoqk(l, y)
  c_aux=[custo(l) for l in gridl]
  c_min=minimum(c_aux)
  return c_min
end

println("Custo Condicional - Executado com Sucesso");