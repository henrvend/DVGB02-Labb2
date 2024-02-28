#include "Sim_Engine.h"
#include "Host_A.h"
#include <string.h>
#include <stdbool.h>

#define TT 1000

int A = 0, queLen = 0;
struct pkt packetQue[100];
bool timerOn = false, ackNumber = 0, seqNumber = 0;

/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{

  struct pkt pkt2give;
  ackNumber = !ackNumber;
  seqNumber = !seqNumber;

  pkt2give.acknum = ackNumber;
  pkt2give.seqnum = seqNumber;

  strcpy(pkt2give.payload, message.data);
  // Copies incomming message to stucts payload.

  pkt2give.checksum = sumLength(pkt2give);
  // Calls function that sums pkt2give and gives the value to .checksum.

  enqueue(pkt2give);

  if (!timerOn)
  {
    toB(A, pkt2give);
  }
  // If the timer is off then calls on function "toB" that sends the package to B.
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  if (timerOn)
  {
    stoptimer(A);
    timerOn = false;
  }

  if (packet.acknum != packet.seqnum || sumLength(packet) != packet.checksum)
  {
    toB(A, packetQue[0]);
  }
  // Controls if acksum and seqnum are different and that the total sumLenght is different from checksum. Then sends tries to send the package again.
  else
  {
    dequeue();
    if (queLen != 0)
    {
      toB(A, packetQue[0]);
    }
    // Dequeue the array with package and sends the package that now is in place 0. Also checks if there is anything in the que.
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt()
{
  toB(A, packetQue[0]);
}
// The timer runs out and the packges is sent to B again.

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init()
{
  /* TODO */
}

/*Send packet to layer 3*/
void toB(int AorB, struct pkt packet)
{
  timerOn = true;
  tolayer3(AorB, packet);
  starttimer(A, TT);
}

// Adds package to que and also adds to varible that counts how many packages that is current in the que.
void enqueue(struct pkt packet)
{
  packetQue[queLen] = packet;
  queLen++;
}

/*Moves  package 1 to the begining of the que, this happens when the current first package in que is finished.
Also changes the value of "quelen" to signal how many packages that is left in que.
*/
void dequeue()
{
  for (int i = 0; i < queLen - 1; i++)
  {
    packetQue[i] = packetQue[i + 1];
  }
  queLen--;
}

/* Check total size of packet*/
// sumLength is needed two times in the file so a function was created to return sumLength to those places.
int sumLength(struct pkt packet)
{

  int sumLen = 0;
  for (int i = 0; packet.payload[i] != '\0'; i++)
  {
    sumLen += packet.payload[i];
  }
  sumLen += packet.seqnum;
  sumLen += packet.acknum;

  return sumLen;
}
