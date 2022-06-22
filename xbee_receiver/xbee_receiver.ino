const int pin = 0; 

bool started= false;
bool ended  = false;
char incomingByte ;
char msg[3];  
byte index; 

void setup() {

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  
  while (Serial.available()>0){
    incomingByte = Serial.read();
    if(incomingByte == '(')
    {
     started = true;
     index = 0;
     msg[index] = '\0'; 
   }
   else if(incomingByte == ')')
   {
     ended = true;
   }

   else
   {
     if(index < 20)      {
       msg[index] = incomingByte; 
       index++;
       msg[index] = '\0';
     }
   }
 }
 
 if(started && ended)
 {
   int value = atoi(msg);
   analogWrite(pin, value);
   index = 0;
   msg[index] = '\0';
   started = false;
   ended = false;
 }

}
  
