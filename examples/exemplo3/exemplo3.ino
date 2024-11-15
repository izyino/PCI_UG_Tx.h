//
// Exemplo n.3 de utilização da biblioteca PCI_UG_T1.h
// com WiFi no modo station, para manter acesso a internet
// ------------------------------------------------------------------
// Pressupõe um motor de passo 28byj-48 (motor n.0) conectado a CN1
// Na primeira execução deve-se conectar via access point na rede
// PCI_UG_Tx com a senha 00000000. Acessar então o IP 192.168.4.1
// em resposta, o programa exibe uma tela contendo a lista de redes
// WiFi ao alcance, para que o usuário informe qual rede será utilizada
// juntamente com a senha correspondente. Dependendo do número de redes
// ao alcance, pode demorar um certo tempo até que a lista seja exibida
//
// Informar então o número da rede escolhida e a sua senha, seguido por
// um clique no botão "submit". Após, basta apontar o browser para o IP 
// fixo normalmente igual a 192.168.1.99 ou outro, informado pela serial
// ------------------------------------------------------------------
//

#include <WiFi.h>
#include <EEPROM.h>
#include <WiFiClient.h>
#include <TimeLib.h>  
#include <WiFiAP.h>
#include <PCI_UG_Tx.h>

WiFiServer server(80);

PCI_UG_Tx x(2, 0);      //indica motor de passo no modo 2 em CN1 

String ssid="";         //não alterar. Será inicializado pelo programa
String password="";     //não alterar. Será inicializado pelo programa

const char* xssid     = "PCI_UG_T1";
const char* xpassword = "00000000";

String p1="<html><head><meta charset='utf-8'><title></title></head><body><table border='0' width='600' id='table1' cellspacing='4' cellpadding='8' height='400'><tr><td colspan='4' align='center' bgcolor='#CCFF99'><p align='center'><b><font size='4'>REDES WiFi<br>AO ALCANCE DO ESP32 DA PCI_UG_T1</font></b></p></td></tr><tr><td colspan='4' align='center'></td></tr>";
String p2="<tr><td align='center' bgcolor='#FFCCFF' width='100'>#AAA</td><td align='center' bgcolor='#FFCCFF' width='300'><p align='left'>#BBB</td><td align='center' bgcolor='#FFCCFF' width='100'>#CCC</td><td align='center' bgcolor='#FFCCFF' width='100'>#DDD</td></tr>";
String p3="<tr><td colspan='4' align='center'></td></tr><form action='/ok'><tr><td colspan='4' align='center' bgcolor='#CCFF99'><b><font size='2'>REDE ESCOLHIDA</font></b></td></tr><tr><td align='center' bgcolor='#FFFF99'><b><font size='2'>nº</font></b></td><td colspan='2' align='center' bgcolor='#FFFF99'><font size='2'><b>senha</b></font></td><td align='center' rowspan='2' bgcolor='#FFCC66'><font size='2'><input type='submit' value='Submit'></font></td></tr><tr><td align='center' bgcolor='#CCFFFF'><input type='text' id='rede' name='rede' size='7'></td><td colspan='2' align='center' bgcolor='#CCFFFF'><input type='text' id='senha' name='senha' size='40'></td></tr></form></table></body></html>";
String savP2=p2;

