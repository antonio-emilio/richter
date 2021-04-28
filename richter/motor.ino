
/*
Control the left motor with binary commands (Full speed forward, full speed backward, full stopped.)
*/
void LeftMotorBinary(String action){
  if (action.equals("FOWARD")){
    Serial.println("Moving Forward");
    digitalWrite(motor1Pin1Left, LOW);
    digitalWrite(motor1Pin2Left, HIGH);
  } else if (action.equals("BACKWARD")){
    Serial.println("Moving Backwards");
    digitalWrite(motor1Pin1Left, HIGH);
    digitalWrite(motor1Pin2Left, LOW); 
  } else {
    Serial.println("Motor stopped");
    digitalWrite(motor1Pin1Left, LOW);
    digitalWrite(motor1Pin2Left, LOW);
  }
}

/*
Control the right motor with binary commands (Full speed forward, full speed backward, full stopped.)
*/
void RightMotorBinary(String action){
  if (action.equals("FOWARD")){
    Serial.println("Moving Forward");
    digitalWrite(motor2Pin1Right, LOW);
    digitalWrite(motor2Pin2Right, HIGH);
  } else if (action.equals("BACKWARD")){
    Serial.println("Moving Backwards");
    digitalWrite(motor2Pin1Right, HIGH);
    digitalWrite(motor2Pin2Right, LOW); 
  } else {
    Serial.println("Motor stopped");
    digitalWrite(motor2Pin1Right, LOW);
    digitalWrite(motor2Pin2Right, LOW);
  }
}


/*
Control the left motor with PWM commands.
*/
void LeftMotorPWM(String action, int dutyCycle, int delayTime){
  if (action.equals("FOWARD")){
    Serial.print("Forward with duty cycle: ");
    digitalWrite(motor1Pin1Left, LOW);
    digitalWrite(motor1Pin2Left, HIGH);
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      delay(delayTime);
    }
    
  } else if (action.equals("BACKWARD")){
    digitalWrite(motor1Pin1Left, HIGH);
    digitalWrite(motor1Pin2Left, LOW); 
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      delay(delayTime);
    }
  }
}

/*
Control the right motor with PWM commands.
*/
void RightMotorPWM(String action, int dutyCycle, int delayTime){
  if (action.equals("FOWARD")){
    Serial.print("Forward with duty cycle: ");
    digitalWrite(motor2Pin1Right, LOW);
    digitalWrite(motor2Pin2Right, HIGH);
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel2, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      delay(delayTime);
    }
    
  } else if (action.equals("BACKWARD")){
    digitalWrite(motor2Pin1Right, HIGH);
    digitalWrite(motor2Pin2Right, LOW); 
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel2, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      delay(delayTime);
    }
  }
}
