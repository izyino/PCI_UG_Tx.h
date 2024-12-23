
//
// Exemplo nº 2 de utilização da biblioteca library PCI_UG_Tx.h
// via Wi-Fi, com o servidor web do ESP-32 definido como access-point
// ------------------------------------------------------------------
// Pressupõe um motor de passo 28byj-48 (motor nº 0) conectado a CN1
// Rede Wi-Fi criada com o nome PCI_UG_Tx senha 00000000, acessí­vel
// pelo IP 192.168.4.1 ou outro, informado pelo monitor serial
// ------------------------------------------------------------------
//

#include <WiFi.h>
#include <PCI_UG_Tx.h>

PCI_UG_Tx x(2, 0);   //indica motor de passo no modo 2 em CN1 

const char* ssid     = "PCI_UG_Tx";
const char* password = "00000000";

#define LED_BUILTIN 0

WiFiServer server(80);

String header;

String html="<html><head><meta charset='utf-8'><title></title></head><body><table border='0' width='600' id='table1' cellspacing='4' cellpadding='4' height='400'><tr><td colspan='6' align='center'><p align='center'><b><font size='4'>CONTROLE DO MOTOR DE PASSO<br>PARA TESTE DA PCI_UG_Tx VIA Wi-Fi</font></b></p></td></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#FFCCFF'><font size='2'><b>VELOCIDADE (RPM)</b></font></td></tr><tr><td align='center' bgcolor='#CV1' width='100'><a href='/V1'><font size='4'>&nbsp;&nbsp;1&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV3' width='100'><a href='/V3'><font size='4'>&nbsp;&nbsp;3&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV5' width='100'><a href='/V5'><font size='4'>&nbsp;&nbsp;5&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV8' width='100'><a href='/V8'><font size='4'>&nbsp;&nbsp;8&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV912' width='100'><a href='/V12'><font size='4'>&nbsp;&nbsp;12&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV916' width='100'><a href='/V16'><font size='4'>&nbsp;&nbsp;16&nbsp;&nbsp;</font></td></a></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#CCFF99'><p align='center'><font size='2'><b>DIREÇÃO</b></font></td></tr><tr><td colspan='3' align='center' bgcolor='#CDE' width='300'><a href='/DE'><font size='4'>&nbsp;&nbsp;&nbsp;&nbsp;ESQUERDA&nbsp;&nbsp;&nbsp;&nbsp;</font></td></a><td colspan='3' align='center' bgcolor='#CDD' width='300'><a href='/DD'><font size='4'>&nbsp;&nbsp;&nbsp;&nbsp;DIREITA&nbsp;&nbsp;&nbsp;&nbsp;</font></td></a></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#99CCFF'><p align='center'><font size='2'><b>MOVIMENTO</b></font></td></tr><tr><td align='center' bgcolor='#C30' width='100'><a href='/C30'><font size='4'>&nbsp;30º&nbsp;</font></td></a><td align='center' bgcolor='#C45' width='100'><a href='/C45'><font size='4'>&nbsp;45º&nbsp;</font></td></a><td align='center' bgcolor='#C90' width='100'><a href='/C90'><font size='4'>&nbsp;90º&nbsp;</font></td></a><td align='center' bgcolor='#C180' width='100'><a href='/C180'><font size='4'>&nbsp;180º&nbsp;</font></td></a><td align='center' bgcolor='#C360' width='100'><a href='/C360'><font size='2'>&nbsp;360º&nbsp;</font></td></a><td align='center' bgcolor='#AP' width='100'><a href='/AP'><font size='2'>ANDA<br>PARA</font></td></a></tr></table></body></html>";
String copyhtml;

uint32_t nsteps;   //steps a percorrer
int      graus=30; //graus a girar (999=contí­nuo, 0=para)
int      vel=5;    //velocidade de deslocamento (RPM), de 1 a 16
char     sent='D'; //sentido esquerda ou direita (E ou D)

void setup() {
  Serial.begin(115200);
  pinMode(32, INPUT);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("-----------------------------------------");
  Serial.println("Rede: PCI_UG_Tx");
  Serial.println("Senha: 00000000");
  Serial.print  ("Acesso pelo IP: ");Serial.println(IP);
  Serial.print  ("Vcc no momento (5vcc): ");Serial.print(float(analogRead(32))/3103.0*5.0,1);Serial.println(" volts");
  Serial.println("-----------------------------------------");
  server.begin();
  x.begin();
}

