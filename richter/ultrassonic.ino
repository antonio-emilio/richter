
Ultrasonic ultrasonic1(PORTA_TRIGGER1, PORTA_ECHO1);
Ultrasonic ultrasonic2(PORTA_TRIGGER2, PORTA_ECHO2);
Ultrasonic ultrasonic3(PORTA_TRIGGER3, PORTA_ECHO3);
Ultrasonic ultrasonic4(PORTA_TRIGGER4, PORTA_ECHO4);



int getDistanceLeft()
{
    int distanciaCM;
    long microsec = ultrasonic1.timing();
    distanciaCM = ultrasonic1.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}

int getDistanceRight()
{
    int distanciaCM;
    long microsec = ultrasonic2.timing();
    distanciaCM = ultrasonic2.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}



int getDistanceFront()
{
    int distanciaCM;
    long microsec = ultrasonic3.timing();
    distanciaCM = ultrasonic3.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}

int getDistanceBack()
{
    int distanciaCM;
    long microsec = ultrasonic4.timing();
    distanciaCM = ultrasonic4.convert(microsec, Ultrasonic::CM);

    return distanciaCM;

}
