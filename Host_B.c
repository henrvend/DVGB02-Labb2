#include "Sim_Engine.h"

int B = 1;
/* Called from layer 5, passed the data to be sent to other side */
void B_output(struct msg message)
{
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet)
{
  /* TODO */
  int sumLen = 0;
  for (int i = 0; packet.payload[i] != '\0'; i++)
  {
    sumLen += packet.payload[i];
  }
  sumLen += packet.seqnum;
  sumLen += packet.acknum;

  if (sumLen != packet.checksum)
  {
    packet.seqnum = 1;
    packet.acknum = !packet.acknum;
    packet.checksum = sumLen;
    tolayer3(B, packet);
    return;
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
