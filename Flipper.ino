#include <U8g2lib.h>
#include <Arduino.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

#ifdef U8X8_HAVE_HW_I2C                     //Les pins supportants l'interruption sont 2,3,18,19,20,21 et les pins 20 et 21 sont ceux de l'I2C donc si on les utilises en interruption ils ne sont pas utilisables en I2C
#include <Wire.h>                           //donc afin d'avoir les champi et les lanceurs le plus reactif possible, vu qu'ils sont 5 j'ai du mettre que 2 champi sur 3 avec des interruptions pour garder l'écran
#endif                                      // la doc arduino sur le sujet : https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

#include <FastLED.h>

#define LED_PIN     A0
#define NUM_LEDS    7

const int piezo0 = 35; //pins des différents composants
const int piezo1 = 29;                
const int piezo2 = 33;  
const int piezo3 = 31;  

const int IR1 = 49;
const int IR2 = 47;
const int IR3 = 45;
const int IR4 = 43;
const int IR5 = 53;

const int shot1 = 2;
const int shot2 = 3;

const int champ1 = 18;
const int champ2 = 19;
const int champ3 = 32;

const int RELshot1 = 44;
const int RELshot2 = 46;
const int RELchamp1 = 50;
const int RELchamp2 = 52;
const int RELchamp3 = 48;
const int RELlanceur = 42;

const int lanceur = 40;
const int boutonpartie = 28;

int points = 0; //nombre de points
int balles = 10; //nombre de balles
int passagepiezzo0 = 0; //nombre de passages par piezzo
int passagepiezzo1 = 0;
int passagepiezzo2 = 0;
int passagepiezzo3 = 0;
int passageIR1 = 0; //nombre de passages par capteur IR
int passageIR2 = 0;
int passageIR3 = 0;
bool retour = 1; //pour savoir si la boule est bien rentrée à la maison

CRGB leds[NUM_LEDS];

void setup() {
  u8g2.begin();
  Serial.begin(9600);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

  leds[0] = CRGB(0, 0, 255);
  leds[1] = CRGB(0, 0, 255);
  leds[2] = CRGB(0, 0, 255);
  leds[3] = CRGB(0, 0, 255);
  leds[4] = CRGB(0, 0, 255);
  leds[5] = CRGB(0, 0, 255);
  leds[6] = CRGB(0, 0, 255);
  FastLED.show();
  
  pinMode(piezo0, INPUT_PULLUP);
  pinMode(piezo1, INPUT_PULLUP);
  pinMode(piezo2, INPUT_PULLUP);
  pinMode(piezo3, INPUT_PULLUP);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);
  pinMode(IR5, INPUT);
  attachInterrupt(digitalPinToInterrupt(shot1),sling1,FALLING);
  attachInterrupt(digitalPinToInterrupt(shot2),sling2,FALLING);
  attachInterrupt(digitalPinToInterrupt(champ1),champi1,FALLING);
  attachInterrupt(digitalPinToInterrupt(champ2),champi2,FALLING);
  pinMode(champ3, INPUT_PULLUP);
  pinMode(RELshot1, OUTPUT);
  pinMode(RELshot2, OUTPUT);
  pinMode(RELchamp1, OUTPUT);
  pinMode(RELchamp2, OUTPUT);
  pinMode(RELchamp3, OUTPUT);
  pinMode(RELlanceur, OUTPUT);
  pinMode(lanceur, INPUT_PULLUP);
  pinMode(boutonpartie, INPUT_PULLUP);
  
  u8g2.clearBuffer();          // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB24_tr); // choose a suitable font
  u8g2.setCursor(5, 30);
  u8g2.print(points); // write something to the internal memory
  u8g2.sendBuffer();

}

void sling1(){        //activer le premier slingshot
  digitalWrite(RELshot1, HIGH);
  for(int i=100;i>=0;i--){
    delay(1);                   //normalement dans une fonction avec des interruptions il ne faut pas mettre de délais, donc j'ai mis une boucle de petit delais pour reduire l'impact par rapport a un gros délai.
  }
  digitalWrite(RELshot1, LOW);
  
}

void sling2(){
  digitalWrite(RELshot2, HIGH);
  for(int i=100;i>=0;i--){
    delay(1);
  }
  digitalWrite(RELshot2, LOW);
}

void champi1(){//activer le premier champignon (bumper)
  digitalWrite(RELchamp1, HIGH);
  for(int i=100;i>=0;i--){
    delay(1);
  }
  digitalWrite(RELchamp1, LOW);
  Serial.print("champ1");
}

void champi2(){
  digitalWrite(RELchamp2, HIGH);
  for(int i=100;i>=0;i--){
    delay(1);
  }
  digitalWrite(RELchamp2, LOW);
  Serial.print("champ2");
}


