Para facilitar o desenvolvimento de aplicações a serem hospedadas nas placas PCI_UG_Tx, foi criada uma biblioteca de nome PCI_UG_Tx.h, disponível em https://github.com/izyino/PCI_UG_Tx.h a qual dispõe das seguintes funções:
--------------------------------------------------------------------------------------------------------

#include <PCI_UG_Tx.h>

para incluir a biblioteca ao programa. Dependendo de onde a biblioteca estiver gravada, pode-se usar alternativamente o formato #include “PCI_UG_T1.h”


--------------------------------------------------------------------------------------------------------
PCI_UG_Tx x(t0, t1);

comando construtor que deve ser informado logo após o include, sendo t0, t1 variáveis do tipo uint8_t que definem o tipo de motor conectado a CN1 e CN2 respectivamente, sendo possível os seguintes valores:

0 – Para motor DC<br>
1 – Para motor 28byj-48, 2048 passos por volta, baixo torque, baixo consumo<br>
2 – Para motor 28byj-48, 2048 passos por volta, alto torque, alto consumo<br>
3 – Para motor 28byj-48, 4096 passos por volta, médio torque, médio consumo<br>
4 - Para motor Nema17, 200 passos por volta, modo único<br>

--------------------------------------------------------------------------------------------------------
x.begin();

inicializa as diversas funções da biblioteca. Deve ser colocado na sessão de setup de todos os programas que se utilizem da biblioteca 

--------------------------------------------------------------------------------------------------------
x.runStep(n, steps, velstep, cwstep);

comando que ativa o motor de passo, de forma automática e assíncrona, conforme as seguintes variáveis:

n – variável uint8_t contendo o número do motor que será movimentado (0 ou 1)<br>
Se n=0, o motor de passo deverá estar conectado ao CN1<br>
Se n=1, o motor de passo deverá estar conectado ao CN2<br>

steps – variável uint32_t contendo o número de passos a movimentar

velstep – variável unint8_t que define a velocidade da movimentação em RPM (Rotações Por Minuto). Pode variar de 1 a 16 para os motores 28byj-48

cwstep – variável booleana que define o sentido da movimentação, sendo “true” para sentido horário e “false” para sentido anti-horário 

--------------------------------------------------------------------------------------------------------
x.where(n);

esta função retorna no formato uint32_t o número de passos ainda restantes para que o motor n (n=0 ou 1) chegue ao seu destino. Zero significa que o motor n já chegou ao seu último destino e já encontra-se parado. Antes de comandar qualquer movimentação deve-se consultar esta função para ter certeza que o motor n encontra-se parado. A variável n é do tipo uint8_t

--------------------------------------------------------------------------------------------------------
x.runDC(n, time, veldc, cwdc);

comando que ativa o motor DC, de forma automática e assíncrona, conforme as seguintes variáveis:

n – variável uint8_t com número do motor DC que será movimentado (0, 1, 2 ou 3):<br>
Se	n=0,	o motor DC deverá estar conectado aos pinos 1 e 2 do CN1<br>
Se	n=1,	o motor DC deverá estar conectado aos pinos 3 e 4 do CN1<br>
Se	n=2,	o motor DC deverá estar conectado aos pinos 1 e 2 do CN2<br>
Se	n=3,	o motor DC deverá estar conectado aos pinos 3 e 4 do CN2<br>
 
time – variável uint32_t contendo o tempo em milisegundos que o motor DC ficará ativado

velDC – variável unint8_t que define a velocidade da movimentação, em termos de  porcentagem entre 0 e 100. Sendo 0=0% motor parado, 100=100% motor com velocidade máxima.

cwDC – variável booleana que define o sentido da movimentação, sendo “true” para sentido horário e “false” para sentido anti-horário 

--------------------------------------------------------------------------------------------------------
x.beep(bnum, bdur, bfreq, binter);

comando que ativa a emissão de beeps sonoros, de forma automática e assíncrona, conforme as seguintes variáveis:

bnum – variável inteira que especifica o número de beeps a serem emitidos

