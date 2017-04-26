#include "xtea.c.h"

#define LED 13
int i;
void setup () {
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
}
 
uint32_t key[] = { 1, 2, 3, 4 };
uint32_t v[]   = { 10, 20 , 30, 40, 50, 60, 70, 80, 90, 100, 110, 120 , 130, 140, 150, 160, 170, 180, 190, 200, 210, 220 , 230, 240, 250, 260, 270, 280, 290, 300, 310, 320 , 330, 340, 350, 360, 370, 380, 390, 400, 410, 420 , 430, 440, 450, 460, 470, 480, 490, 500, 510, 520 , 530, 540, 550, 560, 570, 580, 590, 600, 610, 620 , 630, 640, 650, 660, 670, 680, 690, 700, 710, 720 , 730, 740, 750, 760, 770, 780, 790, 800};

void loop () {
    {
        static int led = 0;
        digitalWrite(LED, led=!led);
    }
    unsigned long t1 = micros();

    for(i=0;i<79;i+=2){
      encipher(32, &v[i], key);
    }
    for(i=0;i<79;i+=2){
      decipher(32, &v[i], key);
    }
    
    unsigned long t2 = micros();
    Serial.println(t2-t1);

}
