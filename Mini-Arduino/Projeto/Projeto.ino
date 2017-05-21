#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>
#define REL 0
#define AL 1
#define TIM 2
#define CRON 3


// Apagar seg_test na versão final;
byte mode;
byte flag_button_blink;
unsigned long int segundos[4];
unsigned long int index_display;
unsigned long int temp_antes;
int seg, seg_test;
byte rel_pisc, pause, ja_tocou_timer, ja_tocou_alarme;

void write_display()
{
  char disp[] = {'0', '0', '0', '0'};
  unsigned long int rest1, rest2, rest3;
  if ((mode == REL) || (mode ==  AL) || ((mode == TIM) && ((segundos[TIM] >= 3600) || (index_display != 6))))
  {
    disp[0] = segundos[mode] / 36000 + '0';
    rest1 = segundos[mode] % 36000;
    disp[1] = rest1 / 3600 + '0';
    rest2 = rest1 % 3600;
    disp[2] = rest2 / 600 + '0';
    rest3 = rest2 % 600;
    disp[3] = rest3 / 60 + '0';
    MFS.write(disp);
  }
  else if ((mode == TIM) && (segundos[TIM] < 3600) && (index_display == 6))
  {
    disp[0] = segundos[mode] / 600 + '0';
    rest1 = segundos[mode] % 600;
    disp[1] = rest1 / 60 + '0';
    rest2 = rest1 % 60;
    disp[2] = rest2 / 10 + '0';
    disp[3] = rest2 % 10 + '0';
    MFS.write(disp);
  }
}

void write_display_cron()
{
  char disp[] = {'0', '0', '0', '0'};
  unsigned long int rest1, rest2, rest3;
  if (segundos[CRON] < 6000)
  {
    disp[0] = segundos[CRON] / 1000 + '0';
    rest1 = segundos[CRON] % 1000;
    disp[1] = rest1 / 100 + '0';
    rest2 = rest1 % 100;
    disp[2] = rest2 / 10 + '0';
    disp[3] = rest2 % 10 + '0';
    Serial.println(segundos[CRON]);
    MFS.write(disp);
  }
  else if ((segundos[CRON] >= 6000) && (segundos[CRON] < 360000))
  {
    disp[0] = segundos[CRON] / 60000 + '0';
    rest1 = segundos[CRON] % 60000;
    disp[1] = rest1 / 6000 + '0';
    rest2 = rest1 % 6000;
    disp[2] = rest2 / 1000 + '0';
    rest3 = rest2 % 1000;
    disp[3] = rest3 / 100 + '0';
    MFS.write(disp);
  }
  else
  {
    disp[0] = segundos[CRON] / 3600000 + '0';
    rest1 = segundos[CRON] % 3600000;
    disp[1] = rest1 / 360000 + '0';
    rest2 = rest1 % 360000;
    disp[2] = rest2 / 60000 + '0';
    rest3 = rest2 % 60000;
    disp[3] = rest3 / 6000 + '0';
    MFS.write(disp);
  }
}

void blink_display()
{
  switch (index_display)
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
  switch (button)
  {
    case BUTTON_1_SHORT_RELEASE:
      if (!flag_button_blink)
      {
        if (ja_tocou_timer == 2)
        {
          ja_tocou_timer = 0;
          MFS.beep(0, 0, 0, 0, 0);
          MFS.writeLeds(LED_3, OFF);
        }
        else if (ja_tocou_alarme == 2)
        {
          ja_tocou_alarme = 0;
          MFS.beep(0, 0, 0, 0, 0);
          MFS.writeLeds(LED_2, OFF);
        }
        else
          mode++;
        temp_antes = segundos[mode];
      }
      break;
    case BUTTON_2_PRESSED:
      if (mode != CRON)
      {
        if (index_display == 6)
        {
          flag_button_blink = !flag_button_blink;
          if (mode == REL)
            rel_pisc = 0;
          index_display = 36000;
          temp_antes = segundos[mode];
          segundos[mode] = 0;
          MFS.blinkDisplay(DIGIT_ALL, OFF);
        }
        else
        {
          if (index_display != 3600)
          {
            index_display = index_display / 10;
            if (index_display == 6)
            {
              flag_button_blink = !flag_button_blink;
              if (mode == REL)
                rel_pisc = 0;
              MFS.blinkDisplay(DIGIT_ALL, OFF);
            }
          }
          else
            index_display = index_display / 6;
          temp_antes = segundos[mode];
        }
      }
      if ((mode == CRON) && (segundos[mode] > 0))
      {
        pause = 1;
        segundos[mode] = 0;
      }
      break;
    case BUTTON_3_PRESSED:
      if (flag_button_blink == 1)
      {
        if ((mode == REL) || (mode ==  AL) || (mode == TIM))
        {
          segundos[mode] += index_display;
          check_out_of_bounds(mode);
        }
      }
      if (mode == CRON)
      {
        pause = !pause;
        segundos[mode]++;
      }
      break;
  }
}

