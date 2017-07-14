# Apresentação Projeto Final Sistemas Reativos

Alunos: Rafael Najjar (@kyros200) e Lucas d'Amaral (@LucasAmaralPIres)

## Objetivo
O objetivo do projeto é de fornecer controle para a área do conjunto de salas na 6023-F, na UERJ (Universidade do Estado do Rio de Janeiro). Tem como objetivo facilitar o acesso de todas as **Pessoas** as salas 6023-F. Enquanto todos que estão presentes no Banco de Dados tem acesso a sala, somente o Professor consegue adicionar ou remover Pessoas do Banco de Dados, dando a essas permissão de entrada na sala ou removendo-as da lista de acesso.

## Perfis
Existem 2 perfis de **Pessoa**: Aluno e Professor. O professor é aquele que tem acesso irrestrito na área caso ele dê alguma aula, somente ele também pode adicionar ou remover alunos. O aluno por sua vez só tem acesso se for adicionado pelo professor.

## Componentes
Para isso precisaremos de:
* **Arduino**
* **Shield leitor cartao RFID** para reconhecer o cartão da Pessoa
* **Shield Teclado Matricial de Membrana 12 Teclas** para a Pessoa ter a opção de colocar a Matrícula invés de passar o cartão