bdur – variável inteira que especifica a duração de cada beep, em milisegundos 

bfreq – variável inteira que especifica a freqüência dos beeps, em Hertz (Hz)

binter – variável inteira que especifica a duração da pausa entre os beeps, em milisegundos 

--------------------------------------------------------------------------------------------------------
x.led(lnum, ldur, linter);

comando que ativa piscadas do Led, de forma automática e assíncrona, conforme as seguintes variáveis:

lnum – variável inteira que especifica o número de piscadas a serem emitidas

ldur – variável inteira que especifica a duração do Led acesso em cada piscada, em milisegundos 

linter – variável inteira que especifica a duração do Led apagado em cada piscada, em milisegundos 

--------------------------------------------------------------------------------------------------------
x.setms(yms);

comando para inicializar o contador de milisegundos com o valor informado pela variável yms do tipo uint32_t. Imediatamente após inicializado, o contador começa ser subtraído de 1 a cada milisegundo

--------------------------------------------------------------------------------------------------------
x.getms();

esta função retorna no formato uint32_t o estado antes do contador de milisegundos previamente inicializado pelo comando x.setms. Serve como alternativa para a função delay(), de forma assíncrona

--------------------------------------------------------------------------------------------------------
x.stopStep(n);

esta função interrompe o movimento do motor de passo n (n=0 ou 1)

--------------------------------------------------------------------------------------------------------
x.stopDC(n);

esta função interrompe o movimento do motor DC n (n=0, 1, 2 ou 3)

--------------------------------------------------------------------------------------------------------
x.stopBeep();

esta função interrompe a emissão de beeps sonoros

--------------------------------------------------------------------------------------------------------
x.stopLed();

esta função interrompe as piscadas do Led

############################################################################################<br>
Como alternativa para algumas das funções e comandos acima, pode-se acessar diretamente algumas variáveis internas da biblioteca. Entretanto, tais acessos se feitos de forma indevida, podem provocar erros e paradas inesperadas. Recomenda-se portanto que as variáveis relacionadas sejam acessadas diretamente apenas em último caso e apenas por programadores experientes:


variáveis de controle dos beeps sonoros
--------------------------------------------------------------------------------------------------------
x.bdur	duração dos beeps (ms)*10<br>
x.binter	pausa interbeeps (ms)*10<br>
x.bfreq	frequência dos beeps (Hz)<br>
x.bnum	quantidade de beep+pausa a emitir <br>


variáveis de controle das piscadas do Led
--------------------------------------------------------------------------------------------------------
x.ldur 	duração do Led aceso (ms)*10<br>
x.linter 	duração do Led apagado (ms)*10<br>
x.lnum	quantidade de piscadas<br>


variáveis de controle dos motores de passo (índice n abaixo entre 0 e 1)
--------------------------------------------------------------------------------------------------------
x.xtipostep[n]	tipo do motor de passo n (pode ser 0,1,2 ou 3)<br>
x.xsteps[n]	quantidade de passos a mover<br>
x.xvelstep[n]	velocidade (RPM)<br>
x.xvelnow[n]	velocidade no momento (vide .cpp)<br>
x.xfase[n]	fase antes do ciclo do motor  (vide .cpp)<br>
x.xcwstep[n]	sentido (1=true=horário, 0=false=anti-horário)<br>


variáveis de controle dos motores DC (índice n abaixo entre 0 e 3)
--------------------------------------------------------------------------------------------------------
x.xtime[n] 	tempo a movimentar (ms)*10<br>
x.xveldc[n]	velocidade (0=parado, 100=velocidade máxima)<br>
x.xcwdc[n] 	sentido (1=true=horário, 0=false=anti-horário)<br>


variável de controle temporal
--------------------------------------------------------------------------------------------------------
x.xms	quantidade desejada de milisegundos*10


############################################################################################<br>
Exemplos de utilização da biblioteca<br>

No início do programa:<br>
#include <PCI_UG_Tx.h><br>
PCI_UG_Tx x(2, 0);<br>

na sessão do setup:<br>
x.begin();<br>

