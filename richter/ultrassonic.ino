/*Declarations.*/
Ultrasonic ultrasonic1(PORTA_TRIGGER1, PORTA_ECHO1);
Ultrasonic ultrasonic2(PORTA_TRIGGER2, PORTA_ECHO2);
Ultrasonic ultrasonic3(PORTA_TRIGGER3, PORTA_ECHO3);
Ultrasonic ultrasonic4(PORTA_TRIGGER4, PORTA_ECHO4);


/*
Return the distance to the left direction.
*/
int getDistanceLeft()
{
    int distanciaCM;
    long microsec = ultrasonic1.timing();
    distanciaCM = ultrasonic1.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}
/*
Return the distance to the right direction.
*/
int getDistanceRight()
{
    int distanciaCM;
    long microsec = ultrasonic2.timing();
    distanciaCM = ultrasonic2.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}

/*
Return the distance to the front direction.
*/
int getDistanceFront()
{
    int distanciaCM;
    long microsec = ultrasonic3.timing();
    distanciaCM = ultrasonic3.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}

/*
Return the distance to the back direction.
*/
int getDistanceBack()
{
    int distanciaCM;
    long microsec = ultrasonic4.timing();
    distanciaCM = ultrasonic4.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}

/*Verify all sensors*/
int checkPerimeter(){
  int distanceBack = getDistanceBack();
  int distanceFront = getDistanceFront();
  int distanceRight = getDistanceRight();
  int distanceLeft = getDistanceLeft();

  if (distanceBack < 20)
    avoidBack = 1;

  if (distanceFront < 20)
    avoidFront = 1;

  if (distanceRight < 20)
    avoidRight = 1;

  if (distanceLeft < 20)
    avoidLeft = 1;

  return SUCCESFULL;
}
