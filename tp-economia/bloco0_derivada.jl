function derivada(f,ponto,precisao)
  variacao=abs(sum(precisao))
  quociente_de_newton=(f(ponto+precisao...)-f(ponto...))/variacao
  return quociente_de_newton
end