#include "Sim_Engine.h"
#include "Host_A.h"
#include <string.h> //Tillagd
#include <stdbool.h>

#define TT 1000

int A = 0,queLen = 0;
struct pkt packetQue[100];
bool timerOn = false, ackNumber = 0, seqNumber = 0;

/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  int sumLen = 0;
  struct pkt pkt2give;
  ackNumber = !ackNumber;
  seqNumber = !seqNumber;

  pkt2give.acknum = ackNumber;
  pkt2give.seqnum = seqNumber;

  strcpy(pkt2give.payload, message.data);

  for (int i = 0; pkt2give.payload[i] != '\0'; i++)
  {
    sumLen += pkt2give.payload[i];
  }
  sumLen += pkt2give.seqnum;
  sumLen += pkt2give.acknum;

  pkt2give.checksum = sumLen;

  enqueue(pkt2give);

  if (!timerOn)
  {
    toB(A, pkt2give);
  }

}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  /* TODO */
  if (timerOn)
  {
    stoptimer(A);
    timerOn = false;
  }

  int sumLen = 0;
  for (int i = 0; packet.payload[i] != '\0'; i++)
  {
    sumLen += packet.payload[i];
  }
  sumLen += packet.seqnum;
  sumLen += packet.acknum;

  if (packet.acknum != packet.seqnum || sumLen != packet.checksum)
  {
    toB(A, packetQue[0]);
  }else{
    dequeue();
    if(queLen!=0){
      toB(A, packetQue[0]);
    }
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt()
{
  toB(A, packetQue[0]);
}

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init()
{
  /* TODO */
}

void toB(int AorB, struct pkt packet)
{
  timerOn = true;
  tolayer3(AorB, packet);
  starttimer(A, TT);
  
}

void enqueue(struct pkt packet)
{
  packetQue[queLen] = packet;
  queLen++;
}

void dequeue()
{
  for(int i=0; i<queLen-1; i++){
    packetQue[i]=packetQue[i+1];
  }
  queLen--;
}