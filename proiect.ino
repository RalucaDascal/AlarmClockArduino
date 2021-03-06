#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
RTC_DS3231 RTC;

//declarare butoane
int buttonAlarm=5; 
int button1=6; 
int button2=7;
int button3=8; 
int button4=9;
//declarare intrare pin senzor
int soundPin=A0;
//declarare iesiri
#define buzzer 10
#define LED 11

int hourAux=0;
int minAux=0;
int yearAux;
int monthAux=1;
int dayAux=1;
int pressedButton1 =0;
int pressedButtonAlarm=0;

uint8_t hour = 0, min = 0; 

void setup()
{
  //initializare lcd
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  //butoane
  pinMode(button1,INPUT_PULLUP);
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  pinMode(button4,INPUT_PULLUP);
  pinMode(buttonAlarm,INPUT_PULLUP);

 //output-uri
  pinMode(LED,OUTPUT);
  pinMode(buzzer, OUTPUT);
   
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  //in cazul in  care apare o eroare la conecatarea RTC-ului, acesta este semnalata in Serial Montiro
 if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  // Se initializeaza ceasul cu datele din laptop
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
}
 
void loop()
{
  if(digitalRead(button1)== LOW) 
  {
   pressedButton1=pressedButton1+1;
  }
  // in functie de apasarile primului buton se initializeaza diferite actiuni care vor fi explicate ulterior
  if(pressedButton1==0)
  {
    DisplayDateTime();
    Alarm();
  }
  if (pressedButton1==1)
    { SetHour();}
  if (pressedButton1==2)
    {SetMinute();}
  if (pressedButton1==3)
    {SetDay();}
  if (pressedButton1==4)
    {SetMonth();}
  if (pressedButton1==5)  
    {SetYear();}
  if (pressedButton1==6)
  {   updateDateTime(); 
      delay(500);
      pressedButton1=0;
  }    
 // in functie de apasarile celui de al patrulea buton se initializeaza diferite actiuni in ceea ce priveste setarea alarmei 
  if((digitalRead(button4)== LOW))                                                                                                                                                                     
  {
    SetHourAlarm();
    SeMinutetAlarm();
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("Alarm ");
    lcd.setCursor(6,1);
    lcd.print(hour, DEC);
    lcd.print(":");
    if (min<=9)
     {
      lcd.print("0");
      lcd.print(min, DEC);
      }
    else
     {
          lcd.print(min, DEC);
     }
    delay(1000);
    lcd.clear();
  }

  // Cand cele doua butoane sunt apasate deodata este vizibila alarma setata, iar in cazul in care nu exista o alarma activa un mesaj corespunzator
  if((digitalRead(button2)== LOW)&&(digitalRead(button3)== LOW)) 
    {
      if ((pressedButtonAlarm==1))         
    {
      lcdAlarm();
      delay(2000);
    }
    else {
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print ("No alarm");
      delay(2000);
      }
    }
 
  delay(100);
}

//Afisarea datei si timpului curent conform unui anumit format
void DisplayDateTime ()
{
  lcd.clear();
  DateTime now = RTC.now();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  
  if (now.minute()<10)
  {
    lcd.print("0");
    lcd.print(now.minute(), DEC);
  }
  else{
     lcd.print(now.minute(), DEC);
      }
  
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day(), DEC);
  
  lcd.print("/");
  if (now.month()<10)
  {
    lcd.print("0");
    lcd.print(now.month(), DEC);
  }
   else {  
      lcd.print(now.month(), DEC);
        } 
  lcd.print("/");
  lcd.print(now.year(), DEC);
  yearAux=now.year();
}

// Setarea orei - cu butoanele 2 si 3 se face parcurgerea intervalului 0-23
void SetHour()
{
  lcd.clear();
  DateTime now = RTC.now();
  if(digitalRead(button3)==LOW)
  {
    if(hourAux==23)
    {
      hourAux=0;
    }
    else
    {
      hourAux=hourAux+1;
    }
  }
   if(digitalRead(button2)==LOW)
  {
    if(hourAux==0)
    {
      hourAux=23;
    }
    else
    {
      hourAux=hourAux-1;
    }
  }
  lcd.setCursor(4,0);
  lcd.print("Set hour: ");
  lcd.setCursor(7,1);
  lcd.print(hourAux,DEC);
  delay(100);
}

