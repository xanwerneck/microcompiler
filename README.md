INF1018 - Software B�sico - 2013.2

Segundo Trabalho

Gerador de C�digo

O objetivo deste trabalho � desenvolver, em C, uma fun��o gera que implementa um pequeno gerador de c�digo (um "micro-compilador") para uma linguagem de programa��o bastante simples, chamada SB. A fun��o gera dever� ler um arquivo texto contendo o c�digo fonte de uma ou mais fun��es escritas em SB e retornar dois ponteiros:

um ponteiro para o in�cio da regi�o de mem�ria que cont�m o c�digo que corresponde � tradu��o das fun��es SB para c�digo de m�quina
um ponteiro para o in�cio do c�digo da �ltima fun��o lida (essa � a fun��o do c�digo gerado que ser� "chamada" externamente)
Al�m disso, dever� ser implementada uma fun��o libera, que libera a mem�ria alocada para o c�digo criado por gera.
Os trabalhos podem ser feitos em grupos de at� dois alunos. Leia com aten��o o enunciado do trabalho e as instru��es para a entrega. Em caso de d�vidas, n�o invente. Pergunte!

A Linguagem SB

Fun��es na linguagem SB cont�m apenas atribui��es, opera��es aritm�ticas, chamadas de outras fun��es e retornos condicionais. Todas as fun��es SB s�o delimitadas por uma marca de in�cio (function) e uma marca de fim (end).

Uma atribui��o tem a forma

var '=' expr
onde var � uma vari�vel local ou um par�metro e expr � uma opera��o aritm�tica ou uma chamada de fun��o.
Uma opera��o aritm�tica tem a forma

varc op varc
onde varc � uma vari�vel local, um par�metro ou uma constante inteira e op � um dos operadores: +, - ou *.
Uma chamada de fun��o tem a forma

'call' num varc
onde num � um n�mero que identifica a fun��o SB que ser� chamada, com par�metro varc. A primeira fun��o no arquivo de entrada ser� a de n�mero 0, a segunda a de n�mero 1 e assim por diante. Uma fun��o s� pode chamar a si mesma ou fun��es que apare�am antes dela no arquivo de entrada. A �ltima fun��o do arquivo de entrada � a que ser� chamada pelo programa principal.
Um retorno condicional tem a forma

'ret?' varc varc
Seu significado � que, se o primeiro operando tiver valor igual a zero, a fun��o corrente deve retornar e o valor de retorno � o segundo operando.
Em SB as vari�veis locais s�o da forma vi, sendo o �ndice i utilizado para identificar a vari�vel (ex. v0, v1, etc...). Em SB cada fun��o usa no m�ximo 10 vari�veis locais.

Constantes em SB s�o escritas na forma $i, onde i � um valor inteiro (com sinal opcional). Por exemplo, $10 representa o valor 10 e $-10 representa o valor -10.

Par�metros s�o da forma pi, sendo p0 o primeiro par�metro, p1 o segundo, e assim sucessivamente. Contudo, � importante notar que apenas a �ltima fun��o definida no arquivo (que � chamada externamente) poder� receber mais que um par�metro (no m�ximo 10), pois a sintaxe de SB s� permite a passagem de um par�metro em uma chamada de fun��o.

A sintaxe da linguagem Limitada pode ser definida um pouco mais formalmente como abaixo:

pgm	::	func | func pgm
func	::	header cmds endf
header	::	'function\n'
endf	::	'end\n'
cmds	::	cmd '\n' | cmd '\n' cmds
cmd	::	att | ret
att	::	var ' = ' expr	
expr	::	oper | call
oper	::	varc op varc
call	::	'call ' num ' ' varc
ret	::	'ret? ' varc ' ' varc
var	::	'v' digito | 'p' digito
varc	::	var | '$' snum
op	::	' + ' | ' - ' | ' * '
num	::	digito | digito num
snum	::	[-] num
digito	::	0' | '1' | '2' | '3' | '4' | '5' | '6' | '7'| '8' | '9'

Alguns exemplos

Um exemplo trivial � uma fun��o SB que retorna uma constante:
function
ret? $0 $1
end
O pr�ximo exemplo implementa em SB uma fun��o f(x) = x + 1.

function
p0 = p0 + $1
ret? $0 p0
end
Uma fun��o SB que calcula o fatorial de seu par�metro:
function
ret? p0 $1
v0 = p0 - $1
v1 = call 0 v0
v0 = p0 * v1
ret? $0 v0
end
E, finalmente, uma fun��o que calcula a soma dos quadrados de 1 at� o seu par�metro, usando uma fun��o auxiliar para calcular o quadrado de um n�mero:
function
p0 = p0 * p0
ret? $0 p0
end
function
ret? p0 $0
v0 = call 0 p0
p0 = p0 - $1
v1 = call 1 p0
v0 = v0 + v1
ret? $0 v0
end

O que fazer

Desenvolva, em C, uma fun��o chamada gera que leia um arquivo de entrada contendo o c�digo fonte de uma ou mais fun��es SB, gere o c�digo de m�quina IA32 correspondente e retorne um ponteiro para a regi�o de mem�ria que cont�m o c�digo gerado e um ponteiro para o in�cio do c�digo da �ltima fun��o definida no arquivo de entrada.
O c�digo gerado dever� seguir as conven��es de C/Linux para a passagem de par�metros, retorno de valores e salvamento de registradores.

O prot�tipo de gera � o seguinte:

typedef int (*funcp) ();
void gera(FILE *f, void **code, funcp *entry);
O par�metro f � o descritor de um arquivo texto j� aberto para leitura, de onde deve ser lido o c�digo fonte SB. O par�metro code � o endere�o de uma vari�vel onde deve ser armazenado o ponteiro para a �rea que cont�m o c�digo gerado. O par�metro entry � o endere�o de uma vari�vel onde deve ser armazenado o ponteiro (endere�o) do c�digo da fun��o a ser chamada.
O prot�tipo de gera � o seguinte:

typedef int (*funcp) ();
void gera(FILE *f, void **code, funcp *entry);
O arquivo de entrada ter� no m�ximo 100 linhas, com um comando SB por linha.
Desenvolva tamb�m uma fun��o que libere a �rea alocada por gera, com o prot�tipo:

void libera(void *p);
Voc� dever� incluir no arquivo que cont�m as implementa��es de gera e libera o arquivo de cabe�alho gera.h, que pode ser obtido aqui. 

Execu��o

Voc� deve criar um arquivo contendo as fun��es gera e libera e outro arquivo com uma fun��o main para test�-las. Sua fun��o main dever� abrir o arquivo texto que cont�m uma ou mais fun��es SB e chamar a fun��o gera, passando o arquivo aberto como argumento e os endere�os das vari�veis que dever�o receber o endere�o do in�cio do c�digo gerado (a tradu��o das fun��es SB e o endere�o da fun��o a ser chamada (a �ltima fun��o do arquivo com o c�digo SB.. Ap�s chamar gera, sua fun��o main dever� chamar a fun��o retornada, passando os par�metros apropriados, e imprimir o valor de retorno dessa fun��o. Esse retorno � um valor inteiro, que pode ser exibido com c�digo de forma��o ("%d\n"). A fun��o chamada � a �ltima fun��o do arquivo com o c�digoSB. 

Implementa��o

A fun��o gera deve alocar um bloco de mem�ria para escrever o c�digo gerado. Os valores de retorno de gera ser�o um ponteiro para o in�cio dessa �rea e um ponteiro para o in�cio do c�digo da �ltima fun��o (dentro dessa �rea, obviamente).

O c�digo gerado por gera dever� ser um c�digo de m�quina IA-32, e n�o um c�digo fonte assembly. Ou seja, voc� dever� descobrir o c�digo de m�quina que corresponde �s instru��es de assembly que implementam cada uma das instru��es de nossa linguagem, usando o programa objdump e possivelmente a documenta��o das instru��es da Intel, dispon�vel na p�gina do curso. Por exemplo, para descobrir o c�digo gerado por movl %eax, %ecx, voc� pode criar um arquivo meuteste.s contendo apenas essa instru��o, traduzi-lo com o gcc para gerar um arquivo meuteste.o, e usar objdump -d meuteste.o para ver o c�digo de m�quina gerado.

Lembre-se que as instru��es assembly ocupam um n�mero vari�vel de bytes na mem�ria!

N�o � necess�rio fazer o tratamento de erros do arquivo de entrada, voc� pode supor que o c�digo nele estar� sempre correto. Vale a pena colocar alguns testes (ver programa exemplo abaixo) s� para facilitar a pr�pria depura��o do seu c�digo, mas as entradas usadas como testes na corre��o do trabalho sempre estar�o corretas.

Para ler e interpretar cada linha da linguagem SB, teste se a linha cont�m cada um dos formatos poss�veis. Veja um esbo�o de c�digo C para fazer a interpreta��o de c�digo aqui. (Lembre-se que voc� ter� que fazer adapta��es pois, dentre outros detalhes, essa interpreta��o n�o deve ser feita na main!!)

IMPORTANTE: Esse programa n�o � trivial. Implemente sua solu��o passo a passo, testando separadamente cada passo implementado! Por exemplo:

Compile um arquivo assembly contendo uma fun��o simples usando:
minhamaquina> gcc -m32 -c code.s
(para apenas compilar e n�o gerar o execut�vel) e depois veja o c�digo de m�quina gerado usando:
minhamaquina> objdump -d code.o
Construa um programa inicial que aloque espa�o e coloque esse c�digo "colado" do compilador, bem conhecido, para testar um esqueleto inicial.
Implemente e teste a tradu��o de uma fun��o SB que contenha apenas uma instru��o de retorno, como a primeira fun��o exemplo acima. Depois teste retornar um par�metro de entrada.
Implemente e teste cada opera��o aritm�tica por vez.
E assim por diante...
Prazo

O trabalho deve ser entregue at� meia-noite do dia 25 de novembro por correio eletr�nico. 
Ser� descontado um ponto por dia de atraso. 

Entrega

Dever�o ser entregues dois arquivos:
Um arquivo fonte chamado gera.c, contendo as fun��es gera e libera. Esse arquivo n�o deve conter a fun��o main.
Um arquivo texto, chamado relatorio.txt contendo um pequeno relat�rio descrevendo os testes realizados. Esse relat�rio, al�m de explicar o que est� funcionando e o que n�o est� funcionando, dever� mostrar alguns programas SB que voc� usou para testar o seu trabalho. Mostre tanto os programas SB compilados e executados com sucesso como os que resultaram em erros (se for o caso).

Coloque no in�cio do arquivo fonte, como coment�rio, os nomes dos integrantes do grupo da seguinte forma:
/* Nome_do_Aluno1 Matricula Turma */
/* Nome_do_Aluno2 Matricula Turma */
Coloque tamb�m no relat�rio o nome dos integrantes do grupo 

O trabalho deve ser enviado por email para o professor da turma, com subject/assunto "Trabalho 2 - Software Basico". Indique claramente os integrantes do grupo no corpo do email e inclua, como anexo, os arquivo "gera.c" e "relatorio.txt".