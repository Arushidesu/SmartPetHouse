#include <Wire.h>          //Biblioteca de hora do sistema
#include <LiquidCrystal.h> //Carrega a biblioteca LiquidCrystal
#include "RTClib.h"        //Carrega a biblioteca do RTC
#include <Servo.h>         //Carrega a biblioteca do servo

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); //Define os pinos que serão utilizados para ligação ao display
Servo servo;                           //Cria um servo de nome servo

bool active = false;     //Condição do botão de troca
const int botao = 52;    //Botão para troca de horários (aumento)
const int botao1 = 50;   //Botão para troca de horários (decremento)
const int botao2 = 48;   //Botão para aumentar hora
const int botao22 = 46;  //Botão para diminuir hora
const int botao3 = 44;   //Botão para aumentar minuto
const int botao33 = 42;  //Botão para diminuir minuto
int one[] = {0, 0};      //Array para armezenar horas e minutos da "Hora 1"
int two[] = {0, 0};      //Array para armezenar horas e minutos da "Hora 2"
int three[] = {0, 0};    //Array para armezenar horas e minutos da "Hora 3"
int contador = 0;        //Variável de contagem dos botões
int estado = 0;          //Leitura do estado do botão de aumento dos horários
int estado1 = 0;         //Leitura do estado do botão de decremento dos horários


RTC_DS1307 rtc;    // Cria um RTC_DS1307 chamado rtc

//Array com os dias da semana
char daysOfTheWeek[7][14] = {"Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sabado"};

//Adicionar um 0 nos horários
String zero(int n){ //função que verifica se o numero é menor
                    //que dez, e se for adiciona o 0 antes
  if(n < 10){       //Exemplo: entra 5, sai 05-> 00:05:00
    return "0" + String(n);       //como seria-> 00:5:00
  }else{
    return String(n);
  }
}

void printTime(){
  DateTime now = rtc.now();  //Variável now do tipo DateTime e define ela como a hora do RTC

  //Imprime as informações: dia da semana, dia/mês/ano - hh:mm:ss
  Serial.print(String(daysOfTheWeek[now.dayOfTheWeek()]) + ", " + zero(now.day()) + "/" + zero(now.month()) + "/" + now.year());
  Serial.print(" - ");
  Serial.println(zero(now.hour()) + ":" + zero(now.minute()) + ":" + zero(now.second()));
}

void setup() {
  lcd.begin(16, 2); //Define o número de colunas e linhas do LCD

  servo.attach(10); //Pino digital do servo
  
  //Seta os botões como entrada
  pinMode(botao, INPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao22, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao33, INPUT); 

  Serial.begin(9600); //Inicia o monitor serial

  //Se !(not) iniciar o RTC imprime a mensagem
  if(!rtc.begin()){  
    Serial.println("RTC não encontrado.");
    while(1);
  }
  
  //rtc.adjust é a função que envia a data e hora para o RTC:
  //rtc.adjust(DateTime(2017, 10,  18, 14, 58, 15)); //descomente essa linha para alterar a hora do RTC!

  //Condição caso o RTC não esteja funcionando
  if(!rtc.isrunning()){
    Serial.println("RTC nao esta funcionando!");  
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Data e hora da compilação do sketch
  }
}