// Setarea minutului - cu butoanele 2 si 3 se face parcurgerea intervalului 0-59
void SetMinute()
{

  lcd.clear();
  if(digitalRead(button3)==LOW)
  {
    if (minAux==59)
    {
      minAux=0;
    }
    else
    {
      minAux=minAux+1;
    }
  }
   if(digitalRead(button2)==LOW)
  {
    if (minAux==0)
    {
      minAux=59;
    }
    else
    {
      minAux=minAux-1;
    }
  }
  lcd.setCursor(3,0);
  lcd.print("Set minute: ");
  lcd.setCursor(7,1);
  lcd.print(minAux,DEC);
  delay(100);
}

// Setarea anului - cu butoanele 2 si 3 se face parcurgerea anilor (nu avem limite)
 
void SetYear()
{

  lcd.clear();
  if(digitalRead(button3)==LOW)
  {    
    yearAux=yearAux+1;
  }
   if(digitalRead(button2)==LOW)
  {
    yearAux=yearAux-1;
  }
  lcd.setCursor(4,0);
  lcd.print("Set year: ");
  lcd.setCursor(7,1);
  lcd.print(yearAux,DEC);
  delay(100);
}

 // Setarea lunii- cu butoanele 2 si 3 se face parcurgerea intervalului 1-11
void SetMonth()
{
  lcd.clear();
  if(digitalRead(button3)==LOW)
  {
    if (monthAux==12)
    {
      monthAux=1;
    }
    else
    {
      monthAux=monthAux+1;
    }
  }
   if(digitalRead(button2)==LOW)
  {
    if (monthAux==1)
    {
      monthAux=12;
    }
    else
    {
      monthAux=monthAux-1;
    }
  }
  lcd.setCursor(3,0);
  lcd.print("Set month: ");
  lcd.setCursor(7,1);
  lcd.print(monthAux,DEC);
  delay(100);
}

// Setarea zilei - cu butoanele 2 si 3 se face parcurgerea intervalului 1-31
void SetDay()
{
  lcd.clear();
  if(digitalRead(button3)==LOW)
  {
    if (dayAux==31)
    {
      dayAux=1;
    }
    else
    {
      dayAux=dayAux+1;
    }
  }
   if(digitalRead(button2)==LOW)
  {
    if (dayAux==1)
    {
      dayAux=31;
    }
    else
    {
      dayAux=dayAux-1;
    }
  }
  lcd.setCursor(4,0);
  lcd.print("Set day: ");
  lcd.setCursor(7,1);
  lcd.print(dayAux,DEC);
  delay(100);
}
// In urma setarii timpului si a datei se face o actualizare a RTC-ului si apoi se revine in etapa de afisare a acestora
void updateDateTime()
{
  lcd.clear();
  RTC.adjust(DateTime(yearAux,monthAux,dayAux,hourAux,minAux,0));
  DisplayDateTime();
  delay(100);
} 

