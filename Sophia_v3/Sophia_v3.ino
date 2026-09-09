//De um apaixonado por tecnologia parte dois
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>

int cont=0;
int minDia;
int capturaTemp;
bool sinal=false;
bool stsQua=false;
bool stsExt=false;
bool stsCoz=false;
bool bombPlant=false;
bool umaVezBom=false;
bool direto = true;
String site;
String comando;
unsigned long agora=0;
unsigned long antes=0;
unsigned long antesOf=0;
unsigned long antesVi=0;
const int fuso=-3*3600;
byte hora;
byte minuto;

const int perConex = 100;
const int perVig = 30000;
const unsigned long perData=300000; //5 minutos
const byte pinLED = 2;
const byte pinExt = 12;
const byte pinCoz = 13;
const byte pinQua = 14;
const byte pinBom = 15;

IPAddress ipESP(192,168,1,42);
IPAddress ipRot(192,168,1,254);
IPAddress Masc(255,255,255,0);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP,fuso);

const char* rede = "Dalekinho";
const char* senha = "banacafalata";

WiFiServer concursado(80); //todo concursado é um SeRvIDoR, entendeu? kkkkkkk

void setup() {
Serial.begin(115200);
timeClient.begin();
  pinMode(pinExt, OUTPUT);
  pinMode(pinCoz, OUTPUT);
  pinMode(pinQua, OUTPUT);
  pinMode(pinBom, OUTPUT);
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinCoz,0);
  digitalWrite(pinQua,0);
  digitalWrite(pinExt,0);
  digitalWrite(pinBom,0);
    WiFi.begin(rede, senha);
      while (!WiFi.isConnected()){
        agora=millis();
          if (agora-antes>perConex){
            Serial.print("-");
              digitalWrite(pinLED,sinal);
          sinal=!sinal;
          cont++;  
          antes=agora;  
          }
          if (cont>3000){
            ESP.restart();
          }
      }
        Serial.println("| Dentro ;)");
        digitalWrite(pinLED,HIGH); //vai manter a luz desligada (sim o comando é invertido mesmo) para não ofuscar a noite
          WiFi.config(ipESP, ipRot, Masc); //GARANTE O IP FIXO (Em caso de aplicação externa desativar isso)
          //Serial.print("IP(var) ESP: ");
          //Serial.println(WiFi.localIP());
            concursado.begin();
              Serial.println("Servidor Iniciado");
              site="<!DOCTYPE html> <html lang=\"pt-br\"> <head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>qua:0-coz:0-ext:0-bom:0</title> <style> .box{ display: flex; flex-direction: column; justify-content: center; width: 45vw; height: 30vh; } .info { display: flex; justify-content: center; align-items: center; height: 50px; color: #290404; border: 1px solid #031823; border-radius: 10px; font-size: 25px; margin-bottom: 10px; } body { background-color: #01180b; display: flex; flex-direction: row; justify-content: center; } </style> </head> <body> <span class=\"box\"> <span id=\"qua\" style=\"background-color: #252525;\" class=\"info\">Quarto: apagado</span> <span id=\"coz\" style=\"background-color: #252525;\" class=\"info\">Cozinha: apagado</span> <span id=\"ext\" style=\"background-color: #252525;\" class=\"info\">Externo: apagado</span> <span id=\"bom\" style=\"background-color: #252525;\" class=\"info\">Bomba: desligado</span> </span> </body> </html>";
                antes=0;
}   

