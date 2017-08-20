int RedPin = 6;
int GreenPin = 10;
int BluePin = 9;

char cmd[10];
int cmdIndex;
int MAX_CMD_LENGTH = 10;
char incomingByte;
int prevSlider = 0;
int redVal, blueVal, greenVal;
boolean pwm;

void setup() {
 
 Serial.begin(9600);
 pinMode(RedPin, OUTPUT);
 pinMode(BluePin, OUTPUT);
 pinMode(GreenPin, OUTPUT);
 digitalWrite(RedPin, HIGH);
 digitalWrite(BluePin, HIGH);
 digitalWrite(GreenPin, HIGH);
 cmdIndex = 0;
}

void loop() {

  if(incomingByte = Serial.available() > 0) {
    char byteIn = Serial.read();
    cmd[cmdIndex] = byteIn;
    
    if(byteIn == '\n') {
      // end of command
      cmd[cmdIndex] = '\0';
      Serial.print("Command received: ");
      Serial.println(cmd);
      cmdIndex = 0;
      
      String stringCmd = String(cmd);
      if(strcmp(cmd, "LEDON") == 0) {
        Serial.println("Command: LEDON");
        digitalWrite(RedPin, LOW);
        digitalWrite(BluePin, LOW);
        digitalWrite(GreenPin, LOW);
      } else if(strcmp(cmd, "LEDOFF") == 0) {
        Serial.println("Command: LEDOFF");
        digitalWrite(RedPin, HIGH);
        digitalWrite(BluePin, HIGH);
        digitalWrite(GreenPin, HIGH);
        
      } else if(strcmp(cmd, "LEDPWM") == 0) {
        
        Serial.println("PWM");
        pwm = true;
        pwm_function();
        
      } else if(stringCmd.substring(0,4)="COL#") {
        Serial.println("Command: COL#");
        
        if(stringCmd.length() == 10) {
          char *pEnd;
          long int rgb = strtol(&cmd[4], &pEnd, 16);
          redVal = (rgb & 0xFF0000) >> 16;
          greenVal = (rgb & 0xFF00) >> 8;
          blueVal = rgb & 0xFF;
          Serial.print("rgb: ");
          Serial.println(rgb);
          Serial.print("r: ");
          Serial.println(redVal);
          Serial.print("g: ");
          Serial.println(greenVal);
          Serial.print("b: ");
          Serial.println(blueVal);
          
          analogWrite(RedPin, redVal);
          analogWrite(BluePin, blueVal);
          analogWrite(GreenPin, greenVal);
        }
      } else {
        Serial.println("Unknown command");
      }
    } else {
      if(cmdIndex++ >= MAX_CMD_LENGTH) {
        cmdIndex = 0;
      }
    }
  }
  
  
}

void pwm_function() {
  while(pwm) {
    redVal = 255;
    blueVal = 0;
    greenVal = 0;
    
    digitalWrite(RedPin, HIGH);
    digitalWrite(BluePin, HIGH);
    digitalWrite(GreenPin, HIGH);
  
    for(int i = 0; i < 255; i++) {
      if(Serial.available() > 0) {
        pwm = false;
        break;
      }
      greenVal++;
      redVal--;
      analogWrite(GreenPin, 255 - greenVal);
      analogWrite(RedPin, 255 - redVal);
      delay(20);
    }
  
    redVal = 0;
    blueVal = 0;
    greenVal = 255;
  
    for(int i = 0; i < 255; i++) {
      if(Serial.available() > 0) {
        pwm = false;
        break;
      }
      blueVal++;
      greenVal--;
      analogWrite(BluePin, 255 - blueVal);
      analogWrite(GreenPin, 255 - greenVal);
      delay(20);
    }
  
    redVal = 0;
    blueVal = 255;
    greenVal = 0;
  
    for(int i = 0; i < 255; i++) {
      if(Serial.available() > 0) {
        pwm = false;
        break;
      }
      redVal++;
      blueVal--;
      analogWrite(RedPin, 255 - redVal);
      analogWrite(BluePin, 255 - blueVal);
      delay(20);
    }
  }
  
  
}
