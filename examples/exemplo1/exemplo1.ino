
//
// Exemplo nº 1 de utilização da biblioteca library PCI_UG_Tx.h
// emite beeps e piscadas de led, e movimentos repetitivos
// -----------------------------------------------------------
// Pressupõe um motor de passo 28byj-48 (motor nº 0) conectado
// a CN1 e dois motores DC (motor DC nº 2 nos pinos 1 e 2 de
// CN2) e (motor DC nº 3 nos pinos 3 e 4 de CN2) 
// -----------------------------------------------------------
//

#include <Arduino.h>
#include <PCI_UG_Tx.h>

bool s=false;
uint8_t RPM=16;

PCI_UG_Tx x(2, 0);            //indica motor de passo no modo 2 em CN1 e motor(es) DC em CN2 

void setup() {
  x.begin();
  Serial.begin(115200);
}

void loop() {
  x.beep(2, 200, 1440, 100);     //emite 2 beep de 200ms cada, 1440Hz, intervalo entre eles de 100ms 
  x.led(25, 100, 50);            //pisca o LED 20 vezes com 100ms aceso e 50ms apagado

  x.runDC(2, 3000, 60, s);       //ativa o motor DC nº 2 por 3 segundos, PWM 60%
  x.runDC(3, 6000, 50, !s);      //ativa o motor DC nº 3 por 6 segundos, PWM 50%, setido invertido

  if (x.where(0)==0){            //se o motor de passo nº 0 já chegou ao seu último destino (está parado)
    s=!s;                        //inverte o sentido anterior
    x.runStep(0, 2048*2, RPM, s);//ativa o motor de passo para dar duas voltas (2048*2 passos)
  }
  
  x.setms(7500);
  while(x.getms()>0){}           //espera 7,5 segundos 
}

//fim