void loop() {
agora=millis();
  WiFiClient cliente = concursado.available(); //Verifica se algum cliente solicitou algo, o valor retornado trata-se de um bool
    //Serial.println(cliente); Tão rápido que nem dá pra ver
    if (cliente){
        comando=cliente.readStringUntil('\r');
        Serial.println(comando);
          if (comando.indexOf("/iluext") != -1) {
            stsExt=!stsExt;
            digitalWrite(pinExt,stsExt);
            Serial.println("Externo:"+String(stsExt));
              if (stsExt){
                site.replace("Externo: apagado","Externo: ligado");
                site.replace("ext\" style=\"background-color: #252525","ext\" style=\"background-color: #21ff4e");
                site.replace("ext:0","ext:1");
              }else{
                site.replace("Externo: ligado","Externo: apagado");
                site.replace("ext\" style=\"background-color: #21ff4e","ext\" style=\"background-color: #252525");
                site.replace("ext:1","ext:0");
              }
                cliente.print(site);
                cliente.stop();
          } else {
              if (comando.indexOf("/ilucoz") != -1) {
                stsCoz=!stsCoz;
                digitalWrite(pinCoz,stsCoz);
                Serial.println("Cozinha:"+String(stsCoz));
                  if (stsCoz){
                    site.replace("Cozinha: apagado","Cozinha: ligado");
                    site.replace("coz\" style=\"background-color: #252525","coz\" style=\"background-color: #21ff4e");
                    site.replace("coz:0","coz:1");
                  }else{
                    site.replace("Cozinha: ligado","Cozinha: apagado");
                    site.replace("coz\" style=\"background-color: #21ff4e","coz\" style=\"background-color: #252525");
                    site.replace("coz:1","coz:0");
                  }
                    cliente.print(site);
                    cliente.stop();
              }else{
                  if (comando.indexOf("/iluqua") != -1) {
                    stsQua=!stsQua;
                    digitalWrite(pinQua,stsQua);
                    Serial.println("Quarto:"+String(stsQua));
                      if (stsQua){
                        site.replace("Quarto: apagado","Quarto: ligado");
                        site.replace("qua\" style=\"background-color: #252525","qua\" style=\"background-color: #21ff4e");
                        site.replace("qua:0","qua:1");
                      }else{
                        site.replace("Quarto: ligado","Quarto: apagado");
                        site.replace("qua\" style=\"background-color: #21ff4e","qua\" style=\"background-color: #252525");
                        site.replace("qua:1","qua:0");
                      }
                      cliente.print(site);
                      cliente.stop();
                        } else {
                          if (comando.indexOf("/atvbom") != -1) {
                            bombPlant=true;
                            umaVezBom=true;
                              Serial.println("Bomba Plantada");
                                site.replace("Bomba: desligado","Bomba: Ligado");
                                site.replace("bom\" style=\"background-color: #252525","bom\" style=\"background-color: #21ff4e");
                                site.replace("bom:0","bom:1");
                                  cliente.print(site);
                                  cliente.stop();
                          } else {
                            cliente.print(site);
                            cliente.stop();
                          }
                        }
              }
          
          }
    }

    if (bombPlant){
      digitalWrite(pinBom,1);
      
      if (umaVezBom){
        capturaTemp=minDia;
        umaVezBom=false;
      }
      //Temporizador
      if (minDia>capturaTemp+20){
          digitalWrite(pinBom,0);
            bombPlant=false;
            Serial.println("Bomba Desarmada!");
              site.replace("Bomba: Ligado","Bomba: desligado");
              site.replace("bom\" style=\"background-color: #21ff4e","bom\" style=\"background-color: #252525");
              site.replace("bom:1","bom:0");
                cliente.print(site);
                cliente.stop();
      }
    }

      if (agora-antes>perData || direto){ //O direto permite ele passar uma vez apenas diretamente.
        timeClient.update();
          hora=timeClient.getHours();
          minuto=timeClient.getMinutes();
            Serial.println(String(hora)+":"+String(minuto)+"h - Atualizado da Internet");
      antes=agora;
      direto=false;
      } else{
        if (agora-antesOf>=60000){
          minuto++;
            if (minuto>=60){
              minuto=0;
              hora++;
                if (hora>=24){
                  hora=0;
                }
            }
            antesOf=agora;
            Serial.println(String(hora)+":"+String(minuto)+"h - offline");

              
        }
      }

      //VIGIA QUE FARÁ OS ACIONAMENTOS
            if (agora-antesVi>perVig){
              minDia=hora*60+minuto;
                //SE ENTRE 18:00h e 5:45h LIGAR LAMPADAS EXTERNAS
                  if (minDia>=1080 || minDia <=345){
                    if (!stsExt){
                      stsExt=true;
                      digitalWrite(pinExt,stsExt);
                        Serial.println("Vigia Ativado -> Externo:"+String(stsExt));
                          site.replace("Externo: apagado","Externo: ligado");
                          site.replace("ext\" style=\"background-color: #252525","ext\" style=\"background-color: #21ff4e");
                          site.replace("ext:0","ext:1");
                            cliente.print(site);
                              cliente.stop();
                    }
                  } else{
                    if (stsExt){
                      stsExt=false;
                      digitalWrite(pinExt,stsExt);
                        Serial.println("Vigia Ativado -> Externo:"+String(stsExt));
                          site.replace("Externo: ligado","Externo: apagado");
                            site.replace("ext\" style=\"background-color: #21ff4e","ext\" style=\"background-color: #252525");
                          site.replace("ext:1","ext:0");
                            cliente.stop();
                            cliente.print(site);
                              cliente.stop();
                    }
                  }

                  //Bomba de ÁGUA
                  if (minDia>=600 || minDia<=620){
                    digitalWrite(pinBom,1);
                    site.replace("Bomba: desligado","Bomba: Ligado");
                    site.replace("bom\" style=\"background-color: #252525","bom\" style=\"background-color: #21ff4e");
                    site.replace("bom:0","bom:1");
                      cliente.print(site);
                      cliente.stop();
                  }else{
                    digitalWrite(pinBom,0);
                    site.replace("Bomba: Ligado","Bomba: desligado");
                    site.replace("bom\" style=\"background-color: #21ff4e","bom\" style=\"background-color: #252525");
                    site.replace("bom:1","bom:0");
                      cliente.print(site);
                      cliente.stop();
                  }
              antesVi=agora;
              }
//delay(500);
}
