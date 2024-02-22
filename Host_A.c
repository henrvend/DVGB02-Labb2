#include "Sim_Engine.h"
#include <string.h> //Tillagd
#include <stdbool.h>

#define TT 1000
int A = 0;
bool ackNumber = 0, seqNumber = 0;

/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  int x, sumLen = 0;
  ackNumber = !ackNumber;
  seqNumber = !seqNumber;
  struct pkt pkt2give;

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

  tolayer3(A, pkt2give);
  starttimer(A, TT);
  printf("Inkommit output\n");

  /* TODO */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  /* TODO */
  stoptimer(A);
  int sumLen = 0;
  for (int i = 0; packet.payload[i] != '\0'; i++)
  {
    sumLen += packet.payload[i];
  }
  sumLen += packet.seqnum;
  sumLen += packet.acknum;

  if (packet.acknum != packet.seqnum || sumLen != packet.checksum)
  {
    printf("skicka på nytt\n");
  }

  printf("Inkommit input\n");
}

/* Called when A's timer goes off */
void A_timerinterrupt()
{
  /* TODO */
  printf("Inkommit timer");
}

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init()
{
  /* TODO */
}
