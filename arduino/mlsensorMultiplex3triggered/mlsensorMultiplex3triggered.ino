

// pins
int sig = 1;
int s0 = 2;
int s1 = 3;
int s2 = 4;
int s3 = 5;

// Global counter for cycling through the select lines using interrupts.
int counter = 0;

int val = 0; 
double read1 = 0;
double read2 = 0;
double read3 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
 
}




void loop() {
 if (Serial.available())
  { 
    counter = Serial.parseInt();
        if (1 == counter)
        {
          // 0000
          digitalWrite(s0, LOW);
          digitalWrite(s1, LOW);
          digitalWrite(s2, LOW);
          digitalWrite(s3, LOW);
          //counter = 1;
        //  Serial.println( counter);
        }
        else if (2 == counter)
        {
          // 0001
          digitalWrite(s0, HIGH);
          digitalWrite(s1, LOW);
          digitalWrite(s2, LOW);
          digitalWrite(s3, LOW);
         // counter = 2;
         // Serial.println( counter);
        }
        else if (3 == counter)
        {
          // 0010
          digitalWrite(s0, LOW);
          digitalWrite(s1, HIGH);
          digitalWrite(s2, LOW);
          digitalWrite(s3, LOW);
         // counter = 0;
         // Serial.println( counter);
        }
  }
}
