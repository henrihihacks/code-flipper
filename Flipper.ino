#include <U8g2lib.h>
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#include <FastLED.h>
#include "Defines.h"
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED

void ManageObject(ObjectTouch_t * Obj, uint16_t * Score, CRGB * LEDArray, uint8_t Index);
void ActuateRelay(uint8_t index, uint16_t to_delay_ms);
void ActuatorManager(void);

uint16_t points = 0; //nombre de points
uint8_t balles = 10; //nombre de balles
uint8_t RefreshScreen = 0;

bool retour = 1; //pour savoir si la boule est bien rentrée à la maison

CRGB leds[NUM_LEDS];
ObjectTouch_t ObjectHandle[7];
Actuator_t ActuatorsHandle[6];


void setup() {
    /* =========================== */
    /*          SERIAL INIT        */
    /* =========================== */
    u8g2.begin();
    Serial.begin(9600);
    
    /* =========================== */
    /*         OBJECT INIT         */
    /* =========================== */
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    for (uint8_t i = 0; i < 7; i++) {
        leds[i] = CRGB(0, 0, 255);
        ObjectHandle[i].Counter = 0;
        ObjectHandle[i].Points[0] = 5;
        ObjectHandle[i].Points[1] = 10;
        ObjectHandle[i].Points[2] = 20;
    }
    ActuatorsHandle[0] = (Actuator_t) {PIN_RELshot1,      0, 0};
    ActuatorsHandle[1] = (Actuator_t) {PIN_RELshot2,      0, 0};
    ActuatorsHandle[2] = (Actuator_t) {PIN_RELchamp1,     0, 0};
    ActuatorsHandle[3] = (Actuator_t) {PIN_RELchamp2,     0, 0};
    ActuatorsHandle[4] = (Actuator_t) {PIN_RELchamp3,     0, 0};
    ActuatorsHandle[5] = (Actuator_t) {PIN_RELlanceur,    0, 0};
    FastLED.show();
    
    /* =========================== */
    /*           PIN INIT          */
    /* =========================== */
    pinMode(PIN_piezo0, INPUT_PULLUP);
    pinMode(PIN_piezo1, INPUT_PULLUP);
    pinMode(PIN_piezo2, INPUT_PULLUP);
    pinMode(PIN_piezo3, INPUT_PULLUP);
    pinMode(PIN_IR1, INPUT);
    pinMode(PIN_IR2, INPUT);
    pinMode(PIN_IR3, INPUT);
    pinMode(PIN_IR4, INPUT);
    pinMode(PIN_IR5, INPUT);
    pinMode(PIN_shot1, INPUT_PULLUP);
    pinMode(PIN_shot2, INPUT_PULLUP);
    pinMode(PIN_champ2, INPUT_PULLUP);
    pinMode(PIN_champ1, INPUT_PULLUP);
    pinMode(PIN_champ3, INPUT_PULLUP);
    pinMode(PIN_RELshot1, OUTPUT);
    pinMode(PIN_RELshot2, OUTPUT);
    pinMode(PIN_RELchamp1, OUTPUT);
    pinMode(PIN_RELchamp2, OUTPUT);
    pinMode(PIN_RELchamp3, OUTPUT);
    pinMode(PIN_RELlanceur, OUTPUT);
    pinMode(PIN_lanceur, INPUT_PULLUP);
    pinMode(PIN_boutonpartie, INPUT_PULLUP);
    
    points = 0;
    u8g2.clearBuffer(); // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB24_tr); // choose a suitable font
    u8g2.setCursor(5, 30);
    u8g2.print(points); // write something to the internal memory
    u8g2.sendBuffer();

}