void loop() {

  if(digitalRead(piezo0)==LOW){ //Marquer des points avec les cibles
    if(passagepiezzo0==0){
      points=points+5;
      leds[0] = CRGB(0, 255, 0);
    }
    if(passagepiezzo0==1){
      points=points+10;
      leds[0] = CRGB(255, 0, 0);
    }
    if(passagepiezzo0==2){
      points=points+20;
      leds[0] = CRGB(0, 0, 0);
    }
    
    passagepiezzo0++;
    if(passagepiezzo0==3){
      passagepiezzo0=0;
      leds[0] = CRGB(0, 0, 255);
    }
    
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }

  if(digitalRead(piezo1)==LOW){
    if(passagepiezzo1==0){
      points=points+5;
      leds[1] = CRGB(0, 255, 0);
    }
    if(passagepiezzo1==1){
      points=points+10;
      leds[1] = CRGB(255, 0, 0);
    }
    if(passagepiezzo1==2){
      points=points+20;
      leds[1] = CRGB(0, 0, 0);
    }
    
    passagepiezzo1++;
    if(passagepiezzo1==3){
      passagepiezzo1=0;leds[1] = CRGB(0, 0, 255);
    }
    
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }

  if(digitalRead(piezo2)==LOW){
    if(passagepiezzo2==0){
      points=points+5;
      leds[2] = CRGB(0, 255, 0);
    }
    if(passagepiezzo2==1){
      points=points+10;
      leds[2] = CRGB(255, 0, 0);
    }
    if(passagepiezzo2==2){
      points=points+20;
      leds[2] = CRGB(0, 0, 0);
    }
    
    passagepiezzo2++;
    if(passagepiezzo2==3){
      passagepiezzo2=0;
      leds[2] = CRGB(0, 0, 255);
    }
    
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }
  if(digitalRead(piezo3)==LOW){
    if(passagepiezzo3==0){
      points=points+5;
      leds[3] = CRGB(0, 255, 0);
    }
    if(passagepiezzo3==1){
      points=points+10;
      leds[3] = CRGB(255, 0, 0);
    }
    if(passagepiezzo3==2){
      points=points+20;
      leds[3] = CRGB(0, 0, 0);
    }
    
    passagepiezzo3++;
    if(passagepiezzo3==3){
      passagepiezzo3=0;
      leds[3] = CRGB(0, 0, 255);
    }
    
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }

     
  if(digitalRead(IR1)==LOW){ //Marquer des points avec les capteurs IR entre les portes
    if(passageIR1==0){
      points=points+5;
      leds[6] = CRGB(0, 255, 0);
    }
    if(passageIR1==1){
      points=points+10;
      leds[6] = CRGB(255, 0, 0);
    }
    if(passageIR1==2){
      points=points+20;
      leds[6] = CRGB(0, 0, 0);
    }
    passageIR1++;
    if(passageIR1==3){
      passageIR1=0;
      leds[6] = CRGB(0, 0, 255);
    }
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }
  
  if(digitalRead(IR2)==LOW){
    if(passageIR2==0){
      points=points+5;
      leds[5] = CRGB(0, 255, 0);
    }
    if(passageIR2==1){
      points=points+10;
      leds[5] = CRGB(255, 0, 0);
    }
    if(passageIR2==2){
      points=points+20;
      leds[5] = CRGB(0, 0, 0);
    }
    passageIR2++;
    if(passageIR2==3){passageIR2=0;leds[5] = CRGB(0, 0, 255);}
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }
  if(digitalRead(IR3)==LOW){
    if(passageIR3==0){
      points=points+5;
      leds[4] = CRGB(0, 255, 0);
    }
    if(passageIR3==1){
      points=points+10;
      leds[4] = CRGB(255, 0, 0);
    }
    if(passageIR3==2){
      points=points+20;
      leds[4] = CRGB(0, 0, 0);
    }
    
    passageIR3++;
    if(passageIR3==3){
      passageIR3=0;
      leds[4] = CRGB(0, 0, 255);
    }
    
    FastLED.show();
    u8g2.clearBuffer();          // clear the internal memory
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();
  }

  if (digitalRead(champ3)==LOW){
    digitalWrite(RELchamp3, HIGH);
    delay(100);
    digitalWrite(RELchamp3, LOW);
    delay(80);
    Serial.print("champ3");
  }

  if (((digitalRead(lanceur)==LOW)&&(balles >0))&&(retour==1)){//Lancer une balle
    digitalWrite(RELlanceur, HIGH);
    delay(500);
    digitalWrite(RELlanceur, LOW);
    balles = balles-1;
    retour = 0;
  }
                        
  if (digitalRead(boutonpartie) == LOW){//Recommencer une partie
    points = 0;
    balles = 10;
    retour = 1;
  }
             
  if (digitalRead(IR4)==LOW){//Lancer une balle de plus
    retour=1;
  }
  
  if(digitalRead(IR5)==LOW){
    digitalWrite(RELlanceur, HIGH);
    delay(500);
    digitalWrite(RELlanceur, LOW);
  }
}
