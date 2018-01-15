
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <DS3231.h>

int relayPin = 10;
int buttonPin1 = 6;
int buttonPin2 = 7;

DS3231 rtc( A4, A5);
Time t;
LiquidCrystal_I2C screen(0x27,2,1,0,4,5,6,7,3, POSITIVE);




void setup() {
  // put your setup code here, to run once:
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.begin(9600);
  rtc.begin();
  screen.begin(16,2);
}


//Diverse variabler
bool ScreenState = false;
int infoState = 0;
int onTime = 9;
int offTime = 16;
String printStr1;
String printStr2;
unsigned long tim = 0;
unsigned long timold = 0;
bool relayOverride = 0;
int ovState = 0;

void loop() {
  // put your main code here, to run repeatedly:
  t = rtc.getTime();
  
  //Ser om tiden er innenfor parametre, og aktiverer eller skrur av lampe
 
   if(t.hour == onTime && t.min == 0 && t.sec == 0){
    if(t.dow >=0 && t.dow <= 4){
      ovState = 0;
    }
  }
  
  if(digitalRead(buttonPin2) == HIGH){
    changeOV(ovState);
  }
  
  if(ovState == 0){
     
        if(checkTime(onTime,offTime, t) &&t.dow >=0 && t.dow <= 4){
            digitalWrite(relayPin, HIGH);
        }
        else{
          digitalWrite(relayPin, LOW);
        
     }
  }
  else if (ovState == 1){
    digitalWrite(relayPin, LOW);
  }
  else if (ovState == 2){
    digitalWrite(relayPin, HIGH);
  }
  

  
  
  
  
  

  if(digitalRead(buttonPin1) == HIGH){
    changeScreenState(infoState);
  }
  if(digitalRead(buttonPin2) == HIGH){
    changeOV(ovState);
  }
  
  
  //Vise informasjon på skjerm:
  
  displayInfo(infoState);
  
}


void changeOV(int& state){
  state++;
  if(ovState == 3){
    ovState = 0;
  }
  delay(300);
}


 








void printToScreen(String str1, String str2){
  
  
    screen.setCursor(0,0);
    screen.print(str1);
    screen.setCursor(0,1);
    screen.print(str2);
  
}




String timeStr(Time t){
  String str;
  str = t.hour;
  str += ":";
  str += t.min;
  str += ":";
  str += t.sec;
  return str;
}

String timeStrDown(Time t){
  String str;
  
  int temp;
  if(t.hour > onTime){
    temp = 24 + onTime - t.hour;
  }
  else{
    temp = onTime - t.hour;
  }
  str+=temp;
  str += ":";
  str += String(60 - t.min);
  str += ":";
  str += String(60 - t.sec);
  return str;
}

void displayInfo(int state){
  screen.setCursor(0,0);
  switch(state){
    case 0:
      {
        screen.backlight();
       printStr1 = "Klokka er:";
       printStr2 = timeStr(t);
       printToScreen(printStr1, printStr2);
       break;
      }
      case 1:{
        screen.backlight();
        printStr1 = "Rele startes om:         ";
       printStr2 = timeStrDown(t) + "           ";
       printToScreen(printStr1, printStr2);
       break;
      }
      case 2:{
        int temp = rtc.getTemp();
        printStr1 = "Temperaturen er:         ";
        printStr2 = temp;
        printStr2 = printStr2 + " C            ";
        printToScreen(printStr1, printStr2);
        break;
      }
      
      case 3:{
        screen.backlight();
        //screen.clear();
        printStr1 = "Override               ";
        if(ovState == 1 || ovState == 2){
          printStr2 = "on                      "; 
        }
        else{
          printStr2 = "off                   ";
        }
        printToScreen(printStr1, printStr2);
        break;
      }

      
     
      
       case 4:{
        screen.clear();
        screen.noBacklight();
        break;
      }
  }
}


void changeScreenState(int& state){
  state++;
  if (state == 5){
    state =0; 
  }
  delay(300);
}




bool checkTime(int lowHour, int highHour, Time t){
  if (t.hour >= lowHour && t.hour < highHour){
    return true;
  }
  else{
    return false;
  }
}















