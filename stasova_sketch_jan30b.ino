#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SENSOR_PIN 35
#define PIN_QUIET 33
#define PIN_MODERATE 25
#define PIN_LOUD 26

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int sampleWindow = 50;
unsigned int sample;

void setup() {
    pinMode(SENSOR_PIN, INPUT);
    pinMode(PIN_QUIET, OUTPUT);
    pinMode(PIN_MODERATE, OUTPUT);
    pinMode(PIN_LOUD, OUTPUT);
    
    digitalWrite(PIN_QUIET, LOW);
    digitalWrite(PIN_MODERATE, LOW);
    digitalWrite(PIN_LOUD, LOW);
    
    Serial.begin(115200);
    
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Loudness Meter");
}

void loop() {
    unsigned long startMillis = millis();                  
    float peakToPeak = 0;                                  
    unsigned int signalMax = 0;                            
    unsigned int signalMin = 1024;                         
    
    while (millis() - startMillis < sampleWindow) {
        sample = analogRead(SENSOR_PIN);
        if (sample < 1024) {                                
            if (sample > signalMax) {
                signalMax = sample;                          
            } else if (sample < signalMin) {
                signalMin = sample;                         
            }
        }
    }
    
    peakToPeak = signalMax - signalMin;                    
    int db = map(peakToPeak, 0, 900, 49, 90);         
    Serial.print("\t");
    Serial.println(db);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loudness: ");
    lcd.print(db);
    lcd.print("dB");
    
    if (db <= 55) {
        lcd.setCursor(0, 1);
        lcd.print("Level: Quiet");
        digitalWrite(PIN_QUIET, HIGH);
        digitalWrite(PIN_MODERATE, LOW);
        digitalWrite(PIN_LOUD, LOW);
    } else if (db > 60 && db < 85) {
        lcd.setCursor(0, 1);
        lcd.print("Level: Moderate");
        digitalWrite(PIN_QUIET, LOW);
        digitalWrite(PIN_MODERATE, HIGH);
        digitalWrite(PIN_LOUD, LOW);
    } else if (db >= 85 && db <= 90) {
        lcd.setCursor(0, 1);
        lcd.print("Level: High");
        digitalWrite(PIN_QUIET, LOW);
        digitalWrite(PIN_MODERATE, LOW);
        digitalWrite(PIN_LOUD, HIGH);
    } else {
        digitalWrite(PIN_QUIET, LOW);
        digitalWrite(PIN_MODERATE, LOW);
        digitalWrite(PIN_LOUD, LOW);
    }
    
    delay(200);
}