// Setarea orei pentru alarma - cu butoanele 2 si 3 se face parcurgerea intervalului 0-23 (Merge pe acelasi criteriu cu setarea orei)
void SetHourAlarm()
{
  while(digitalRead(button1)==HIGH){
  lcd.clear();
  if(digitalRead(button3)==LOW)
  {
    if(hour==23)
    {
      hour=0;
    }
    else
    {
      hour=hour+1;
    }
  }
   if(digitalRead(button2)==LOW)
  {
    if(hour==0)
    {
      hour=23;
    }
    else
    {
      hour=hour-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set hour alarm: ");
  lcd.setCursor(7,1);
  lcd.print(hour,DEC);
  delay(100);
 }
 delay(100);
}
// Setarea minutului - cu butoanele 2 si 3 se face parcurgerea intervalului 0-59  (Merge pe acelasi criteriu cu setarea minutului)
void SeMinutetAlarm()
 {
  while(digitalRead(button1)==HIGH){ 

  lcd.clear();
  if(digitalRead(button3)==LOW)
  {
    if (min==59)
    {
      min=0;
    }
    else
    {
      min=min+1;
    }
  }
   if(digitalRead(button2)==LOW)
  {
    if (min==0)
    {
      min=59;
    }
    else
    {
      min=min-1;
    }
  }
  lcd.setCursor(0,0);
  lcd.print("Set minute alarm:");
  lcd.setCursor(7,1);
  lcd.print(min,DEC);
  delay(100);
 }
 delay(100);
}
// in cazul in care sunt actionate butoanele 2 si 3 simultan este posibila vederea alarmei conform unui anumit format
void lcdAlarm(){
  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("Alarm ");
  
  lcd.setCursor(6,1);
  lcd.print(hour, DEC);
  lcd.print(":");
  if (min < 10)
  {
    lcd.print("0");
    lcd.print(min, DEC); 
  }
  else {
     lcd.print(min, DEC); 
  }
}
//Activarea alarmei
void Alarm()
{
 if(digitalRead(buttonAlarm)== LOW)
  {
   pressedButtonAlarm=pressedButtonAlarm+1;
  }
  // daca alarma este dezactivata in partea dreapta sus a LCD-ului se va afisa OFF, iar led-ul si buzzer-ul nu vor functiona.
  if (pressedButtonAlarm==0)
    {
     noTone (buzzer);
     digitalWrite(LED,LOW);
     lcd.setCursor (13,0);
     lcd.print ("OFF");
     LcdOnOff();
     }
     // daca alarma este activata se va executa corpul acestui IF
  if (pressedButtonAlarm==1)
    { 

     lcd.setCursor (13,0);
     lcd.print (" ON");
     DateTime now = RTC.now();
     // cu 10 secunde inainte de alarma setata ledul va palpai si buzzer-ul va suna aproximativ jumatate de secunda din fiecare cele 10
     if (now.hour() == hour && now.minute() ==min-1 && now.second() >= 48 && now.second() <=59)
      {
          tone(buzzer,1000);
          digitalWrite(LED,HIGH);
          delay (400);
          noTone(buzzer);
          digitalWrite(LED,LOW);
          delay(500);
      }
      // cand timpul curent este egala cu alarma atunci se va apela procedura de afisare a alarmei, buzzer-ul va suna continuu, iar led-ul va fi aprins pana cand este apasat
      //butonul pentru dezactivarea alarmei sau trece 1 minut
     else if ( now.hour() == hour && now.minute() == min )
        {
         lcdAlarm();
         DateTime now = RTC.now();
         digitalWrite(LED,HIGH);
         tone(buzzer,400); 
         delay (300);
         tone(buzzer,450); 
        }
    else{
      //daca este activata alarma, insa timpul curent nu este acelasi cu alarma buzzer-ul nu va suna, iar ledul va fi oprit
         LcdOnOff(); 
         noTone (buzzer);
         digitalWrite(LED,LOW);
        }
    } 
  if (pressedButtonAlarm==2)
    {
     pressedButtonAlarm=0;
    }
    delay(200);
}
//functionarea senzorului de sunet
void LcdOnOff(){
long aux = 0;
// se fac 300 de citiri ale senzorului
for(int i=0; i<300; i++)
{
aux += analogRead(soundPin);
}

aux = aux/300; 
// in cazul in care s-a produs un zgomot suficient de puternic lcd-ul va fi oprit timp de 3 secunde
if(aux > 50){ 
    lcd.noBacklight();
    lcd.noDisplay();
    delay(3000);
//altfel lcd-ul va fi activ si se va afisa data si timpul curent
}else{
   lcd.backlight();
   lcd.display();
  }
delay(10);
}
