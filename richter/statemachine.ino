void SM_s1() {

  /*State machine selection*/
  switch (state_s1) {
    case STATE_0: 

      /*To be implemented.....*/ 
      r = SUCCESFULL;

      if (r == SUCCESFULL) {state_s1 = 1;} 
    break;

    case STATE_1: 

      /*To be implemented.....*/ 
      r = SUCCESFULL;

      if (r == SUCCESFULL) {state_s1 = 2;} 
    break;

    case STATE_2: 

      /*To be implemented.....*/ 
      r = SUCCESFULL;

      if (r == SUCCESFULL) {state_s1 = 0;} 
    break;

    default:
      /*Something went wrong?*/
      break;
  }

}
