//Professor, não consegui acessar o site circuits.io para testar o programa.
/*
Piscar o LED a cada 1 segundo
Botão 1: Acelerar o pisca-pisca a cada pressionamento
Botão 2: Desacelerar a cada pressionamento
Botão 1+2 (em menos de 500ms): Parar

//Foi realizado esse trabalho conforme colocado no github.
*/
#define LED_PIN 13
#define BUT_PIN 2
#define BUT2_PIN 4

unsigned long old;                      //Varíavel usada para controlar o tempo decorrido entre as atualizações do LED
int st;                                 //Responsável por alterar o estado do LED
unsigned long ace;                      //Controla a aceleração o pisca-picas do LED
int aci1,aci2;                          //Controla a leitura do botão assegurando que cada clique do botão só seja executado uma vez
unsigned long cont_tmp1,cont_tmp2;      //Controla o tempo decorrido desde o botão ser apertado até ele ser 'desapertado'
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
  unsigned long now = millis();         //Controla o tempo decorrido desde o inpicio do programa
  if(now >= (old+ace))                  //Controla o pisca-pisca do LED
  {                                     //...
    old = now;                          //...
    st = !st;                           //...
    digitalWrite(LED_PIN,st);           //...
  }                                     //...
  int but = digitalRead(BUT_PIN);       //Lê o estado do botão 1
  if(!but)                              //Caso apertado entra no if
  {                                     //  
    if(aci1 == 0)                       //Só entra no if uma vez a cada vez que o botão é apertado
    { 
      ace -= 100;                       //Controla a velocidade do pisca-pisca do LED 
      cont_tmp1 = millis();             //Controla o tempo em que o botão foi apertado.
    }
    aci1 = 1;                           //Controla a entrada do if imediatamente anterior
  }
  else
  {
    aci1 = 0;
    cont_tmp1 = 0;                      //Controla o tempo de duração do aperto do botão 1
  }
  but = digitalRead(BUT2_PIN);          //Igual a botão 1 aplicado ao botão 2
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
  if((cont_tmp1 != 0) && (cont_tmp2 != 0))      //Só entra caso os doi botões estajam apertados
  {
    if(((cont_tmp1-cont_tmp2)<=500) || ((cont_tmp2-cont_tmp1)<=500))    //Controla se os dois botões foram apertados em menos de 500ms 
    {
      digitalWrite(LED_PIN, LOW);               
      while(1);                                 //Cria um loop infinito
    }
  }
}