--------------------------------------------------------------------------------------------------------
//movimenta o motor de passo n.0 (conectado em CN1), tipo 28BYJ-48, <br>
//velocidade 3, sentido horário, 2048 passos:<br>

//via chamada convencional:<br>
x.runStep(0, 2048, 3, true);<br>

//via acesso direto as variáveis da biblioteca:<br>
x.xtipostep[0]=2; x.xvelstep[0]=3; x.xcwstep[0]=1; x.xsteps[0]=2048;<br>
//o motor começa a se movimentar imediatamente após a variável x.xsteps ser inicializada<br>

//para saber se o motor de passo n.0 já chegou ao destino, fazer<br>
//via chamada convencional:<br>
if (x.where(0)>0) {ainda não chegou ao destino. Está em movimento...};<br>

//via acesso direto as variáveis da biblioteca:<br>
if (x.xsteps[0]>0) {ainda não chegou ao destino. Está em movimento...};<br>

//a qualquer momento o movimento do motor de passo n.0 pode ser interrompido<br>
//via chamada convencional:<br>
x.stopStep(0);<br>

//via acesso direto as variáveis da biblioteca:<br>
x.xsteps[0]=0;<br>

--------------------------------------------------------------------------------------------------------
//movimenta o motor DC n.3 (conectado aos pinos 1 e 2 do CN2),<br>
//velocidade 75%, sentido anti-horário, durante 15segundos:<br>

//via chamada convencional:<br>
x.runDC(3, 15000, 75, false);<br>

//via acesso direto as variáveis da biblioteca:<br>
x.xveldc[3]=75; x.xcwdc[3]=0; x.xtime[3]=150000(*);<br>
//o motor começa a se movimentar imediatamente após a variável x.xtime ser inicializada<br>

//a qualquer momento o movimento do motor DC n.3 pode ser interrompido<br>
//via chamada convencional:<br>
x.stopDC(3);

//via acesso direto as variáveis da biblioteca:<br>
x.xtime[3]=0;<br>

--------------------------------------------------------------------------------------------------------
//emite 10 beeps de 2KHz de 0,5s com pausa interbeeps de 0,25s:<br>

//via chamada convencional:<br>
x.beep(10, 500, 2000, 250);<br>

//via acesso direto as variáveis da biblioteca:<br>
x.bdur=5000(*); x.binter=2500(*); x.bfreq=2000; x.bnum=10;<br>
//os beeps começam a ser emitidos imediatamente após a variável x.bnum ser inicializada<br>

//a qualquer momento a emissão dos beeps sonoros pode ser interrompida<br>
//via chamada convencional:<br>
x.stopBeep();<br>

//via acesso direto as variáveis da biblioteca:<br>
x.bnum=0;<br>

--------------------------------------------------------------------------------------------------------
//pisca o Led 50 vezes com 0,25s aceso seguido de 0,10s apagado: <br>

//via chamada convencional:<br>
x.led(50, 250, 100);<br>

//via acesso direto as variáveis da biblioteca:<br>
x.ldur=2500(*); x.linter=1000(*); x.lnum=50;<br>
//o Led começa a piscar imediatamente após a variável x.lnum ser inicializada<br>

//a qualquer momento as piscadas do Led podem ser interrompidas<br>
//via chamada convencional:<br>
x.stopLed();<br>

//via acesso direto as variáveis da biblioteca:<br>
x.lnum=0;<br>

--------------------------------------------------------------------------------------------------------
//contagem de 4 segundos, de forma assíncrona:<br>

//via chamada convencional:<br>
x.setms(4000);while (x.getms()>0){enquanto espera 4s, pode fazer coisas…}<br>

//via acesso direto as variáveis da biblioteca:<br>
x.xms=40000(*); while (x.xms>0){enquanto espera 4s, pode fazer coisas…}<br>
//a variável x.xms começa a ser decrementada a cada um milisegundo imediatamente após ter sido inicializada<br>

############################################################################################<br>
(*) - Esses valores são sempre multiplicados por 10 quando tratar-se de acesso direto as variáveis da biblioteca<br>
############################################################################################<br>
