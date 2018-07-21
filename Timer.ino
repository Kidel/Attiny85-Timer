#define F_CPU 8000000UL

#include <avr/interrupt.h>
#include <avr/sleep.h>

const int LED_1 = 0;     //LED row 1
const int LED_2 = 1;     //LED row 2    
const int LED_3 = 2;     //LED row 3
const int BUZZER = 4;    // Buzzer
const int switchPin = 3; // Button

int buttonState = HIGH;
int lastButtonState=HIGH;

int n = 0;
int m = 0;

int timer = 0;
unsigned long old_millis = 0;
int seconds = 0;
int onZero = 0;

int soundOnce = 0;

void sleep() {
    GIMSK |= _BV(PCIE);                     // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT3);                   // Use PB3 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                   // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // replaces above statement

    sleep_enable();                         // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                                  // Enable interrupts
    sleep_cpu();                            // sleep

    cli();                                  // Disable interrupts
    PCMSK &= ~_BV(PCINT3);                  // Turn off PB3 as interrupt pin
    sleep_disable();                        // Clear SE bit
    ADCSRA |= _BV(ADEN);                    // ADC on

    sei();                                  // Enable interrupts
}

ISR(PCINT0_vect) {
    // This is called when the interrupt occurs, but I don't need to do anything in it
}

void setup() 
{
  pinMode(BUZZER, OUTPUT);
  pinMode(switchPin,INPUT);
  digitalWrite(switchPin,HIGH);
}

void loop()
{
  buttonState = digitalRead(switchPin);
  if(buttonState == LOW && lastButtonState != buttonState) { 
    n++;
    n=n%13;
    m++;
    m=m%7;
    tone(BUZZER, (m+1)*1000);
    delay(200);
    old_millis = millis();

    timer = 1;
    seconds = 0;
  }
  lastButtonState=buttonState;
  turnOn(n);
  noTone(BUZZER);

  if(soundOnce >= 1 && n < 1 && timer < 1) {
    sleep();
  }
  
  if (timer > 0 && n > 0 && ((seconds) > (60*n) + 3)) {
    for(int j=0; j<10; j++) {
      int a = 0;
      int t = 0;
      for(int i=0; i<7; i++) {
        if((++a)>6) a = 12;
        turnOn(a);
        tone(BUZZER, (++t)*1000);
        delay(200);
      }
    }
    timer = 0;
    n = 0;
    m = 0;
    soundOnce = 1;
  } else if(timer > 0) {
    if(millis() - old_millis > 500) {
      if(onZero < 1) turnOn(0);
      onZero = 1;
      delay(200);
    }
    if(millis() - old_millis > 1000) {
      //turnOn(n);
      onZero = 0;
      old_millis = millis();
      seconds++;
      delay(200);
    }
  }
}

void turnOn(int combination) {
  switch (combination) {
    case 1:
      pinMode(LED_1, OUTPUT);     //row 1
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, HIGH);  
      pinMode(LED_3, INPUT);      //row 3
      digitalWrite(LED_3, LOW);
      break;
    case 2:
      pinMode(LED_1, OUTPUT);     //row 1
      digitalWrite(LED_1, HIGH);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, LOW);   
      pinMode(LED_3, INPUT);      //row 3
      digitalWrite(LED_3, LOW);
      break;
    case 3:
      pinMode(LED_1, INPUT);      //row 1
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, HIGH);  
      pinMode(LED_3, OUTPUT);     //row 3
      digitalWrite(LED_3, LOW);
      break;
    case 4:
      pinMode(LED_1, INPUT);     //row 1
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, OUTPUT);    //row 2
      digitalWrite(LED_2, LOW);  
      pinMode(LED_3, OUTPUT);    //row 3
      digitalWrite(LED_3, HIGH);
      break;
    case 5:
      pinMode(LED_1, OUTPUT);    //row 1
      digitalWrite(LED_1, HIGH);
      pinMode(LED_2, INPUT);     //row 2
      digitalWrite(LED_2, LOW);
      pinMode(LED_3, OUTPUT);    //row3
      digitalWrite(LED_3, LOW);
      break;
    case 6:
      pinMode(LED_1, OUTPUT);
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, INPUT);
      digitalWrite(LED_2, LOW);
      pinMode(LED_3, OUTPUT);
      digitalWrite(LED_3, HIGH);
      break;
    case 7:
      pinMode(LED_1, OUTPUT);     //row 1
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, HIGH);   
      pinMode(LED_3, OUTPUT);      //row 3
      digitalWrite(LED_3, LOW);
      break;
    case 8:
      pinMode(LED_1, OUTPUT);
      digitalWrite(LED_1, HIGH);
      pinMode(LED_2, OUTPUT);
      digitalWrite(LED_2, LOW);
      pinMode(LED_3, OUTPUT);
      digitalWrite(LED_3, HIGH);
      break;
    case 9:
      pinMode(LED_1, OUTPUT);     //row 1
      digitalWrite(LED_1, HIGH);
      pinMode(LED_2, OUTPUT);    //row 2
      digitalWrite(LED_2, HIGH);  
      pinMode(LED_3, OUTPUT);    //row 3
      digitalWrite(LED_3, LOW);
      break;
    case 10:
      pinMode(LED_1, OUTPUT);      //row 1
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, LOW);  
      pinMode(LED_3, OUTPUT);     //row 3
      digitalWrite(LED_3, HIGH);
      break;
    case 11:
      pinMode(LED_1, OUTPUT);     //row 1
      digitalWrite(LED_1, HIGH);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, LOW);  
      pinMode(LED_3, OUTPUT);      //row 3
      digitalWrite(LED_3, LOW);
      break;
    case 12:
      pinMode(LED_1, OUTPUT);    //row 1
      digitalWrite(LED_1, LOW);
      pinMode(LED_2, OUTPUT);     //row 2
      digitalWrite(LED_2, HIGH);
      pinMode(LED_3, OUTPUT);    //row3
      digitalWrite(LED_3, HIGH);
      break;
    default:
      pinMode(LED_1, INPUT);      //row 1
      digitalWrite(LED_1, HIGH);
      pinMode(LED_2, INPUT);     //row 2
      digitalWrite(LED_2, HIGH);  
      pinMode(LED_3, INPUT);     //row 3
      digitalWrite(LED_3, HIGH);
  }
}
