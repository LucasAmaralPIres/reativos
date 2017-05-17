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

void write_display()
{
  char disp[] = {'0','0','0','0'};
  if((mode == REL) || (mode ==  AL))
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
              MFS.blinkDisplay(DIGIT_ALL, OFF);
            }
          }
          else
            index_display = index_display/6;
        }
        break;
    case BUTTON_1_LONG_RELEASE:
        flag_button_blink = !flag_button_blink;
        MFS.blinkDisplay(DIGIT_ALL, OFF);
        index_display = 36000;
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

void setup() 
{
 Timer1.initialize();
 MFS.initialize(&Timer1);
 mode = REL;
// Serial.begin(9600);
 flag_button_blink = 0;
 MFS.blinkDisplay(DIGIT_ALL, ON);
 for(int i = 0;i<4;i++)
 {
   segundos[i] = 0;
 }
}
void loop() 
{
  byte but = MFS.getButton();
  Serial.println(segundos[REL]);
  button_pressed(but);
  if(flag_button_blink)
    blink_display();
  if (mode > CRON)
    mode = REL;
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
  if(seg == 100)
  {
    seg = 0;
    segundos[REL] += 1;
  }
  write_display();
  delay(10);
  seg++;
}

