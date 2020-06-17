

// include the library code:
#include <LiquidCrystal.h>
#include <SimpleDHT.h>
#include <Keypad.h>
#include <Servo.h>
Servo myservo;
#define buzzer 6
#define BLUE 8
#define GREEN 9
#define RED 10
int redValue;
int greenValue;
int blueValue;
int motor = 39;
int adc_id = 3;
int HistoryValue = 0;
char printBuffer[128];
int ledPin = 33;
int pirPin = 52;
int pirValue;
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean passChangeMode = false;
boolean passChanged = false;
boolean activated = false;
int screenOffMsg =0;
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  String password="1234";
char keypressed;
String test;
String tempPassword;
int led_red=41;
int led_green=40;
int pinDHT11 = 51;
SimpleDHT11 dht11;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {43, 44, 45, 46}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {47, 48, 49, 50}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad myKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Welcome Home");
  delay(2000);
  pinMode(ledPin,OUTPUT);
  pinMode(pirPin,INPUT);
  digitalWrite(ledPin,LOW);
  myservo.attach(53);
  myservo.write(90);
  pinMode(motor,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(led_red,OUTPUT);
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
}

void loop() {
    int value = analogRead(adc_id); // get adc value
    
    if (activateAlarm) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm will be");
    lcd.setCursor(0,1);
    lcd.print("activated in");
   
    int countdown = 5; // 9 seconds count down before activating the alarm
    while (countdown != 0) {
      lcd.setCursor(13,1);
      lcd.print(countdown);
      countdown--;
      tone(buzzer, 700, 100);
      delay(1000);
    }
    myservo.write(0); 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm Activated!");
    activateAlarm = false;
    alarmActivated = true;
  }
  
   if (alarmActivated == true){        
        digitalWrite(led_green,LOW);
        digitalWrite(led_red,HIGH);
        lcd.clear();
        pirValue = digitalRead(pirPin);
        if(pirValue > 0){
          digitalWrite(ledPin,HIGH);
          tone(buzzer, 1000);
          enterPassword();
        }
      }
    
    
  if (!alarmActivated) {
    digitalWrite(led_red,LOW);
    digitalWrite(led_green,HIGH);
    if (screenOffMsg == 0 ){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - Activate");
      lcd.setCursor(0,1);
      lcd.print("B - Change Pass");
      screenOffMsg = 1;
    }
    keypressed = myKeypad.getKey();
     if (keypressed =='A'){        //If A is pressed, activate the alarm
      tone(buzzer, 1000, 200);
      activateAlarm = true;
      
                
    }
     if (keypressed =='B') {
      lcd.clear();
      int i=1;
      tone(buzzer, 2000, 100);
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Current Password");
      lcd.setCursor(0,1);
      lcd.print(">");
      passChangeMode = true;
      passChanged = true;   
      while(passChanged) {      
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
         tempPassword += keypressed;
         lcd.setCursor(i,1);
         lcd.print("*");
         i++;
         tone(buzzer, 2000, 100);
        }
      }
      if (i > 5 || keypressed == '#') {
        tempPassword = "";
        i=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Password");
        lcd.setCursor(0,1);
        lcd.print(">"); 
      }
      if ( keypressed == '*') {
        i=1;
        tone(buzzer, 2000, 100);
        if (password == tempPassword) {
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set New Password");
          lcd.setCursor(0,1);
          lcd.print(">");
          while(passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }
            }
            if (i > 5 || keypressed == '#') {
              tempPassword = "";
              i=1;
              tone(buzzer, 2000, 100);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set New Password");
              lcd.setCursor(0,1);
              lcd.print(">");
            }
            if ( keypressed == '*') {
              i=1;
              tone(buzzer, 2000, 100);
              password = tempPassword;
              passChangeMode = false;
              passChanged = false;
              screenOffMsg = 0;
            }            
          }
        }
      }
    }
   }
        else if (keypressed =='C'){
          temp();
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("T:  ");lcd.print(temperature);lcd.print(" *C");
        lcd.setCursor(0,1);
        lcd.print("H:  ");lcd.print(humidity);lcd.print(" %");
        
    }
    else if(keypressed =='#'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("C - Temperaut");
      lcd.setCursor(0,1);
      lcd.print("D - LG");
    }
     else if(keypressed =='0'){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - Activate");
      lcd.setCursor(0,1);
      lcd.print("B - Change Pass");
    }
    else if(keypressed =='1'){
      digitalWrite(motor,HIGH);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LG ON");
      lcd.setCursor(0,1);
      lcd.print("2 - oFF");
    }
    else if(keypressed =='2'){
      digitalWrite(motor,LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LG OFF");
      lcd.setCursor(0,1);
      lcd.print("1 - ON");
    }
    else if(keypressed =='5'){
       lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LG ON");
      lcd.setCursor(0,1);
      lcd.print("6 - OFF");
      #define delayTime 10 // fading time between colors

redValue = 255; // choose a value between 1 and 255 to change the color.
greenValue = 0;
blueValue = 0;

// this is unnecessary as we've either turned on RED in SETUP
// or in the previous loop ... regardless, this turns RED off
// analogWrite(RED, 0);
// delay(1000);

for(int i = 0; i < 255; i += 1) // fades out red bring green full when i=255
{
redValue -= 1;
greenValue += 1;
// The following was reversed, counting in the wrong directions
// analogWrite(RED, 255 - redValue);
// analogWrite(GREEN, 255 - greenValue);
analogWrite(RED, redValue);
analogWrite(GREEN, greenValue);
delay(delayTime);
}

redValue = 0;
greenValue = 255;
blueValue = 0;

for(int i = 0; i < 255; i += 1) // fades out green bring blue full when i=255
{
greenValue -= 1;
blueValue += 1;
// The following was reversed, counting in the wrong directions
// analogWrite(GREEN, 255 - greenValue);
// analogWrite(BLUE, 255 - blueValue);
analogWrite(GREEN, greenValue);
analogWrite(BLUE, blueValue);
delay(delayTime);
}

redValue = 0;
greenValue = 0;
blueValue = 255;

for(int i = 0; i < 255; i += 1) // fades out blue bring red full when i=255
{
// The following code has been rearranged to match the other two similar sections
blueValue -= 1;
redValue += 1;
// The following was reversed, counting in the wrong directions
// analogWrite(BLUE, 255 - blueValue);
// analogWrite(RED, 255 - redValue);
analogWrite(BLUE, blueValue);
analogWrite(RED, redValue);
delay(delayTime);
}
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("5-RGB OFF");
      lcd.setCursor(0,1);
      lcd.print("RGB - OFF");
    }
    else if(keypressed =='6'){
     analogWrite(BLUE, LOW);
         analogWrite(GREEN, LOW);
         analogWrite(RED, LOW);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("LD OFF");
      lcd.setCursor(0,1);
      lcd.print("5 - ON");
    }
    else if(keypressed =='7'){
      

    if(((HistoryValue>=value) && ((HistoryValue - value) > 10)) || ((HistoryValue<value) && ((value - HistoryValue) > 10)))
    {
      sprintf(printBuffer,"ADC%d level is %d\n",adc_id, value);
      Serial.print(printBuffer);
      HistoryValue = value;
    }
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print('HistoryValue');
      lcd.setCursor(0,1);
      lcd.print("1 - ON");
    }
 }
}
void enterPassword() {
  int k=5;
  tempPassword = "";
  activated = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" *** ALARM *** ");
  lcd.setCursor(0,1);
  lcd.print("Pass>");
      while(activated) {
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
          tempPassword += keypressed;
          lcd.setCursor(k,1);
          lcd.print("*");
          k++;
        }
      }
      if (k > 9 || keypressed == '#') {
        tempPassword = "";
        k=5;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" *** ALARM *** ");
        lcd.setCursor(0,1);
        lcd.print("Pass>");
      }
      if ( keypressed == '*') {
        if ( tempPassword == password ) {
          activated = false;
          alarmActivated = false;
          noTone(buzzer);
          screenOffMsg = 0; 
        }
        else if (tempPassword != password) {
          lcd.setCursor(0,1);
          lcd.print("Wrong! Try Again");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" *** ALARM *** ");
          lcd.setCursor(0,1);
          lcd.print("Pass>");
        }
      }    
    }
    
    myservo.write(90);
    
}
void temp(){
   // start working...
  
  
  // read with raw sample data.

  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed");
    return;
  }
  

  Serial.print((int)temperature); Serial.print(" *C, ");
  Serial.print((int)humidity); Serial.println(" %");
}
