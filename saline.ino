#include <HX711.h>
#include <SoftwareSerial.h>

HX711 scale;
const int buzzerPin = 2;


SoftwareSerial serialSIM800(9, 8);

int a = 0; 
int b = 0; 

void setup() {
  Serial.begin(9600); 
  scale.begin(A1, A0); 
  scale.set_scale(454.0); 
  scale.tare(); 
  pinMode(buzzerPin, OUTPUT); 
  
 
  serialSIM800.begin(9600);
 
  delay(5000);
  
  serialSIM800.println("AT");
  
  delay(3000);
  
}

void loop() {
  
  a = (int) scale.get_units();

  Serial.print("Weight: ");
  Serial.print(a);
  Serial.print(" g, Counter: ");
  Serial.println(b);

  if (a > 1 && a < 250) { 
    b++;
  } else {
    b = 0; 
  }

  if (b % 5 == 0 && b > 0) { 
    
    serialSIM800.println("AT+CMGF=1\r"); 
    delay(1000);
    serialSIM800.println("AT+CMGS=\"+919361406696\"\r"); 
    delay(1000);
    serialSIM800.print("Low saline level "); 
    serialSIM800.print(a);
    serialSIM800.print(" g.");
    delay(1000);
    serialSIM800.write((char)26); 
    Serial.println("SMS Sent!");

    delay(5000); 

  }
  
  if (b > 5) { // Check if the counter is greater than 5
    for (int i = 0; i < 5; i++) { // Play a pattern of five tones
      tone(buzzerPin, 1000 + i * 200); // Play a tone at increasing frequency
      delay(50); // Wait for 50 milliseconds before playing the next tone
      noTone(buzzerPin); // Turn off the tone
      delay(50); // Wait for 50 milliseconds before playing the next tone
    }
  } else {
    noTone(buzzerPin); // Turn off the tone
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
  }

  if (b == 0) { // Check if the counter is equal to 0
    Serial.println("Counter reset."); // Print a message indicating that the counter has been reset
  }

  delay(1000); // Wait for 1 second before taking another reading
}