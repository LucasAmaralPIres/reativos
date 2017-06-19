# Apresentação Projeto Final Sistemas Reativos

Alunos: Rafael Najjar (@kyros200) e Lucas d'Amaral (@LucasAmaralPIres)

## Objetivo
O objetivo do projeto é de fornecer controle para a área do conjunto de salas na 6023F, na UERJ (Universidade do Estado do Rio de Janeiro). Tem como objetivo ter acesso mais fácil e acessível a todas as **Pessoas**, enquanto ao mesmo tempo aplica alguns métodos de entrada de acordo com o perfil da **Pessoa** (por exemplo, um aluno só poderá entrar na área caso esteja na hora da aula dele e quando o professor dessa aula estiver dentro).

## Perfis
Existem 3 perfis de **Pessoa**: Aluno, Professor e Admin. Admin tem como funcionalidade ser o responsável da definição dos professores nas matérias. O professor é aquele que tem acesso irrestrito na área caso ele dê alguma aula. O aluno por sua vez só tem acesso caso esteja na hora da aula dele e caso o professor dessa aula esteja na área.

## Componentes
Para isso precisaremos de:
* **Arduino**
* **Shield leitor de microSD** para que tenha bancos de dados.
* **Shield de Clock** para ter as horas atuais.
* **Shield leitor cartao RFID** para reconhecer o cartão da Pessoa
* **Shield Teclado Matricial de Membrana 12 Teclas** para a Pessoa ter a opção de colocar a Matrícula invés de passar o cartão
