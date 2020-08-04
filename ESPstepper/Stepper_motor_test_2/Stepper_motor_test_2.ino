// Run a A4998 Stepstick from an Arduino UNO.
// Paul Hurley Aug 2015
int x; 
#define BAUD (9600)


void setup() 
{
  Serial.begin(BAUD);
  pinMode(10,OUTPUT); // Enable
  pinMode(8,OUTPUT); // Step
  pinMode(9,OUTPUT); // Dir
  digitalWrite(10,LOW); // Set Enable low
}

void loop() 
{
  digitalWrite(10,LOW); // Set Enable low
  digitalWrite(9,HIGH); // Set Dir high
  Serial.println("Loop 200 steps (1 rev)");
  for(x = 0; x < 200; x++) // Loop 200 times
  {
    digitalWrite(8,HIGH); // Output high
    delay(10); // Wait
    digitalWrite(8,LOW); // Output low
    delay(100); // Wait
  }
  Serial.println("Pause");
  delay(1000); // pause one second
}
