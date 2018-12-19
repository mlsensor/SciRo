//Pin intialization
// motor 1
int  motor1_Enable_Pin = 43;
int  motor1_Direction_Pin = 44;
int  motor1_Clock_Pin = 45;
int  motor1_Back_LS = 51;
int  motor1_Forward_LS = 52;
int  pressure_Sensor1_signal = A1;

//inorout => 49 = pushing forwards
//inorout => 48 = pushing backwards
//inorout => 50 = stop
int inorout1 = 49;
int ini=0;

//preallocation for deley value for varying motor speed with PID
float delay_val1 = 2000;//micro

float delay_inp = 1000; //milli

// PID Parameters
int Kp = 10;
int Ki = 0.05;
int Kd = 30;

//PID tolerance
int integralThresh = 1;
float pressureThresh = 0.15;

//preallocation for integral values for PID
int integ1 = 0;


//set points for path following
//const int numPoints =35; // number of waypoints in the path.
const int numPoints = 1000;

//float setpt1[numPoints] = {2, 0,1.5,2.5,0.5,1,0,2.5,0,1.5,0.5,2.5,1.8,0.2};
//float setpt1[numPoints] = {0.2, 0,0.4,0.5,0.6,0.9,0.4,1.2,1.3,2.9,2.3,1.2,2.3,1.5,2.5,2.4,2.3,2.1,2.5,0,1.5,0,2.5,2,1.7,1.5,1,0.1,3.5,2.5,0.5,3.4,1.2,1.9,2.5};
//float setpt1[numPoints] = {0,1.5};

float setpt1[numPoints];

//preallocation for current pressure values
float curr1 = 0;


//preallocation for previous pressure values
float prev1 = 0;


//preallocation for error value between desired and current position
float err1 = 0;


//sum of all PID terms
float drive1 = 0;


//absolute value of drive variable
float drive1abs = 0;

//time passed since last step to be used for PID speed control
float timer1 = 0;


//compare timer with time_passed to evaluate elapsed time
float time_passed1 = 0;


//flag for reseting motor stage
int reset_motors = 0;

//preallocation for input value - when input is 1 from serial after reset motors, program will start
int run_stop = 0;

//pressure sensors parameters
int Vsupply = 5;
int Pmax = 150; //psi
int Pmin = 0;

//preallocation for elapsed time since all 3 units are at desired pressures
float SetPointTimer1 = 5000;

//preallocation for is unit at desired pressure +- Threshhold
int SetPointOn1 = 0;


//count for current set point number
int count1 = 0;


// counter for moving initial starting point to middle of syringe for enabling vacuum use.
int stepCount = 0;

int sens = 0;



void setup() {
  Serial.begin(115200);

  //Motor 1

  pinMode(motor1_Enable_Pin, OUTPUT);
  pinMode(motor1_Direction_Pin, OUTPUT);
  pinMode(motor1_Clock_Pin, OUTPUT);
  pinMode(motor1_Back_LS, INPUT);
  pinMode(motor1_Forward_LS, INPUT);

  digitalWrite(motor1_Enable_Pin, HIGH);


}




