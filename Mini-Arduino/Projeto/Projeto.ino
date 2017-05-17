#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
#define REL 0
#define AL 1
#define TIM 2
#define CRON 3

byte mode;
byte flag_button_blink;
unsigned long int segundos[4];
unsigned long int index_display;
int seg;
byte rel_pisc;

void write_display()
{
  char disp[] = {'0','0','0','0'};
  if((mode == REL) || (mode ==  AL) || (mode == TIM))
  {
    disp[0] = segundos[mode]/36000 + 48;
    int rest1 = segundos[mode]%36000;
    disp[1] = rest1/3600 + 48;
    int rest2 = rest1%3600;
    disp[2] = rest2/600 + 48;
    int rest3 = rest2%600;
    disp[3] = rest3/60 + 48;
    MFS.write(disp);
  }
}
void blink_display()
{
  switch(index_display)
  {
    case 36000:
      MFS.blinkDisplay(DIGIT_1, ON);
      break;
    case 3600:
      MFS.blinkDisplay(DIGIT_1, OFF);
      MFS.blinkDisplay(DIGIT_2, ON);
      break;
    case 600:
      MFS.blinkDisplay(DIGIT_2, OFF);
      MFS.blinkDisplay(DIGIT_3, ON);
      break;
    case 60:
      MFS.blinkDisplay(DIGIT_3, OFF);
      MFS.blinkDisplay(DIGIT_4, ON);
      break;
  }
}

void button_pressed (byte button)
{
  switch(button)
  {
    case BUTTON_1_SHORT_RELEASE:
        if(!flag_button_blink)
          mode++;
        else
        {
          if (index_display != 3600)
          {
            index_display = index_display/10;
            if(index_display == 6)
            {
              flag_button_blink = !flag_button_blink;
              rel_pisc = 0;
              MFS.blinkDisplay(DIGIT_ALL,OFF);
            }
          }
          else
            index_display = index_display/6;
        }
        break;
    case BUTTON_1_LONG_RELEASE:
        flag_button_blink = !flag_button_blink;
        rel_pisc = 0;
        index_display = 36000;
        MFS.blinkDisplay(DIGIT_ALL,OFF);
        break;
    case BUTTON_2_PRESSED:
        if(flag_button_blink == 1)
        {
          if((mode == REL) || (mode ==  AL) || (mode == TIM))
          {
            segundos[mode] += index_display;
          }
        }
        break;
    case BUTTON_3_PRESSED:
        if(flag_button_blink == 1)
        {
          if(((mode == REL) || (mode ==  AL) || (mode == TIM)) && (index_display < segundos[mode]))
          {
            segundos[mode] -= index_display;
          }
        }
        break;
  }
}

void controle_led()
{
  if(mode == REL)
  {
    MFS.writeLeds(LED_1,ON);
    if(segundos[CRON] == 0)
     MFS.writeLeds(LED_4,OFF);
  }
  else
  {
    MFS.writeLeds(LED_1,OFF);
  }
  if(mode != AL)
  {
    if(segundos[AL]>0)
      MFS.blinkLeds(LED_2,ON);
    else
      MFS.blinkLeds(LED_2,OFF);
  }
  else
    MFS.writeLeds(LED_2,ON);
  if(mode != TIM)
  {
    if(segundos[TIM]>0)
      MFS.blinkLeds(LED_3,ON);
    else
      MFS.blinkLeds(LED_3,OFF);
  }
  else
  {
    MFS.writeLeds(LED_3,ON);
    if(segundos[AL] == 0)
      MFS.writeLeds(LED_2,OFF);
  }
  if(mode != CRON)
  {
    if(segundos[CRON]>0)
      MFS.blinkLeds(LED_4,ON);
    else
      MFS.blinkLeds(LED_4,OFF);
  }
  else
  {
    MFS.writeLeds(LED_4,ON);
    if(segundos[TIM] == 0)
      MFS.writeLeds(LED_3,OFF);
  }
}
void setup() 
{
 Timer1.initialize();
 MFS.initialize(&Timer1);
 mode = REL;
 Serial.begin(9600);
 flag_button_blink = 0;
 for(int i = 0;i<4;i++)
 {
   segundos[i] = 0;
 }
 index_display = 6;
 rel_pisc = 1;
}
void loop() 
{
  delay(10);
  seg++;
  if(seg == 100)
  {
    seg = 0;
    segundos[REL] += 1;
    if(segundos[TIM] > 0)
      segundos[TIM] -= 1;
  }
  byte but = MFS.getButton();
  button_pressed(but);
  if(flag_button_blink)
    blink_display();
  if (mode > CRON)
    mode = REL;
  if((index_display == 6) && (((mode == AL) && (segundos[AL] == 0)) || ((mode == TIM) && (segundos[TIM]==0)) || ((mode == CRON) && (segundos[CRON]==0)) || (rel_pisc == 1)))
    MFS.blinkDisplay(DIGIT_ALL, ON);
  else if(index_display == 6)
    MFS.blinkDisplay(DIGIT_ALL,OFF);
  for(int i = 0;i<4;i++)
  {
    if((segundos[i] >= 86400) && (index_display == 6))
      segundos[i] = segundos[i] - 86400;
  /*  else if (index_display == 60)
    {
      if((segundos[i]%600)>540)
      {
        segundos[i] -= 540;
      }
    }
    else if (index_display == 600)
    {
      if((segundos[i]%3600)>3000)
      {
        segundos[i] -= 3000;
      }
    }
    else if (index_display == 3600)
    {
      if((segundos[i]%36000)>36000)
      {
        segundos[i] -= 36000;
      }
    }
    else if (index_display == 36000)
    {
      if(segundos[i]>72000)
      {
        segundos[i] -= 108000;
      }
    }
//    Serial.println(segundos[REL]);*/
  }
  write_display();
  controle_led();
/*  Serial.print(mode);
  Serial.print("  ");
  Serial.print(rel_pisc);
  Serial.print("  ");
  Serial.println(index_display);*/
}