void loop() {
    /* ======================================== */
    /*              MANAGE PIEZZOS              */
    /* ======================================== */
    if (digitalRead(PIN_piezo0) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_PIEZZO_0);
    }

    if (digitalRead(PIN_piezo1) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_PIEZZO_1);
    }

    if (digitalRead(PIN_piezo2) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_PIEZZO_2);
    }

    if (digitalRead(PIN_piezo3) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_PIEZZO_3);
    }
    
    /* ======================================== */
    /*              MANAGE IR GATES             */
    /* ======================================== */
    if (digitalRead(PIN_IR1) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_IR1);
    }

    if (digitalRead(PIN_IR2) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_IR2);
    }
    
    if (digitalRead(PIN_IR3) == LOW) {
        ManageObject(ObjectHandle, &points, leds, ID_IR3);
    }
    
    /* ======================================== */
    /*               MANAGE RELAYS              */
    /* ======================================== */

    if (digitalRead(PIN_shot1) == LOW) {//activer le premier slingshot
        ActuateRelay(ID_SHOT_1, 100);
    }

    if (digitalRead(PIN_shot2) == LOW) {
        ActuateRelay(ID_SHOT_2, 100);
    }

    if (digitalRead(PIN_champ1) == LOW) {//activer le premier champignon (bumper)
        ActuateRelay(ID_CHAMP_1, 100);
        Serial.print("champ1");
    }

    if (digitalRead(PIN_champ2) == LOW) {
        ActuateRelay(ID_CHAMP_2, 100);
        Serial.print("champ2");
    }

    if (digitalRead(PIN_champ3) == LOW) {
        ActuateRelay(ID_CHAMP_3, 100);
        Serial.print("champ3");
    }

    if (((digitalRead(PIN_lanceur) == LOW )&& (balles > 0)) && (retour == 1)) {//Lancer une balle
        ActuateRelay(ID_LANCEUR, 500);
        balles--;
        retour = 0;
    }

    if (digitalRead(PIN_boutonpartie) == LOW) {//Recommencer une partie
        points = 0;
        balles = 10;
        retour = 1;
        RefreshScreen |= 1;
    }

    if (digitalRead(PIN_IR4) == LOW) {//Lancer une balle de plus
        retour = 1;
    }
    if (digitalRead(PIN_IR5) == LOW) {
        ActuateRelay(ID_LANCEUR, 500);
    }
    
    /* Real time actuation managing */
    ActuatorManager();
    
    /* ======================================== */
    /*                SEND DATA                 */
    /* ======================================== */
    if ((millis() % 16) == 0) { /* Refresh Rate: 16ms / 62.5 FPS */
        FastLED.show();
    }
    
    if (RefreshScreen != 0) { /* Refresh screen on request only */
        u8g2.clearBuffer(); // clear the internal memory
        u8g2.setCursor(5, 30);
        u8g2.print(points); // write something to the internal memory
        u8g2.sendBuffer();
        RefreshScreen = 0;
    }
    
}

void ManageObject(ObjectTouch_t Obj[], uint16_t * Score, CRGB LEDArray[], uint8_t Index) {
    
    *Score += Obj[Index].Points[Obj[Index].Counter];
    Obj[Index].Counter++;
    
    switch(Obj[Index].Counter)
    {
        case 1:
            LEDArray[Index] = CRGB(0, 0xFF, 0);
            break;
        case 2:
            LEDArray[Index] = CRGB(0xFF, 0, 0);
            break;
        case 3:
            LEDArray[Index] = CRGB(0, 0, 0xFF);
            Obj[Index].Counter = 0;
            break;
        default:
            break;
    }
    RefreshScreen |= 1;
}

void ActuateRelay(uint8_t index, uint16_t to_delay_ms) {
    ActuatorsHandle[index].Status = 1;
    digitalWrite(ActuatorsHandle[index].PinNumber, HIGH);
    ActuatorsHandle[index].TimeOut = millis() + to_delay_ms;
}

void ActuatorManager(void) {
    uint8_t index = 0;
    
    for (index = 0; index < 6; index++) {
        if ((ActuatorsHandle[index].Status == 1) && (millis() >= ActuatorsHandle[index].TimeOut)) {
            digitalWrite(ActuatorsHandle[index].PinNumber, LOW);
            ActuatorsHandle[index].Status = 0;
        }
    }
}
