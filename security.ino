/*
 Projeto de Automação Residencial -> Bruno Rezende
 Testes de Alarme Simples
 */

// include the library code:
#include <ShiftedLCD.h>
#include <SPI.h>

//sonar
 #include <NewPing.h>

 
// include the Servo library
#include <Servo.h>


Servo myServo;  // create a servo object

int lvermelho = 3;
int lroxo = 5;
int laser3 = 12;

int buzzer = 10;

int botao1 = A0;
int botao2 = A1;
int botao3 = A2;
int botao4 = A3;

  
#define TRIGGER_PIN  4    /* Arduino pino trigger (envia onda)*/
 #define ECHO_PIN     7    /* Arduino pino echo (recebe onda) */
 #define MAX_distancia 100  /* Distancia máxima do ping em centimetro. */ 


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_distancia); /* NewPing configuração de pinos e distancia máxima. */
 
 int distancia;

unsigned long millisTarefa1 = millis();
unsigned long millisTarefa2 = millis();
unsigned long millisTarefa3 = millis();
unsigned long timerbotao = millis();

int alarmeativo = 0;
int alarmedisparado = 0;

  int gambiarra = 0;

// initialize the library with the number of the sspin 
// (or the latch pin of the 74HC595)
LiquidCrystal lcd(9);

void setup() {
  Serial.begin(9600);
    myServo.attach(6); // attaches the servo on pin 9 to the servo object

  pinMode(lvermelho,OUTPUT);
  pinMode(lroxo,OUTPUT);
  pinMode(laser3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(botao1,INPUT);
  pinMode(botao2,INPUT);
  pinMode(botao3,INPUT);
  pinMode(botao4,INPUT);

  digitalWrite(botao1, HIGH);
  digitalWrite(botao2, HIGH);
  digitalWrite(botao3, HIGH);
  digitalWrite(botao4, HIGH);
  digitalWrite(buzzer, LOW);

  millisTarefa2 = millis();
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("BRTec Security");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  // print the number of seconds since reset:

  // set the servo position
  

  if(alarmedisparado == 1){
     if((millis() - millisTarefa3) > 50){
        
        digitalWrite(buzzer, !digitalRead(buzzer));
        millisTarefa3 = millis();
      }
  }
  
  
  if(alarmeativo == 1){
      digitalWrite(laser3, HIGH);
      digitalWrite(lvermelho, HIGH);
      digitalWrite(lroxo, HIGH);

      int sensor_sonar = calcularPING();
      if(sensor_sonar >= 10 && sensor_sonar <= 15){
        ativa_alarme();
      }
      
      
      if((millis() - millisTarefa2) > 1000){
        if(gambiarra == 0){
        myServo.write(180);
        gambiarra = 1;
        }else{
          myServo.write(-180);
          gambiarra = 0;
        }
        
        millisTarefa2 = millis();
      }
  }else{
    digitalWrite(laser3, LOW);
      digitalWrite(lvermelho, LOW);
      digitalWrite(lroxo, LOW);
  }



  
  
  
  if((millis() - millisTarefa1) > 2000){
    limpadisplay();
  }
  
  if (digitalRead(botao1) == LOW)
  {
       if((millis() - timerbotao) > 1000){
      botao1on();
      tom_botao();
      millisTarefa1 = millis();
      timerbotao = millis();
   }
   }

  if (digitalRead(botao2) == LOW)
  {
     if((millis() - timerbotao) > 1000){

    digitalWrite(laser3, LOW);
      botao2on();
    millisTarefa1 = millis();
    timerbotao = millis();
   }
  }

  if (digitalRead(botao3) == LOW)
  {
     if((millis() - timerbotao) > 1000){
      botao3on();
    millisTarefa1 = millis();
    timerbotao = millis();
   }
  }

  if (digitalRead(botao4) == LOW){
         if((millis() - timerbotao) > 1000){

    botao4on();
    millisTarefa1 = millis();
    timerbotao = millis();
   }
  }

  
}

void tom_botao(){
  digitalWrite(buzzer, HIGH);
  delay(50);
  digitalWrite(buzzer, LOW);
}

void ativa_alarme(){
  alarmedisparado = 1;
  
}

void limpadisplay(){
  lcd.setCursor(1, 1);
  lcd.print("                ");
  
}

void botao1on() {
  lcd.setCursor(1, 1);
  lcd.print("Alarme Ativado");
  alarmeativo = 1;
  Serial.println("alarmeativado");
}

void botao2on(){
  
  lcd.setCursor(1, 1);
  lcd.print("Alarme Desativado");
  alarmeativo = 0;
  Serial.println("alarmedesativado");
}

void botao3on(){
  
  lcd.setCursor(1, 1);
  lcd.print("Lasers Ativos");
    digitalWrite(lroxo, HIGH);
    digitalWrite(lvermelho, HIGH);
  Serial.println(3);
}

void botao4on(){
  
  lcd.setCursor(1, 1);
  lcd.print("Alarme Desativado");
  alarmedisparado = 0;
    digitalWrite(lroxo, LOW);
    digitalWrite(lvermelho, LOW);
  Serial.println(4);
}

int calcularPING(){ 
  unsigned int uS = sonar.ping();  /* Envia ping, retorna ping tempo em microseconds (uS). */
  distancia = uS / US_ROUNDTRIP_CM; /* Converte oing tempo para distancia em cm e retorana resultado. */
  return distancia;
}
