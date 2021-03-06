
/*
Control the left motor with binary commands (Full speed forward, full speed backward, full stopped.)
*/
void LeftMotorBinary(String action){
  if (action.equals("FOWARD")){
    digitalWrite(motor1Pin1Left, LOW);
    digitalWrite(motor1Pin2Left, HIGH);
  } else if (action.equals("BACKWARD")){
    digitalWrite(motor1Pin1Left, HIGH);
    digitalWrite(motor1Pin2Left, LOW); 
  } else {
    digitalWrite(motor1Pin1Left, LOW);
    digitalWrite(motor1Pin2Left, LOW);
  }
}

/*
Control the right motor with binary commands (Full speed forward, full speed backward, full stopped.)
*/
void RightMotorBinary(String action){
  if (action.equals("FOWARD")){
    digitalWrite(motor2Pin1Right, LOW);
    digitalWrite(motor2Pin2Right, HIGH);
  } else if (action.equals("BACKWARD")){
    digitalWrite(motor2Pin1Right, HIGH);
    digitalWrite(motor2Pin2Right, LOW); 
  } else {
    digitalWrite(motor2Pin1Right, LOW);
    digitalWrite(motor2Pin2Right, LOW);
  }
}


/*
Control the left motor with PWM commands.
*/
void LeftMotorPWM(String action, int dutyCycle, int delayTime){
  if (action.equals("FOWARD")){
    digitalWrite(motor1Pin1Left, LOW);
    digitalWrite(motor1Pin2Left, HIGH);
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
    
  } else if (action.equals("BACKWARD")){
    digitalWrite(motor1Pin1Left, HIGH);
    digitalWrite(motor1Pin2Left, LOW); 
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
  }
}

/*
Control the right motor with PWM commands.
*/
void RightMotorPWM(String action, int dutyCycle, int delayTime){
  if (action.equals("FOWARD")){
    digitalWrite(motor2Pin1Right, LOW);
    digitalWrite(motor2Pin2Right, HIGH);
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel2, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
    
  } else if (action.equals("BACKWARD")){
    digitalWrite(motor2Pin1Right, HIGH);
    digitalWrite(motor2Pin2Right, LOW); 
    while (dutyCycle <= 255){
      ledcWrite(pwmChannel2, dutyCycle);   
      dutyCycle = dutyCycle + 5;
      vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
  }
}
