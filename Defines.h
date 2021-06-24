#ifndef DEFINES_H
#define DEFINES_H

#define PIN_piezo0          35
#define PIN_piezo1          29          
#define PIN_piezo2          33
#define PIN_piezo3          31
#define PIN_IR1             49
#define PIN_IR2             47
#define PIN_IR3             45
#define PIN_IR4             43
#define PIN_IR5             53
#define PIN_shot1           38
#define PIN_shot2           30
#define PIN_champ1          36
#define PIN_champ2          34
#define PIN_champ3          32
#define PIN_RELshot1        44
#define PIN_RELshot2        46
#define PIN_RELchamp1       50
#define PIN_RELchamp2       52
#define PIN_RELchamp3       48
#define PIN_RELlanceur      42
#define PIN_lanceur         40
#define PIN_boutonpartie    28

#define ID_PIEZZO_0         0
#define ID_PIEZZO_1         1
#define ID_PIEZZO_2         2
#define ID_PIEZZO_3         3
#define ID_IR1              6
#define ID_IR2              5
#define ID_IR3              4

#define ID_SHOT_1           0
#define ID_SHOT_2           1
#define ID_CHAMP_1          2
#define ID_CHAMP_2          3
#define ID_CHAMP_3          4
#define ID_LANCEUR          5
#define LED_PIN             A0
#define NUM_LEDS            7

typedef struct {
    uint8_t Counter;
    uint8_t Points[3];
} ObjectTouch_t;

typedef struct {
    uint8_t PinNumber;
    uint8_t Status;
    uint32_t TimeOut;
} Actuator_t;



#endif
