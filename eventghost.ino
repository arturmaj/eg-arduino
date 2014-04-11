/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example RF Radio Ping Pair
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two different nodes,
 * connect the role_pin to ground on one.  The ping node sends the current time to the pong node,
 * which responds by sending the value back.  The ping node can then see how long the whole cycle
 * took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);


//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 
  0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
void setup(void)
{

  //
  // Print preamble
  //

  Serial.begin(9600);
  printf_begin();
  //printf("\n\rRF24/examples/pingpair/\n\r");

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(sizeof(int));
  radio.setPALevel(RF24_PA_HIGH);
  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)


  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  //radio.printDetails();
}
String readSerial() {
  String inData = "";
  if (Serial.available() > 0) {
    int h = Serial.available();
    for (int i = 0; i < h; i++) {
      inData += (char)Serial.read();
    }
    return inData;
  }
  else {
    return "Nop";
  }
}
void loop(void)
{
  radio.stopListening();
  String Comm=readSerial();
  //Serial.println(Comm);
  //Serial.println(int(Comm[0]));
  int i=int(Comm[1])-48;
  int i1=int(Comm[2])-48;
  if (Comm[0]==65){
    
    int Temp=i*10+i1;
    Serial.println(Temp);
    // Take the time, and send it.  This will block until complete
    radio.write( &Temp, sizeof(int) );
	radio.startListening();

	// Wait here until we get a response, or timeout (250ms)
	unsigned long started_waiting_at = millis();
	bool timeout = false;
	while (!radio.available() && !timeout)
	if (millis() - started_waiting_at > 500)
		timeout = true;

	// Describe the results
	if (timeout)
	{
		printf("Failed, response timed out.\n\r");
	}
	else
	{
		// Grab the response, compare, and send to debugging spew
		int th;
		radio.read(&th, sizeof(int));

		// Spew it
		if (i1 == 97){
			printf("Kominek.%u\n\r", th);
		}



  
  }

  // Now, continue listening

  }
  delay(200);
}