String html="<html><head><meta charset='utf-8'><title></title></head><body><table border='0' width='600' id='table1' cellspacing='4' cellpadding='8' height='400'><tr><td colspan='6' align='center' bgcolor='#CCFF99'><p align='center'><b><font size='4'>CONTROLE DO MOTOR DE PASSO<br>PARA TESTE DA PCI_UG_T1 VIA Wi-Fi<br></font><font color='#0000FF' size='2'>hh:mm:ss</font></b></p></td></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#FFCCFF'><font size='2'><b>VELOCIDADE</b></font></td></tr><tr><td align='center' bgcolor='#CV2' width='100'><a href='/V2'><font size='4'>&nbsp;&nbsp;2&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV3' width='100'><a href='/V3'><font size='4'>&nbsp;&nbsp;3&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV5' width='100'><a href='/V5'><font size='4'>&nbsp;&nbsp;5&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV10' width='100'><a href='/V10'><font size='4'>&nbsp;&nbsp;10&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CVvi' width='100'><a href='/V20'><font size='4'>&nbsp;&nbsp;20&nbsp;&nbsp;</font></td></a><td align='center' bgcolor='#CV40' width='100'><a href='/V40'><font size='4'>&nbsp;&nbsp;40&nbsp;&nbsp;</font></td></a></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#CCFF99'><p align='center'><font size='2'><b>DIREÇÃO</b></font></td></tr><tr><td colspan='3' align='center' bgcolor='#CDE' width='300'><a href='/DE'><font size='4'>&nbsp;&nbsp;&nbsp;&nbsp;ESQUERDA&nbsp;&nbsp;&nbsp;&nbsp;</font></td></a><td colspan='3' align='center' bgcolor='#CDD' width='300'><a href='/DD'><font size='4'>&nbsp;&nbsp;&nbsp;&nbsp;DIREITA&nbsp;&nbsp;&nbsp;&nbsp;</font></td></a></tr><tr><td colspan='6' align='center'>&nbsp;</td></tr><tr><td colspan='6' align='center' bgcolor='#99CCFF'><p align='center'><font size='2'><b>MOVIMENTO</b></font></td></tr><tr><td align='center' bgcolor='#C15' width='100'><a href='/C15'><font size='4'>&nbsp;15º&nbsp;</font></td></a><td align='center' bgcolor='#C30' width='100'><a href='/C30'><font size='4'>&nbsp;30º&nbsp;</font></td></a><td align='center' bgcolor='#C45' width='100'><a href='/C45'><font size='4'>&nbsp;45º&nbsp;</font></td></a><td align='center' bgcolor='#C90' width='100'><a href='/C90'><font size='4'>&nbsp;90º&nbsp;</font></td></a><td align='center' bgcolor='#CCP' width='100'><a href='/CCP'><font size='2'>ANDA<br>PARA</font></td></a><td align='center' bgcolor='#1VS' width='100'><a href='/1VS'><font size='2'>UMA DOSE<br>VIA SENSOR</font></td></a></tr><tr><td colspan='6' align='center'><p align='center'><br><a href='/ini'>INIFLASH</a><br>Depois de clicar em INIFLASH, conectar na rede PCI_UG_T1, senha 00000000<br> Acessar 192.168.4.1 para escolher a rede a ser utilizada</p></td></tr></table></body></html>";
String copyhtml;

const char* ntpServer = "pool.ntp.org"; //servidor NTP
const long  gmtOffset_sec = -10800;     //gmt-3 (em segundos
const int   daylightOffset_sec = 0;     //horário de verão

uint32_t nsteps;   //steps a percorrer
int      graus=30; //graus a girar (999=contínuo, 0=para)
int      vel=5;    //velocidade de deslocamento sendo máxima 3 sentido 'D'; máxima 5 sentido 'E'
char     sent='D'; //sentido esquerda ou direita (E ou D)
bool     eestate=false, fim=false;
int      n,i,k;
String   header, hora;
char     c;