void loop() {


  //Reset motors
  //reset motor 1
  if (reset_motors == 0) {
    while (digitalRead(motor1_Back_LS) == LOW ) {
      if (digitalRead(motor1_Back_LS) == LOW) {
        digitalWrite(motor1_Enable_Pin, LOW);
        digitalWrite(motor1_Direction_Pin, HIGH);

        digitalWrite(motor1_Clock_Pin, HIGH);
        digitalWrite(motor1_Clock_Pin, LOW);
        delayMicroseconds(1000);
        //            Serial.print(digitalRead(51));
      }

    }

    // Run the motors forward each by 400 microsteps.
    while (stepCount < 300) {
      // Serial.println(stepCount);
      // Move motor 1 forward by 1 microstep.
      digitalWrite(motor1_Enable_Pin, LOW);
      digitalWrite(motor1_Direction_Pin, LOW);

      digitalWrite(motor1_Clock_Pin, HIGH);
      delayMicroseconds(1);
      digitalWrite(motor1_Clock_Pin, LOW);
      delayMicroseconds(1000);
      //            Serial.print(digitalRead(51));


      reset_motors = 1;
      stepCount++;
    }

  }

  do {
    if (Serial.available()) {

      setpt1[ini] = float(Serial.parseInt());
      setpt1[ini]=setpt1[ini]/10;
      Serial.println(ini);

      ini++;
    }

  }
  while (ini < numPoints);

  
// Serial.println(10);

  // Begin running upon reading a '1' over serial. 
  if (Serial.available())
  { // read the incoming byte:
    run_stop = Serial.read();
    run_stop=49;
    // Serial.println(run_stop);
  }


  while (run_stop == 49)
  {

    for (int jj = 0; jj < 10; jj++)
    { sens = sens + analogRead(pressure_Sensor1_signal);
    }
    sens = sens / 10;
    //float pressure1 = (analogRead(pressure_Sensor1_signal)/1024.0*5.0 - 0.10*Vsupply) * (Pmax - Pmin) / 0.8 / Vsupply + Pmin;
    float pressure1 = (sens / 1024.0 * 5.0 - 0.10 * Vsupply) * (Pmax - Pmin) / 0.8 / Vsupply + Pmin;
    sens = 0;

    // motor 1 PID
    curr1 = pressure1;
    err1 = setpt1[count1] - curr1;

    if (abs(err1) < integralThresh) {// prevent integral 'windup'.
      integ1 = integ1 + err1; // accumulate the error integral.
    }
    else {
      integ1 = 0; // zero if it's out of bounds.
    }

    float   P1 = err1 * Kp;
    float   I1 = integ1 * Ki;
    float   D1 = (prev1 - curr1) * Kd;

    drive1 = P1 + I1 + D1;



    if (0 < drive1) {
      inorout1 = 49;
    }
    else if (0 > drive1) {
      inorout1 = 48;
    }

    drive1abs = abs(drive1);

    prev1 = curr1;



    //Motor 1

    if (inorout1 == 48) {
      if (digitalRead(motor1_Back_LS) == HIGH) {
        inorout1 = 50;
        //Serial.print("Y");
      }
      else {
        digitalWrite(43, LOW);
        digitalWrite(44, HIGH);
      }
    }

    if (inorout1 == 49) {
      if (digitalRead(motor1_Forward_LS) == HIGH) {
        inorout1 = 50;
      }
      else {
        digitalWrite(motor1_Enable_Pin, LOW);
        digitalWrite(motor1_Direction_Pin, LOW);
      }
    }

    if (inorout1 == 50) {
      digitalWrite(motor1_Enable_Pin, HIGH);
    }





    if (millis() - SetPointTimer1 > delay_inp) {
      //   Serial.print(SetPointTimer1);
      SetPointOn1 = 0;
      count1++;
      if (count1 == numPoints) {
        count1 = 0;
      }
      SetPointTimer1 = millis();
    }


    // Delay and Run

    delay_val1 = map(drive1abs, 1, 500, 500, 1) / 500;


    // SetPointTimer1 = millis();

    //Motor 1
    if (setpt1[count1] + pressureThresh < pressure1 || setpt1[count1] - pressureThresh > pressure1) {
      time_passed1 = micros() - timer1;
      if (time_passed1 > delay_val1) {
        digitalWrite(motor1_Clock_Pin, HIGH);
        //  delayMicroseconds(1);
        digitalWrite(motor1_Clock_Pin, LOW);
        timer1 = micros();
      }
    }
    //    if (setpt1[count1] + pressureThresh > pressure1 && setpt1[count1] - pressureThresh < pressure1) {
    //      SetPointOn1 = 1;
    //    }



    if (Serial.available() > 0)
    { // read the incoming byte:
      if (Serial.read() == 50)
        run_stop = Serial.read();
    }

    //motor 3 check

    //    if (SetPointOn1 == 0 ) {
    //      SetPointTimer1 = millis(); //done by me
    //    }



    //
    //    Serial.print(pressure1);
    //    Serial.print(" ");
    //    Serial.print('\n');
  }
}

