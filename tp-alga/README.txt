# Instalação e Execução

## Pré-requisitos

Antes de compilar e executar o programa, é necessário possuir:

- Um compilador compatível com C++11 ou superior (recomenda-se o GNU G++, ou g++);
- Um terminal para executar os comandos de compilação e execução.

---

## Compilação

No diretório raiz do projeto, execute o seguinte comando:

```bash
g++ -o exec main.cpp fermatTest.cpp rsa.cpp -O2
```

**Atenção:** o parâmetro "-O2" utiliza a letra **O maiúscula** (de "Optimization"), e não o número zero.

Esse comando gera um executável chamado `exec`.

---

## Execução

### Linux

./exec


### Windows

exec.exe ou .\exec.exe

---

## Observações

- Certifique-se de que os arquivos `main.cpp`, `fermatTest.cpp` e `rsa.cpp` estejam no mesmo diretório em que o comando de compilação for executado.
- Caso sejam realizadas alterações no código-fonte a fim de testes, é necessário recompilar o projeto antes de executá-lo novamente.