void loop()
{
  WiFiClient client = server.available();
  if (client) {
    if ((graus==999)&&(x.xsteps[0]>0)){x.stopStep(0);} //se chegou comando via browser, aborta movimento
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //quando recebe um comando do browser, emite beep e pisca o led
            x.beep(1, 250, 1440, 0); // 1 beep, 0.25 seg, 1440 Hz
            x.led(20, 100, 50);      // 20 piscadas, 100ms aceso, 50ms apagado
            
            //
            // atualiza as variáveis vel, sent e graus
            // conforme os clicks que foram dados na página lá no cliente
            //

            if (header.indexOf("GET /V1")  >= 0) {vel=1;}
            if (header.indexOf("GET /V3")  >= 0) {vel=3;}
            if (header.indexOf("GET /V5")   >= 0) {vel=5;}
            if (header.indexOf("GET /V8")   >= 0) {vel=8;}
            if (header.indexOf("GET /V12")  >= 0) {vel=12;}
            if (header.indexOf("GET /V16")  >= 0) {vel=16;}
            
            if (header.indexOf("GET /DE")   >= 0) {sent='E';}
            if (header.indexOf("GET /DD")   >= 0) {sent='D';}

            if (header.indexOf("GET /C30")  >= 0) {graus=30;}
            if (header.indexOf("GET /C45")  >= 0) {graus=45;}
            if (header.indexOf("GET /C90")  >= 0) {graus=90;}
            if (header.indexOf("GET /C180") >= 0) {graus=180;}
            if (header.indexOf("GET /C360") >= 0) {graus=360;}
            if (header.indexOf("GET /AP")   >= 0) {if (graus==999){graus=0;}else{graus=999;}}
            
            copyhtml=html;

            //
            // atualiza as cores de destaque das 
            // variáveis vel, sent e graus conforme os valores atuais
            //
            
            if (vel==1)    {copyhtml.replace("#CV1", "#FF9999");}  else {copyhtml.replace("#CV1", "#FFCCFF");}
            if (vel==3)    {copyhtml.replace("#CV3", "#FF9999");}  else {copyhtml.replace("#CV3", "#FFCCFF");}
            if (vel==5)    {copyhtml.replace("#CV5","#FF9999");}   else {copyhtml.replace("#CV5","#FFCCFF");}
            if (vel==8)    {copyhtml.replace("#CV8","#FF9999");}   else {copyhtml.replace("#CV8","#FFCCFF");}
            if (vel==12)   {copyhtml.replace("#CV912","#FF9999");} else {copyhtml.replace("#CV912","#FFCCFF");}
            if (vel==16)   {copyhtml.replace("#CV916","#FF9999");} else {copyhtml.replace("#CV916","#FFCCFF");}

            if (sent=='E') {copyhtml.replace("#CDE", "#33CC33");}  else {copyhtml.replace("#CDE", "#CCFF99");}
            if (sent=='D') {copyhtml.replace("#CDD", "#33CC33");}  else {copyhtml.replace("#DDD", "#CCFF99");}

            if (graus==30) {copyhtml.replace("#C30", "#0099FF");}  else {copyhtml.replace("#C30", "#99CCFF");}
            if (graus==45) {copyhtml.replace("#C45", "#0099FF");}  else {copyhtml.replace("#C45", "#99CCFF");}
            if (graus==90) {copyhtml.replace("#C90", "#0099FF");}  else {copyhtml.replace("#C90", "#99CCFF");}
            if (graus==180){copyhtml.replace("#C180", "#0099FF");} else {copyhtml.replace("#C180", "#99CCFF");}
            if (graus==360){copyhtml.replace("#C360", "#0099FF");} else {copyhtml.replace("#C360", "#99CCFF");}
            if (graus==999){copyhtml.replace("#AP", "#0099FF");}   else {copyhtml.replace("#AP", "#99CCFF");}
            
            // envia a página com os botões atualizados
            client.println(copyhtml);

            //starta o motor de acordo com os comandos ativos
            while(x.xsteps[0]>0){}
            if (graus==999){nsteps=2048000000;}
            if (graus!=999){nsteps=map(graus,0,360,0,2048);}
            if (sent=='D'){x.runStep(0, nsteps, vel, true);}else{x.runStep(0, nsteps, vel, false);}

            client.println();
            break;
          } else {
            currentLine = "";
        }} else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
  }
}

//fim