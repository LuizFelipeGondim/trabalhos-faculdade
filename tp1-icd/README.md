# Projeto_ICD-2025_Fut
Repositório para o projeto da disciplina de Introdução à Ciência dos Dados.

## Tema
Análise acerca do desempenho dos times e jogadores no Brasileirão Série A Masculino nos últimos 10 anos.

## Conjunto de dados
O conjunto de dados que escolhemos possui diversas estatísticas sobre diferentes âmbitos de partidas, times e jogadores da Série A do Campeonato Brasileiro Masculino. Nesse conjunto de dados, é possível encontrar informações sobre aspectos variados das partidas de futebol que aconteceram nesse campeonato. Escolhemos trabalhar com os dados dos anos de 2014 a 2024 para ter um volume maior de dados e para ter dados bem abrangentes sobre o futebol brasileiro.
Os dados presentes nesse conjunto são completos, nos permitindo responder perguntas complexas utilizando mais de uma característica para conseguirmos chegar às respostas que queremos. Os dados estão separados em diferentes tabelas e iremos retirar esses dados e agrupá-los por times e jogadores, mantendo essas tabelas. Esses dados que possuímos descrevem muito bem os jogos e contém diversas estatísticas interessantes para serem consideradas e exploradas durante o trabalho.
## Perguntas
### Existe uma disparidade de desempenho entre jogadores de nacionalidade brasileira e dos estrangeiros? Quais aspectos influenciam?
Para responder a essa pergunta, será necessário utilizar os dados individuais sobre os jogadores, suas nacionalidades e métricas de desempenho, como gols marcados, defesas feitas, bolas desarmadas, penalidades convertidas e outras.
### Há alguma relação entre goleiros com mais cleansheets (partidas sem sofrer gols) e times com mais pontos?
Essa pergunta demanda que utilizemos dados individuais dos goleiros de quantas partidas sem sofrer gols eles passaram em cada temporada e cruzar esses dados com a quantidade de pontos que os times desses goleiros fizeram naquela temporada. Para isso, serão necessários, além dos dados já especificados, juntar diferentes tabelas, uma que contenha os goleiros com a informação de qual time ele joga e a outra que contenha os times e a quantidade de pontos que eles tenham marcado no campeonato.
### Qual a relação da porcentagem de posse de bola com a vitória de um time? Há alguma relação do tipo de passe e posse de bola?
Tentaremos relacionar os dados das tabelas de posse, estatísticas gerais e tipo de passe. A primeira possui informações como: % de posse, número de passes recebidos, distância percorrida com a bola. Em tipo de passes temos: passes bem sucedidos, passes feitos, passes com mais de 20m, cruzamentos, passes interceptados, entre outros. A partir dos dados presentes nessas tabelas faremos nossas análises a fim de responder às perguntas propostas.
### Existe algum tipo de chute que torna o time mais efetivo em marcar gols?
Nesse conjunto de dados, existem dados respectivos aos chutes de cada time. Esses dados contemplam quais os times com mais e menos chutes, quais os times com mais e menos chutes ao gol, qual a distância média de chutes dos times, qual a quantidade de gols marcados por cada time, qual a média de chutes necessários para se marcar um gol de cada time, qual a expectativa de gols de um time específico em um jogo. Todos esses dados serão importantes para responder a essa pergunta de forma efetiva.
## Próximos passos
- Download da base de dados via web scraping pois a mesma não oferece um arquivo com os dados, CSV, por exemplo.
- Pré-processamento dos dados, com foco especial no tratamento de valores nulos. Essa etapa exigirá uma análise mais aprofundada, pois, em cada ano, a Série A apresenta variações nos times participantes (devido a rebaixamentos e promoções).
- Junção das tabelas para cada pergunta, acompanhada da remoção de atributos irrelevantes para a análise, a fim de tornar o processo de interpretação dos dados mais eficiente.
## Referências
- FBREF. Série A Stats | FBref.com. Disponível em: https://fbref.com/en/comps/24/2024/2024-Serie-A-Stats. Acesso em: 26 abr. 2025.

