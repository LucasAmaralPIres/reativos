#define LED_PIN 13
#define BUT_PIN 2
#define BUT2_PIN 4

unsigned long old;
int st;
unsigned long ace;
int aci1,aci2;
unsigned long cont_tmp1,cont_tmp2;
void setup() 
{
  pinMode(LED_PIN,OUTPUT);
  pinMode(BUT_PIN,INPUT);
  pinMode(BUT2_PIN,INPUT);
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
  int but = digitalRead(BUT_PIN);
  if(!but)
  {
    if(aci1 == 0)
    {
      ace -= 100; 
      cont_tmp1 = millis();
    }
    aci1 = 1;
  }
  else
  {
    aci1 = 0;
    cont_tmp1 = 0;
  }
  but = digitalRead(BUT2_PIN);
  if(!but)
  {
    if(aci2 == 0)
    {
      ace += 100; 
      cont_tmp2 = millis();
    }
    aci2 = 1;
  }
  else
  {
    aci2 = 0;
    cont_tmp2 = 0;
  }
  if((cont_tmp1 != 0) && (cont_tmp2 != 0))
  {
    if(((cont_tmp1-cont_tmp2)<=500) || ((cont_tmp2-cont_tmp1)<=500))
    {
      digitalWrite(LED_PIN, LOW);
      while(1); 
    }
  }
}