void setup() {
  Serial.begin(115200);
  EEPROM.begin(128);
  pinMode(32, INPUT);
  x.begin();
  x.bdur=200;x.bfreq=2000;x.binter=0;x.bnum=1;
  x.ldur=50;x.linter=25;x.lnum=10;

  dumpflash();  //mostra o estado antes dos 128 bytes da memória flash  

  //***********************************************************************************************
  initRede(); //inicializa ssid e password. Se fot a primeira vez, escolhe a rede/senha a ser usada 
  //***********************************************************************************************

  Serial.println("");
  Serial.print("Conectando na rede: ");Serial.println(ssid);
  Serial.print("Com a senha.......: ");Serial.println(password);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("Rede WiFi conectada");
  Serial.println("");

  IPAddress X=WiFi.localIP();
  X[3]=99;IPAddress local_IP=X;
  X[3]=1; IPAddress gateway=X;
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(1, 1, 1, 1);
  IPAddress secondaryDNS(1, 0, 0, 1);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Falha na configuração da estação");
  }

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
 
  IPAddress IP = WiFi.localIP();
  Serial.println("-----------------------------------------");
  Serial.print("Rede: ");Serial.print(ssid);Serial.println(" conectada");
  Serial.print("Acesso pelo IP fixo: ");Serial.println(IP);
  Serial.print("Vcc no momento (5vcc): ");Serial.print(float(analogRead(32))/3103.0*5.0,1);Serial.println(" volts");
  Serial.println("-----------------------------------------");
  server.begin();
}

