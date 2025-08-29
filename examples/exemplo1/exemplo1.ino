
//
// Exemplo n.1 de utilização da biblioteca PCI_UG_Tx.h
// emite beeps e piscadas de led, e movimentos repetitivos
// -----------------------------------------------------------
// Pressupõe um motor de passo 28byj-48 (motor n.0) conectado a CN1
// e dois motores DC (motor DC n.2 nos pinos 1 e 2 de CN2) 
// e (motor DC n.3 nos pinos 3 e 4 de CN2) 
// -----------------------------------------------------------
//


#include <PCI_UG_Tx.h>
PCI_UG_Tx x(2, 0);           //indica motor de passo no modo 2 em CN1 e motor(es) DC em CN2 

void setup() {
  x.begin();
  Serial.begin(115200);
}

void loop() {
  x.beep(2, 200, 2000, 100);  //emite 2 beep de 200ms cada, 2000Hz, intervalo entre eles de 100ms 
  x.led(20, 100, 50);         //pisca o LED 20 vezes com 100ms aceso e 50ms apagado

  x.runDC(2, 2000, 50, 1);    //ativa o motor DC n.2 por 2 segundos, PWM 50%, setido horário
  x.runDC(3, 2000, 50, 0);    //ativa o motor DC n.3 por 2 segundos, PWM 50%, setido antihorário

  if (x.where(0)==0){         //se o motor de passo n.0 já chegou ao seu último destino (está parado)
    x.runStep(0, 2048, 12, 1);//ativa esse motor para andar 2048 passos, velocodade 12, sentido horário 
  }
   
  x.setms(6000);
  while(x.getms()>0){}        //espera 6 segundos para que os comandos acima terminem e repete tudo 
}
