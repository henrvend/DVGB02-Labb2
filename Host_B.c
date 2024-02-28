#include "Sim_Engine.h"
#include <stdbool.h>

int B = 1;
bool seqNum = 1;

/* Called from layer 5, passed the data to be sent to other side */
void B_output(struct msg message)
{
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet)
{
  int sumLen = 0;

  /*Checks the size of total packet and put it in sumLen*/
  for (int i = 0; packet.payload[i] != '\0'; i++)
  {
    sumLen += packet.payload[i];
  }
  sumLen += packet.seqnum;
  sumLen += packet.acknum;

  // Counts the total sum length to be able to later check if the whole package was sent correct.
  if (sumLen != packet.checksum)
  {
    packet.seqnum = !seqNum;
    packet.acknum = !packet.acknum;
    packet.checksum = sumLen;

    tolayer3(B, packet);
    return;
  }
  else if (packet.seqnum == seqNum)
  {
    // If seqnum is correct then sends package to layer 5 before it gets sent to layer 3.
    seqNum = !seqNum;
    tolayer5(0, packet.payload);
  }
  tolayer3(B, packet);
}

/* Called when B's timer goes off */
void B_timerinterrupt()
{
  /* TODO */
}

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init()
{
  /* TODO */
}