void loop() {
  
  printTime(); //Executa a função para imprimir no serial monitor
  DateTime now = rtc.now();  //Variável now do tipo DateTime e define ela como a hora do RTC

  //Leitura dos botões nas variáveis
  estado = digitalRead(botao);
  estado1 = digitalRead(botao1); 

  //Condição de troca dos horários (aumento)
  if(estado == 1)
    contador += 1;
    else if(contador >= 3)
      contador = 0;

  //Condição de troca dos horários (decremento)
  if(estado1 == 1) contador -= 1;
    else if(contador <= -1) contador = 2;

  //Caso a troca dos horários ocorra
  switch(contador) {

    //Primeiro caso (Hora 1)
    case 0:
      lcd.clear(); //Limpa a tela
      lcd.setCursor(5, 0); //Posiciona o cursor na coluna 5, linha 0
      lcd.print("HORA 1"); //Envia o texto entre aspas para o LCD
      lcd.setCursor(5, 1); //Posiciona o cursor na coluna 5, linha 1

      //Condição para aumentar as horas
      if(digitalRead(botao2) == 1) {
        one[0] += 1;
        if(one[0] > 23) {
          one[0] = 0;
        }  
      }

      //Condição para diminuir as horas
      if(digitalRead(botao22) == 1) {
        one[0] -= 1;
        if(one[0] < 0) {
          one[0] = 23;
        }
      }

      //Condição para aumentar os minutos
      if(digitalRead(botao3) == 1) {
        one[1] += 1;
        if(one[1] > 59) {
          one[1] = 0;
        }
      }

      //Condição para diminuir os minutos
      if(digitalRead(botao33) == 1) {
        one[1] -= 1;
        if(one[1] < 0) {
          one[1] = 59;
        }
      }

      //Escreve no LCD os valores  
      lcd.print(one[0]);
      lcd.print(":");
      lcd.print(one[1]);
      delay(100);
      break;

    //Segundo caso (Hora 2)
    case 1:  
      lcd.clear(); //Limpa a tela
      lcd.setCursor(5, 0); //Posiciona o cursor na coluna 5, linha 0
      lcd.print("HORA 2"); //Envia o texto entre aspas para o LCD
      lcd.setCursor(5, 1); //Posiciona o cursor na coluna 5, linha 1

      //Condição para aumentar as horas
      if(digitalRead(botao2) == 1) {
        two[0] += 1;
        if(two[0] > 23) {
          two[0] = 0;
        }  
      }

      //Condição para diminuir as horas
      if(digitalRead(botao22) == 1) {
        two[0] -= 1;
        if(two[0] < 0) {
          two[0] = 23;
        }
      }

      //Condição para aumentar os minutos
      if(digitalRead(botao3) == 1) {
        two[1] += 1;
        if(two[1] > 59) {
          two[1] = 0;
        }
      } 

      //Condição para diminuir os minutos
      if(digitalRead(botao33) == 1) {
        two[1] -= 1;
        if(two[1] < 0) {
          two[1] = 59;
        }
      }

      //Escreve no LCD os valores
      lcd.print(two[0]);
      lcd.print(":");
      lcd.print(two[1]);
      delay(100);
      break;

    //Terceiro caso (Hora 3)
    case 2:
      lcd.clear(); //Limpa a tela
      lcd.setCursor(5, 0); //Posiciona o cursor na coluna 5, linha 0
      lcd.print("HORA 3"); //Envia o texto entre aspas para o LCD
      lcd.setCursor(5, 1); //Posiciona o cursor na coluna 5, linha 1

      //Condição para aumentar as horas
      if(digitalRead(botao2) == 1) {
        three[0] += 1;
        if(three[0] > 23) {
          three[0] = 0;
        }  
      }

      //Condição para diminuir as horas
      if(digitalRead(botao22) == 1) {
        three[0] -= 1;
        if(three[0] < 0) {
          three[0] = 23;
        }
      }

      //Condição para aumentar os minutos
      if(digitalRead(botao3) == 1) {
        three[1] += 1;
        if(three[1] > 59) {
          three[1] = 0;
        }
      } 

      //Condição para diminuir os minutos
      if(digitalRead(botao33) == 1) {
        three[1] -= 1;
        if(three[1] < 0) {
          three[1] = 59;
        }
      }

      //Escreve no LCD os valores
      lcd.print(three[0]);
      lcd.print(":");
      lcd.print(three[1]);
      delay(100);
      break;        
 }

 //Condição teste para verificar o LCD + RTC
 if(now.hour() == one[0] and now.minute() == one[1]) {
  servo.write(180);
  servo.write(-180);
 }
}
