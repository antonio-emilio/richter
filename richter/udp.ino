/*Init the UDP connection*/
void initUDP(){
  udp.begin( 8888 );
}

/*Send an UDP message.*/
int sendUDPmsg(String replyPacket){

    //send a broadcast message
    udp.beginPacket( { 192, 168, 4, 255 }, 8888 );
    int i = 0;
    while (replyPacket[i] != 0) 
      udp.write((uint8_t)replyPacket[i++]);
    
    if ( !udp.endPacket() ){
        Serial.println("[UDP] Message has not been sent.");
    }
    else{
        Serial.println("[UDP] Message has been sent.");
    }     
}

/*Check for any UDP messages*/
void checkUDPmsg(){
    packetSize = udp.parsePacket();
    if (packetSize)
    {
      // receive incoming UDP packets
      Serial.printf("[UDP] Received %d bytes from %s, port %d\n", packetSize, udp.remoteIP().toString().c_str(), udp.remotePort());
     len = udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("[UDP] UDP packet contents: %s\n", incomingPacket);
    }
    udp.flush();

}