void loop(){
  WiFiClient client = server.available();
  if (client) {
    if (((graus==555)||(graus==999))&&(x.xsteps[0]>0)){x.xsteps[0]=2;} //se chegou comando via browser, aborta movimento
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

            //quando recebe um comando do browser (diferente de /ini),
            //emite um beep de 0.2s e pisca o led 10 vezes
            if (header.indexOf("GET /ini") != 0) {
              x.bdur=200;x.bfreq=2000;x.binter=0;x.bnum=1;
              x.ldur=50;x.linter=25;x.lnum=10;
            }
            
            //
            // antesiza as variáveis vel, sent e graus
            // conforme os clicks que foram dados na página lá no cliente
            //

            if (header.indexOf("GET /V2")  >= 0) {vel=2;}
            if (header.indexOf("GET /V3")  >= 0) {vel=3;}
            if (header.indexOf("GET /V5")  >= 0) {vel=5;}
            if (header.indexOf("GET /V10") >= 0) {vel=10;}
            if (header.indexOf("GET /V20") >= 0) {vel=20;}
            if (header.indexOf("GET /V40") >= 0) {vel=40;}
            
            if (header.indexOf("GET /DE") >= 0) {sent='E';}
            if (header.indexOf("GET /DD") >= 0) {sent='D';}

            if (header.indexOf("GET /C15") >= 0) {graus=15;}
            if (header.indexOf("GET /C30") >= 0) {graus=30;}
            if (header.indexOf("GET /C45") >= 0) {graus=45;}
            if (header.indexOf("GET /C90") >= 0) {graus=90;}
            if (header.indexOf("GET /CCP") >= 0) {if (graus==999){graus=0;}else{graus=999;}}
            if (header.indexOf("GET /1VS") >= 0) {graus=555;}

            if (header.indexOf("GET /ini") >= 0) {
              for (k=0; k<128; k++){EEPROM.write(k, 0x00);};EEPROM.commit();
              x.xtime[0]=0;x.xtime[1]=0;x.xtime[2]=0;x.xtime[3]=0;
              x.xsteps[0]=0;x.xsteps[1]=0;x.bnum=0;x.lnum=0;
              delay(1000);ESP.restart();
            }

            copyhtml=html;

            //
            // antesiza as cores de destaque das 
            // variáveis vel, sent e graus conforme os valores atuais
            //
            
            if (vel==2) {copyhtml.replace("#CV2", "#FF9999");}else{copyhtml.replace("#CV2", "#FFCCFF");}
            if (vel==3) {copyhtml.replace("#CV3", "#FF9999");}else{copyhtml.replace("#CV3", "#FFCCFF");}
            if (vel==5) {copyhtml.replace("#CV5", "#FF9999");}else{copyhtml.replace("#CV5", "#FFCCFF");}
            if (vel==10){copyhtml.replace("#CV10","#FF9999");}else{copyhtml.replace("#CV10","#FFCCFF");}
            if (vel==20){copyhtml.replace("#CVvi","#FF9999");}else{copyhtml.replace("#CVvi","#FFCCFF");}
            if (vel==40){copyhtml.replace("#CV40","#FF9999");}else{copyhtml.replace("#CV40","#FFCCFF");}

            if (sent=='E') {copyhtml.replace("#CDE", "#33CC33");}else{copyhtml.replace("#CDE", "#CCFF99");}
            if (sent=='D') {copyhtml.replace("#CDD", "#33CC33");}else{copyhtml.replace("#DDD", "#CCFF99");}

            if (graus==15) {copyhtml.replace("#C15", "#0099FF");}else{copyhtml.replace("#C15", "#99CCFF");}
            if (graus==30) {copyhtml.replace("#C30", "#0099FF");}else{copyhtml.replace("#C30", "#99CCFF");}
            if (graus==45) {copyhtml.replace("#C45", "#0099FF");}else{copyhtml.replace("#C45", "#99CCFF");}
            if (graus==90) {copyhtml.replace("#C90", "#0099FF");}else{copyhtml.replace("#C90", "#99CCFF");}
            if (graus==999){copyhtml.replace("#CCP", "#0099FF");}else{copyhtml.replace("#CCP", "#99CCFF");}
            if (graus==555){copyhtml.replace("#1VS", "#0099FF");}else{copyhtml.replace("#1VS", "#99CCFF");}

            conectantp();
            copyhtml.replace("hh:mm:ss", hora);
            
            // envia a página com os botões antesizados
            client.println(copyhtml);

            //starta o motor de acordo com o comando recebido
            if (graus!=555){
              while(x.xsteps[0]>0){}
              if (graus==999){nsteps=2048000000;}
              if (graus!=999){nsteps=map(graus,0,360,0,2048);}
              if (sent=='D'){x.runStep(0, nsteps, vel, true);}else{x.runStep(0, nsteps, vel, false);}
            }  

            analogReadResolution(12);

            //se foi para rodar uma dose baseado só no sensor, executa
            if (graus==555){
              Serial.println("");
              bool dir;
              if (sent=='D'){dir=true;}else{dir=false;}
              while (analogRead(34)!=4095){                                       //4095 é preto
                Serial.print(analogRead(34));
                while(x.xsteps[0]>0){};
                x.runStep(0, 6, vel, dir); //gira aprox 1 grau por vez até o início de uma divisão (preto)
              }  
              Serial.print(analogRead(34));Serial.println("");
              while(x.xsteps[0]>0){};
              while (analogRead(34)==4095){                                       //dif. de 4095 é espaço
                Serial.print(analogRead(34));
                while(x.xsteps[0]>0){};
                x.runStep(0, 6, vel, dir); //gira aprox 1 grau por vez até o fim da divisão (sai do preto)
              }
              Serial.print(analogRead(34));Serial.println("");
              while(x.xsteps[0]>0){};
              x.runStep(0, 18, vel, dir);  //ao final, avança aprox. 3 graus para alinhar a pá com a rampa 
            }

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


void initRede()
{
  if ((EEPROM.read(126)==0x55)&&(EEPROM.read(127)==0xAA)){
    ssid="";password="";
    for (k=0; k<EEPROM.read(124); k++){ssid=ssid+char(EEPROM.read(k));}
    for (k=0; k<EEPROM.read(125); k++){password=password+char(EEPROM.read(k+32));}
    Serial.println("Memoria EEPROM ok. Parametros recuperados com sucesso...");
    eestate=true;
  }else{
    for (k=0; k<128; k++){EEPROM.write(k, 0x00);}EEPROM.commit();
    WiFi.softAP(xssid, xpassword);
    IPAddress IP = WiFi.softAPIP();
    Serial.println("");    
    Serial.println("-----------------------------------------");
    Serial.println("Rede: PCI_UG_T1");
    Serial.println("Senha: 00000000");
    Serial.print("Acesso pelo IP: ");Serial.println(IP);
    Serial.print("Vcc no momento (5vcc): ");Serial.print(float(analogRead(32))/3103.0*5.0,1);Serial.println(" volts");
    Serial.println("-----------------------------------------");
    Serial.println("");
    server.begin();
    fim=false;
    while (!fim){looprede();}
  }
}    


void looprede()
{
  WiFiClient client = server.available();

  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
         
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            x.ldur=50;x.linter=25;x.lnum=2;
        
            n = WiFi.scanNetworks();
            if (n>9){n=9;}
            client.print(p1);
                  
            if (n > 0) {
              for (i = 0; i < n; i++) {
                p2=savP2;
                String(i+10);
                p2.replace("#AAA", String(i+11));
                p2.replace("#BBB", WiFi.SSID(i));
                p2.replace("#CCC", String(WiFi.RSSI(i)));
                if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN){p2.replace("#DDD", "aberta");}else{p2.replace("#DDD", "*");}

                client.print(p2);
              }
              delay(10);
            }
            client.print(p3);
            client.println();
            break;
          } else {

            if (currentLine.startsWith("GET /ok?rede=")) //se recebeu n.rede e senha, grava EEPROM e finaliza
            {
              int nr=currentLine.substring(13,15).toInt()-11;
              i=int(currentLine.length())-9;
              String se=currentLine.substring(22, i);
              i=se.length();

              EEPROM.write(124, WiFi.SSID(nr).length());
              char senha[34],rede[34];
              ssid=WiFi.SSID(nr);
            
              ssid.toCharArray(rede, ssid.length()+1);
              EEPROM.write(124, ssid.length());
              for (k=0; k<=ssid.length()+1; k++){EEPROM.write(k, rede[k]);}
              
              se.toCharArray(senha, se.length()+1);
              EEPROM.write(125, i);
              for (k=0; k<=i; k++){EEPROM.write(k+32, senha[k]);}

              EEPROM.write(126, 0x55);EEPROM.write(127, 0xAA);
              EEPROM.commit();

              ssid="";password="";
              for (k=0; k<EEPROM.read(124); k++){ssid=ssid+char(EEPROM.read(k));}
              for (k=0; k<EEPROM.read(125); k++){password=password+char(EEPROM.read(k+32));}
          
              fim=true;

              x.xtime[0]=0;x.xtime[1]=0;x.xtime[2]=0;x.xtime[3]=0;
              x.xsteps[0]=0;x.xsteps[1]=0;x.bnum=0;x.lnum=0;
              delay(1000);ESP.restart();
            }  

            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
  }
}


