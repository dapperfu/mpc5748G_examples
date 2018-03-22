/* edma.c - DMA example TCD initialization */
/* Rev 1.0 09 Apr 2015 S Mihalik - Initial version */
/* Description: TCD 0: Transfers string from RAM to RAM */
/*              TCD 1: Transfers alternate 0's/1's to I/O port pin */

#include "project.h" 
#include "edma.h"

uint8_t TCD0_SourceData[] = {"Hello World\n"};
#pragma alignvar (16)          /* Align for cache inhibit variable with SMPU */
uint8_t TCD0_Destination[13]; 
#pragma alignvar (16)

uint8_t TCD1_SourceData[] = {0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0,0x1,0x0}; 

void initTCDs(void) {                 /* Transfer string to port pin output */

  EDMA.TCD[0].SADDR.R = (vuint32_t) &TCD0_SourceData;  /* Load source address*/
  EDMA.TCD[0].ATTR.B.SSIZE = 0;           /* Read 2**0 = 1 byte per transfer */
  EDMA.TCD[0].ATTR.B.SMOD = 0;            /* Source modulo feature not used */
  EDMA.TCD[0].SOFF.R = 1;                 /* After transfer add 1 to src addr*/
  EDMA.TCD[0].SLAST.R = -12;              /* After major loop, reset src addr*/

  EDMA.TCD[0].DADDR.R = (vuint32_t) &TCD0_Destination ; /* Load dest. address*/
  EDMA.TCD[0].ATTR.B.DSIZE = 0;           /* Write 2**0 = 1 byte per transfer*/
  EDMA.TCD[0].ATTR.B.DMOD = 0;            /* Dest. modulo feature not used */
  EDMA.TCD[0].DOFF.R = 1;                 /* After transfer add 1 to dst addr*/
  EDMA.TCD[0].DLASTSGA.R = 0;             /* After major loop no dest addr change*/
   
  EDMA.TCD[0].NBYTES.MLNO.R = 1;          /* Transfer 1 byte per minor loop */
  EDMA.TCD[0].BITER.ELINKNO.B.ELINK = 0;  /* No Enabling channel LINKing */
  EDMA.TCD[0].BITER.ELINKNO.B.BITER = 13; /* 13 minor loop iterations */
  EDMA.TCD[0].CITER.ELINKNO.B.ELINK = 0;  /* No Enabling channel LINKing */
  EDMA.TCD[0].CITER.ELINKNO.B.CITER = 13; /* Init. current iteraction count */

  EDMA.TCD[0].CSR.B.DREQ = 1;        /* Disable channel when major loop is done*/
  EDMA.TCD[0].CSR.B.INTHALF = 0;     /* No interrupt when major count half complete */
  EDMA.TCD[0].CSR.B.INTMAJOR = 0;    /* No interrupt when major count completes */
  EDMA.TCD[0].CSR.B.MAJORELINK = 0;  /* Dynamic program is not used */
  EDMA.TCD[0].CSR.B.MAJORLINKCH = 0; /* No link channel # used */
  EDMA.TCD[0].CSR.B.ESG = 0;         /* Scatter Gather not Enabled */
  EDMA.TCD[0].CSR.B.BWC = 0;         /* Default bandwidth control- no stalls */
  EDMA.TCD[0].CSR.B.START = 0;       /* Initialize status flags START, DONE, ACTIVE */
  EDMA.TCD[0].CSR.B.DONE = 0;
  EDMA.TCD[0].CSR.B.ACTIVE = 0;


  EDMA.TCD[1].SADDR.R = (vuint32_t) &TCD1_SourceData;  /* Load source address*/
  EDMA.TCD[1].ATTR.B.SSIZE = 0;           /* Read 2**0 = 1 byte per transfer */
  EDMA.TCD[1].ATTR.B.SMOD = 0;            /* Source modulo feature not used */
  EDMA.TCD[1].SOFF.R = 1;                 /* After transfer add 1 to src addr*/
  EDMA.TCD[1].SLAST.R = -12;              /* After major loop, reset src addr*/ 

  EDMA.TCD[1].DADDR.R = (vuint32_t) &SIUL2.GPDO[PA10].R ; /* Destination address pin port 10*/
  EDMA.TCD[1].ATTR.B.DSIZE = 0;           /* Write 2**0 = 1 byte per transfer*/
  EDMA.TCD[1].ATTR.B.DMOD = 0;            /* Dest. modulo feature not used */
  EDMA.TCD[1].DOFF.R = 0;                 /* After transfer add 0 to dst addr*/
  EDMA.TCD[1].DLASTSGA.R = 0;             /* After major loop no dest addr change*/
   
  EDMA.TCD[1].NBYTES.MLNO.R = 1;          /* Transfer 1 byte per minor loop */
  EDMA.TCD[1].BITER.ELINKNO.B.ELINK = 0;  /* No Enabling channel LINKing */
  EDMA.TCD[1].BITER.ELINKNO.B.BITER = 12; /* 12 minor loop iterations */
  EDMA.TCD[1].CITER.ELINKNO.B.ELINK = 0;  /* No Enabling channel LINKing */
  EDMA.TCD[1].CITER.ELINKNO.B.CITER = 12; /* Init. current iteraction count */

  EDMA.TCD[1].CSR.B.DREQ = 1;        /* Disable channel when major loop is done*/
  EDMA.TCD[1].CSR.B.INTHALF = 0;     /* No interrupt when major count half complete */
  EDMA.TCD[1].CSR.B.INTMAJOR = 0;    /* No interrupt when major count completes */
  EDMA.TCD[1].CSR.B.MAJORELINK = 0;  /* Dynamic program is not used */
  EDMA.TCD[1].CSR.B.MAJORLINKCH = 0; /* No link channel # used */
  EDMA.TCD[1].CSR.B.ESG = 0;         /* Scatter Gather not Enabled */
  EDMA.TCD[1].CSR.B.BWC = 0;         /* Default bandwidth control- no stalls */
  EDMA.TCD[1].CSR.B.START = 0;       /* Initialize status flags START, DONE, ACTIVE */
  EDMA.TCD[1].CSR.B.DONE = 0;
  EDMA.TCD[1].CSR.B.ACTIVE = 0;
}

