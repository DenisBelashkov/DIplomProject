int received = 0;

void setup() {
  Serial.begin(9600); 
}

void loop() {

  if (Serial.available() > 0) {
  received = Serial.read();
  Serial.print('(');  
  Serial.print(received);
  Serial.println(')');

}