void controle_led()
{
  if (mode == REL)
  {
    MFS.writeLeds(LED_1, ON);
    if (segundos[CRON] == 0)
      MFS.writeLeds(LED_4, OFF);
  }
  else
  {
    MFS.writeLeds(LED_1, OFF);
  }
  if (mode != AL)
  {
    if (segundos[AL] > 0)
      MFS.blinkLeds(LED_2, ON);
    else
      MFS.blinkLeds(LED_2, OFF);
  }
  else
  {
    MFS.blinkLeds(LED_2, OFF);
    MFS.writeLeds(LED_2, ON);
  }
  if (mode != TIM)
  {
    if (segundos[TIM] > 0)
      MFS.blinkLeds(LED_3, ON);
    else
      MFS.blinkLeds(LED_3, OFF);
  }
  else
  {
    MFS.blinkLeds(LED_3, OFF);
    MFS.writeLeds(LED_3, ON);
    if (segundos[AL] == 0)
      MFS.writeLeds(LED_2, OFF);
  }
  if (mode != CRON)
  {
    if (segundos[CRON] > 0)
      MFS.blinkLeds(LED_4, ON);
    else
      MFS.blinkLeds(LED_4, OFF);
  }
  else
  {
    MFS.blinkLeds(LED_4, OFF);
    MFS.writeLeds(LED_4, ON);
    if (segundos[TIM] == 0)
      MFS.writeLeds(LED_3, OFF);
  }
}

void check_out_of_bounds(byte i)
{

  if (index_display == 60)
  {
    if (((segundos[i] - temp_antes) / 60) >= 10)
    {
      segundos[i] -= 600;
    }
  }
  else if (index_display == 600)
  {
    if (((segundos[i] - temp_antes) / 600) >= 6)
    {
      segundos[i] -= 3600;
    }
  }
  else if (index_display == 3600)
  {
    if (((segundos[i] - temp_antes) / 3600) >= 10)
    {
      segundos[i] -= 36000;
    }
    else if ((segundos[i] >= 72000) && ((segundos[i] - temp_antes) / 3600) >= 4)
    {
      segundos[i] -= 14400;
    }
  }
  else if (index_display == 36000)
  {
    if (segundos[i] >= 108000)
    {
      segundos[i] -= 108000;
    }
  }
}

void cont_rel()
{
  if ((mode != REL) || (index_display == 6))
    segundos[REL] += 1;
  if (segundos[REL] >= 86400)
    segundos[REL] = segundos[REL] - 86400;
}

void cont_alarme()
{
  if ((segundos[AL] == segundos[REL]) && (index_display == 6) && (segundos[AL] != 0))
  {
    ja_tocou_alarme = 2;
    segundos[AL] = 0;
    MFS.beep(50, 50, 5, 50, 100);
  }
}

void cont_timer()
{
  if (index_display == 6)
  {
    if (segundos[TIM] > 0)
    {
      segundos[TIM] -= 1;
      ja_tocou_timer = 1;
    }
    if (segundos[TIM] == 60)
      MFS.beep(100, 100, 0, 1, 100);
    else if ((segundos[TIM] == 0) && (ja_tocou_timer == 1))
    {
      ja_tocou_timer = 2;
      MFS.beep(75, 75, 10, 5, 200);
    }
  }
}

void setup()
{
  Timer1.initialize();
  MFS.initialize(&Timer1);
  mode = REL;
  flag_button_blink = 0;
  for (int i = 0; i < 4; i++)
  {
    segundos[i] = 0;
  }
  index_display = 6;
  rel_pisc = 1;
  pause = 1;
}

void loop()
{
  delay(10);
  seg++;
  if (seg == 100)
  {
    seg = 0;
    cont_rel();
    cont_alarme();
    cont_timer();
  }
  if ((segundos[CRON] > 0) && (!pause))
  {
    segundos[CRON]++;
  }
  byte but = MFS.getButton();
  button_pressed(but);
  if (flag_button_blink)
    blink_display();
  if (mode > CRON)
    mode = REL;
  if ((index_display == 6) && (((mode == AL) && (segundos[AL] == 0)) || ((mode == TIM) && (segundos[TIM] == 0)) || ((mode == CRON) && (segundos[CRON] == 0)) || ((mode == REL) && (rel_pisc == 1))))
    MFS.blinkDisplay(DIGIT_ALL, ON);
  else if (index_display == 6)
    MFS.blinkDisplay(DIGIT_ALL, OFF);
  if (mode != CRON)
    write_display();
  else
    write_display_cron();
  controle_led();
}
