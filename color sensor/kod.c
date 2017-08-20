#include <FreqCount.h>

// połączenia
const int S3 = 13;
const int S2 = 12;
const int Fo = 5;

// OE -> GND
// S0 -> 5V
// S1 -> GND


enum {RED=0, GREEN, BLUE, CLEAR} color=RED;
long results[4]={0,0,0,0};
long max_results[4]={0,0,0,0};
long min_results[4]={0xFFFFFFFF/2,0xFFFFFFFF/2,0xFFFFFFFF/2,0xFFFFFFFF/2};
// aby poprawnie ustalać wartość min i max musimy odpowiedno zainicjalizować zmienne

byte calibration = 1;


void setup() {
  Serial.begin(57600);
  pinMode(Fo, INPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  FreqCount.begin(100);
}

void loop() {  
  // jeśli odebraliśmy znak z UART to sprawdzamy, czy to nie znak zakończenia kalibracji
  if(Serial.available()){
    char c = Serial.read();
    if(c=='x'){//jeśli tak to wyświetlamy dane kalibracji
      calibration=0;
      Serial.println("Calibration disabled");
       Serial.print("RL: ");
       Serial.print(min_results[RED]);
       Serial.print("  RH: ");
       Serial.println(max_results[RED]);
       Serial.print("GL: ");
       Serial.print(min_results[GREEN]);
       Serial.print("  GH: ");
       Serial.println(max_results[GREEN]);
       Serial.print("BL: ");
       Serial.print(min_results[BLUE]);
       Serial.print("  BH: ");
       Serial.println(max_results[BLUE]);
       Serial.print("CL: ");
       Serial.print(min_results[CLEAR]);
       Serial.print("  CH: ");
       Serial.println(max_results[CLEAR]);
       delay(5000);
    }
  }
  // jeśli zakończono pomiar częstotliwości (liczenie impulsów w zadanym czasie)
  if (FreqCount.available()) {
    unsigned long count = FreqCount.read();
    results[color]=count;//zapisujemy ich ilośc (odpowiadającą częstotliwości)
	
	// jeśli kalibracja jest aktywna to zapisujemy minimalną lub maksymalną wartość odczytu z tego kanału
    if(calibration){
      if(results[color]>max_results[color])max_results[color]=results[color];
      if(results[color]<min_results[color])min_results[color]=results[color];
    }
	// blokujemy pomiar, w czelu zresetowania systemu liczenia
    FreqCount.end();
	// w zależności od aktualnego koloru ustawiamy kolejny kolor do sprawdzenia i odpowiednią kombinację S2..3
    switch(color){
      case RED:
        color=GREEN;
        digitalWrite(S2, HIGH);
        digitalWrite(S3, HIGH);
      break;
    
      case GREEN:
        color=BLUE;
        digitalWrite(S2, LOW);
        digitalWrite(S3, HIGH);
      break;
      
      case BLUE:
        color=CLEAR;
        digitalWrite(S2, HIGH);
        digitalWrite(S3, LOW);
      break;
    
      case CLEAR:
        color=RED;
        digitalWrite(S2, LOW);
        digitalWrite(S3, LOW);
		
		// jeden raz na cały cykl pomiarowy wyświetlamy zebrane dane - najpierw w formacie nieprzetworzonym...
        Serial.print("R: ");
        Serial.print(results[RED]);
        Serial.print(" G: ");
        Serial.print(results[GREEN]);
        Serial.print(" B: ");
        Serial.print(results[BLUE]);
        Serial.print(" C: ");
        Serial.print(results[CLEAR]);
        
		// a tu, korzystając z danych kalibracji, przeskalowujemy wnikiki do zakresu 0..100
        results[RED  ]=map(results[RED  ],min_results[RED  ],max_results[RED  ],0,100);
        results[GREEN]=map(results[GREEN],min_results[GREEN],max_results[GREEN],0,100);
        results[BLUE ]=map(results[BLUE ],min_results[BLUE ],max_results[BLUE ],0,100);
        results[CLEAR]=map(results[CLEAR],min_results[CLEAR],max_results[CLEAR],0,100);
        
		// i ponownie wyświetlamy "znormalizowane" dane
        Serial.print(" R: ");
        Serial.print(results[RED]);
        Serial.print(" G: ");
        Serial.print(results[GREEN]);
        Serial.print(" B: ");
        Serial.print(results[BLUE]);
        Serial.print(" C: ");
        Serial.println(results[CLEAR]);
      break;  
    }
	// na koniec uruchamiamy pomiar ponownie (po zmianie wyboru koloru) na 100ms (w tym czasie zostaną zliczone impulsy)
    FreqCount.begin(100);
  }
  
}