void init_edma_channel_arbitration (void) { /* Use default fixed arbitration */
  
  EDMA.CR.R = 0x0000E400; /* Fixed priority arbitration for groups, channels */
  
  EDMA.DCHPRI[0].R  = 0x00; /* Grp 0 chan 00, no suspension, no premption */ 
  EDMA.DCHPRI[1].R  = 0x01; /* Grp 0 chan 01, no suspension, no premption */ 
  EDMA.DCHPRI[2].R  = 0x02; /* Grp 0 chan 02, no suspension, no premption */ 
  EDMA.DCHPRI[3].R  = 0x03; /* Grp 0 chan 03, no suspension, no premption */ 
  EDMA.DCHPRI[4].R  = 0x04; /* Grp 0 chan 04, no suspension, no premption */ 
  EDMA.DCHPRI[5].R  = 0x05; /* Grp 0 chan 05, no suspension, no premption */ 
  EDMA.DCHPRI[6].R  = 0x06; /* Grp 0 chan 06, no suspension, no premption */ 
  EDMA.DCHPRI[7].R  = 0x07; /* Grp 0 chan 07, no suspension, no premption */ 
  EDMA.DCHPRI[8].R  = 0x08; /* Grp 0 chan 08, no suspension, no premption */ 
  EDMA.DCHPRI[9].R  = 0x09; /* Grp 0 chan 09, no suspension, no premption */ 
  EDMA.DCHPRI[10].R = 0x0A; /* Grp 0 chan 10, no suspension, no premption */ 
  EDMA.DCHPRI[11].R = 0x0B; /* Grp 0 chan 11, no suspension, no premption */ 
  EDMA.DCHPRI[12].R = 0x0C; /* Grp 0 chan 12, no suspension, no premption */ 
  EDMA.DCHPRI[13].R = 0x0D; /* Grp 0 chan 13, no suspension, no premption */ 
  EDMA.DCHPRI[14].R = 0x0E; /* Grp 0 chan 14, no suspension, no premption */ 
  EDMA.DCHPRI[15].R = 0x0F; /* Grp 0 chan 15, no suspension, no premption */
  
  EDMA.DCHPRI[16].R = 0x10; /* Grp 1 chan 00, no suspension, no premption */ 
  EDMA.DCHPRI[17].R = 0x11; /* Grp 1 chan 01, no suspension, no premption */ 
  EDMA.DCHPRI[18].R = 0x12; /* Grp 1 chan 02, no suspension, no premption */ 
  EDMA.DCHPRI[19].R = 0x13; /* Grp 1 chan 03, no suspension, no premption */ 
  EDMA.DCHPRI[20].R = 0x14; /* Grp 1 chan 04, no suspension, no premption */ 
  EDMA.DCHPRI[21].R = 0x15; /* Grp 1 chan 05, no suspension, no premption */ 
  EDMA.DCHPRI[22].R = 0x16; /* Grp 1 chan 06, no suspension, no premption */ 
  EDMA.DCHPRI[23].R = 0x17; /* Grp 1 chan 07, no suspension, no premption */ 
  EDMA.DCHPRI[24].R = 0x18; /* Grp 1 chan 08, no suspension, no premption */ 
  EDMA.DCHPRI[25].R = 0x19; /* Grp 1 chan 09, no suspension, no premption */ 
  EDMA.DCHPRI[26].R = 0x1A; /* Grp 1 chan 10, no suspension, no premption */ 
  EDMA.DCHPRI[27].R = 0x1B; /* Grp 1 chan 11, no suspension, no premption */ 
  EDMA.DCHPRI[28].R = 0x1C; /* Grp 1 chan 12, no suspension, no premption */ 
  EDMA.DCHPRI[29].R = 0x1D; /* Grp 1 chan 13, no suspension, no premption */ 
  EDMA.DCHPRI[30].R = 0x1E; /* Grp 1 chan 14, no suspension, no premption */ 
  EDMA.DCHPRI[31].R = 0x1F; /* Grp 1 chan 15, no suspension, no premption */ 
}
