#define dioda_board 13

int diodaStatus = LOW;
int jasnosc = 0;
char inData[3];  // space for the String
char inChar = -1; // where to store the character read
byte index = 0;   // index into array
long randNumber;
long l = 2000000000;

char r[3];
byte b[2] = {82, 192};


void setup() {
  Serial.begin(9600);
  
  pinMode(dioda_board, OUTPUT);
  randNumber = random(l);
  Rotate(b, r, 2);
  r[3] = '\0';
}

char Comp(char* This) {
 while(Serial.available() > 0) {
  if(index < 3 /*&& Serial.read() != '\n'*/) {
   inChar = Serial.read();  // read a character
   if(inChar != '\n') {
     inData[index] = inChar;  // store it
     index++;                 // increment
     inData[index] = '\0';    // null terminate the string 
     //Serial.print("index: ");
     //Serial.println(index);
   } else {
     //Serial.print("\\n index: ");
     //Serial.println(index);
    //inData[index] = '\0';
    //index++;
   }
  }
 }

 if(strcmp(inData, This) == 0) {
  for(int i = 0; i < 3; i++) {
   inData[i] = 0; 
  }
  index = 0;
  return(0);
 } else {
  return(1);
  
 }
}

void Rotate(byte* bytes, char* chars, unsigned int count){
    for(unsigned int i = 0; i < count; i++)
    	 chars[i] = (char)bytes[i];
}

void loop() {  
  if(Comp("11") == 0) {
    digitalWrite(dioda_board, HIGH);
    Serial.print("on");
  } else if(Comp("00") == 0) {
    digitalWrite(dioda_board, LOW);
    Serial.print("off");
  } else if (Comp("S") == 0) {
    randNumber = random(l);
    Serial.print(randNumber);
  } else if (Comp("L@") == 0) {
    Serial.print("L@");
  } else if(Comp(r) == 0) {
    
  }
  
  /*if(Comp("H") == 0 || Comp("h") == 0 || Comp("1") == 0) {
    digitalWrite(dioda_pin, HIGH);
    digitalWrite(dioda_board, HIGH);
    Serial.print("on");
  } else if ( Comp("l") == 0 || Comp("0") == 0) {
    digitalWrite(dioda_pin, LOW);
    digitalWrite(dioda_board, LOW);
    Serial.print("off");
  } else if ( Comp("S") == 0) {
    randNumber = random(l);
    Serial.print(randNumber);
  } else if (Comp("L@") == 0) {
    Serial.print("L@");
  } else if (Comp("R@") == 0) {
    Serial.print("R@");
  }
  */
  
}
