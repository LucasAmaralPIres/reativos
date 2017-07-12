#include <SPI.h>
#include <MFRC522.h>
#define red 14
#define blue 16
#define green 15
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
  
char st[20];
char digt[15];
int cont_di;
long old_time, new_time;

void setup() 
{
  cont_di = 0;
  pinMode(red,OUTPUT);
  pinMode(blue,OUTPUT);
  pinMode(green,OUTPUT);
  //Pinos ligados aos pinos 1, 2, 3 e 4 do teclado - Linhas
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  //Pinos ligados aos pinos 5, 6, 7 e 8 do teclado - Colunas
  pinMode(7, INPUT);
  pinMode(2, INPUT);
  pinMode(8, INPUT);
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.begin(9600);
  digitalWrite(red, HIGH);  
  Serial.println("Aguardando acionamento das teclas ou leitura de cartão...");
  Serial.println();
}

void loop() 
{
    new_time = millis();
    for (int ti = 3; ti<7; ti++)
    {
      if (mfrc522.PICC_IsNewCardPresent()) 
      {
        if (mfrc522.PICC_ReadCardSerial()) 
        {
//          Serial.println();
          String conteudo= "";
          byte letra;
          for (byte i = 0; i < mfrc522.uid.size; i++) 
          {
            Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
            Serial.print(mfrc522.uid.uidByte[i], HEX);
            conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
            conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          Serial.println();
        }
        delay(1000);
      }
      //Alterna o estado dos pinos das linhas
      digitalWrite(3, LOW);
      digitalWrite(4, LOW);
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      digitalWrite(ti, HIGH);
      //Verifica se alguma tecla da coluna 1 foi pressionada
      if (digitalRead(7) == HIGH)
      {
        imprime_linha_coluna(ti-2, 1);
        while(digitalRead(7) == HIGH){}
      }
  
      //Verifica se alguma tecla da coluna 2 foi pressionada    
      if (digitalRead(2) == HIGH)
      {
        imprime_linha_coluna(ti-2, 2);
        while(digitalRead(2) == HIGH){};
      }
     
      //Verifica se alguma tecla da coluna 3 foi pressionada
      if (digitalRead(8) == HIGH)
      {
        imprime_linha_coluna(ti-2, 3);
        while(digitalRead(8) == HIGH){}
      }   
    }
    delay(10);
    if(Serial.available()>0)
    {
      int rec = Serial.read();
      if(rec == 1)
      {
        digitalWrite(red, HIGH);
        digitalWrite(green, LOW);
        digitalWrite(blue, LOW);
      }
      if(rec == 2)
      {
        digitalWrite(red, LOW);
        digitalWrite(green, HIGH);
        digitalWrite(blue, LOW);
        old_time = millis();
      }
      if(rec == 3)
      {
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(blue, HIGH);
      }
      if(rec == 4)
      {
        digitalWrite(red, LOW);
        digitalWrite(green, HIGH);
        digitalWrite(blue, LOW);
        delay(500);        
        digitalWrite(red, LOW);
        digitalWrite(green, LOW);
        digitalWrite(blue, HIGH);
      }
    }
    if((!digitalRead(red)) && (digitalRead(green)) && ((new_time - old_time)>5000))
    {
      digitalWrite(red,HIGH);
      digitalWrite(green,LOW); 
    }
}

void imprime_linha_coluna(int x, int y)
{
//  Serial.println();
  switch(x)
  {
    case 1:
      switch(y)
      {
        case 1:
          digt[cont_di++] = '1';
          break;
        case 2:
          digt[cont_di++] = '2';
          break;
        case 3:
          digt[cont_di++] = '3';
          break;
      }
      break;
    case 2:
      switch(y)
      {
        case 1:
          digt[cont_di++] = '4';
          break;
        case 2:
          digt[cont_di++] = '5';
          break;
        case 3:
          digt[cont_di++] = '6';
          break;
      }
      break;
    case 3:
      switch(y)
      {
        case 1:
          digt[cont_di++] = '7';
          break;
        case 2:
          digt[cont_di++] = '8';
          break;
        case 3:
          digt[cont_di++] = '9';
          break;
      }
      break;
    case 4:
      switch(y)
      {
        case 1:
          for(int i = 0;i<cont_di;i++)
          {
            digt[i] = 0;
          }
          cont_di = 0;
          break;
        case 2:
          digt[cont_di++] = '0';
          break;
        case 3:
          if(cont_di != 0)
          {
            Serial.println(digt);
            delay(1000);
            for(int i = 0;i<cont_di;i++)
            {
              digt[i] = 0;
            }
            cont_di = 0;
          }
          else
          {
            Serial.println("!");
          }
          break;
      }
      break;
  }
}