void dumpflash()
{
  uint8_t xbyte;
  uint16_t addr, k; char hexVal[5];
  Serial.println("");
  for (addr=0; addr<127; addr=addr+16){
    sprintf(hexVal, "%04d", addr);
    Serial.print(hexVal);Serial.print("  ");  
    sprintf(hexVal, "%04X", addr);
    Serial.print(hexVal);Serial.print("  ");  
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);
      sprintf(hexVal, "%02X", (xbyte));Serial.print(hexVal);Serial.print(" ");
    }
    Serial.print(" ");
    for (k=0; k<16; k=k+1){
      xbyte=EEPROM.read(addr+k);         
      Serial.write(xbyte>31 && xbyte != 127 ? char (xbyte) : '.');
    }
    Serial.println("");
  }
  Serial.println("");
}  

void conectantp(){
  struct tm timeinfo;
  bool chntp=false;
  k=0;
  uint32_t milisatu=millis();
  while (!getLocalTime(&timeinfo)){
    if ((k>10)||(milisatu+1000)<millis()){
      chntp=false;break;
    }  
  }

  if (getLocalTime(&timeinfo)){chntp=true;}

  hora="??:??:??";
  
  if (chntp){
    byte h,m,s;
    h=timeinfo.tm_hour;
    m=timeinfo.tm_min;
    s=timeinfo.tm_sec;
    hora="";

    if (h<10){hora="0"+String(h);}else{hora=hora+String(h);}
    hora=hora+":";
    if (m<10){hora=hora+"0"+String(m);}else{hora=hora+String(m);}
    hora=hora+":";
    if (s<10){hora=hora+"0"+String(s);}else{hora=hora+String(s);}
  }
} 
