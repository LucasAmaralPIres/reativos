//circuits.io
#define LED_PIN 13
#define BUT_PIN 2
#define BUT2_PIN 4

unsigned long old;
unsigned long old2;
unsigned long old3;
int st;
unsigned long ace;
int cont;
void setup() 
{
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUT_PIN,INPUT);
  st = 0;
  ace = 1000;  
}

void loop() 
{
  unsigned long now = millis();
  if(now >= (old+ace))
  {
    old = now;
    st = !st;
    digitalWrite(LED_PIN,st);
  }
  if(now >= (old2+200))
  {
    old2 = now;
    if(cont == 2)
    {
      ace -=100;
      cont = 1;
    }
    else if (cont == 3)
    {
      ace += 100; 
      cont = 1;
    }
  }
  int but = digitalRead(BUT_PIN);
  if(!but)
  {
    cont = 2;
  }
  int but2 = digitalRead(BUT2_PIN);
  if(!but2)
  {
    cont = 3;
  }
  if((!but) && (!but2))
  {
    old3 += 1;
    if() >=)
    {
      digitalWrite(LED_PIN,LOW);
      while(1); 
    }
  }

}
