/*****************************************************************************/
/* FILE NAME: reg_protection_Z4_1.c   COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* PLATFORM: DEVKIT-MPC5748G												 */
/* DESCRIPTION: Main C program for core 0 to exercise register protection    */
/*				SOFT LOCK: This part writes the value to the mirror registers*/
/*				and locks the register. So attempt to modify original 		 */
/* 				register or mirror register will fail. 						 */
/*				CLEAR SOFT LOCK: Clears the soft-lock bit. So now 			 */
/*				it can modify register value								 */
/*				HARD LOCK: Locks the soft-lock bit by setting hard lock. 	 */
/*				So any attempt to modify original register, mirror register  */
/*				or soft lock bit will generate machine check interrupt.		 */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	 Mihalik/Obrien 02 Apr 2014   Initial Version                        */
/* 1.1   Mihalik        07 Jan 2015   Simplified last example and comments   */
/* 1.2   SM             27 Jan 2015   Added Machine Check comments           */
/*                                    & removed unrequired SIUL ME_PCTL code */
/* 1.3   K Shah			14 Mar 2016	  Ported to S32DS						 */
/*****************************************************************************/

#include "derivative.h" /* include peripheral declarations */
#include "mode.h"
#include "project.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);

void hw_init(void)
{
#if defined(DEBUG_SECONDARY_CORES)
	uint32_t mctl = MC_ME.MCTL.R;
#if defined(TURN_ON_CPU1)
	/* enable core 1 in all modes */
	MC_ME.CCTL[2].R = 0x00FE;
	/* Set Start address for core 1: Will reset and start */
	MC_ME.CADDR[2].R = 0x11d0000 | 0x1;
#endif	
#if defined(TURN_ON_CPU2)
	/* enable core 2 in all modes */
	MC_ME.CCTL[3].R = 0x00FE;
	/* Set Start address for core 2: Will reset and start */
	MC_ME.CADDR[3].R = 0x13a0000 | 0x1;
#endif
	MC_ME.MCTL.R = (mctl & 0xffff0000ul) | KEY_VALUE1;
	MC_ME.MCTL.R =  mctl; /* key value 2 always from MCTL */
#endif /* defined(DEBUG_SECONDARY_CORES) */
}

__attribute__ ((section(".text")))

/****************** Additional Headers for Register Protection ***************/
struct SIUL2_GFCR_tag {
  union {                              /* Global Configuration Register */
    vuint32_t R;
    struct {
      vuint32_t HLB:1;                 /* Hard Lock Bit */
      vuint32_t  :7;
      vuint32_t UAA:1;                 /* User Access Allowed. */
      vuint32_t  :23;
    } B;
  } GCR;
};

struct SIUL2_SSLB_tag {
  union {                              /* Set Soft Lock Bit Registers */
    vuint8_t R;
    struct {
      vuint8_t WE0:1;                  /* Write Enable Bits */
      vuint8_t WE1:1;
      vuint8_t WE2:1;
      vuint8_t WE3:1;
      vuint8_t SLB0:1;                 /* Soft Lock Bits */
      vuint8_t SLB1:1;
      vuint8_t SLB2:1;
      vuint8_t SLB3:1;
    } B;
  } SSLB[1535];
};

#define SIUL2_REGLOCK     (*(volatile struct SIUL2_tag *)        0xFFFC2000UL)
#define SIUL2_LOCKBITS    (*(volatile struct SIUL2_SSLB_tag *)   0xFFFC3800UL)
#define SIUL2_GLOBALLOCK  (*(volatile struct SIUL2_GFCR_tag *)   0xFFFC3FFCUL)

/************************************ Main ***********************************/

int main(void)
{

  uint32_t i = 0;
  uint32_t __attribute__ ((unused)) LockedRegisterUnchanged  = 0;  /* test result */
  uint32_t __attribute__ ((unused)) UnlockedRegisterChanged  = 0;  /* test result */
  uint32_t __attribute__ ((unused)) RegisterLockMaintained   = 0;  /* test result */

  xcptn_xmpl ();         /* Configure and Enable Interrupts */

  system160mhz();        /* sysclk=160MHz, dividers configured, mode trans*/

  /******************************************************/
  /*  SOFT LOCK                                         */
  /******************************************************/

  SIUL2_REGLOCK.IFER0.R = 0x11111111;  /* Write to address mirror & set lock */
    /* Next instruction will cause a Machine Check on cut 2 or later silicon */
  SIUL2.IFER0.R = 0x22222222;          /* Attempt to modify register */
    /* Next instruction will cause a Machine Check on cut 2 or later silicon */
  SIUL2_REGLOCK.IFER0.R = 0x33333333;  /* Attempt to modify, lock register */
  if (SIUL2.IFER0.R==0x11111111) {     /* Verify locked value was maintained */
    LockedRegisterUnchanged = 1;       /* Register was not modified */
  }

  /******************************************************/
  /*  CLEAR SOFT LOCK                                   */
  /******************************************************/

  SIUL2_LOCKBITS.SSLB[14].R = 0xF0;   /* Un-Write protect register SIUL2_IFER0 */
  	  	  	  	  	  	  	  	  	  /* IFER0 has SIUL2 module offset 0x38 = 56*/
                                      /* Register's SSLB index = 56 bytes/4 = 14*/
  SIUL2.IFER0.R = 0x44444444;         /* Attempt to modify register */
  if (SIUL2.IFER0.R==0x44444444) {    /* Verify unlock succeeded */
    UnlockedRegisterChanged = 1;      /* Register was modified */
  }

  /******************************************************/
  /*   WRITE PROTECT A MODULE'S LOCK BITS               */
  /******************************************************/

  SIUL2_REGLOCK.IFER0.R = 0x55555555; /* IFER0: soft lock with new value */
  SIUL2_GLOBALLOCK.GCR.B.HLB = 1;     /* Protect all protectable registers lock bits*/
    /* Next instruction will cause a Machine Check on cut 2 or later silicon */
  SIUL2_LOCKBITS.SSLB[14].R = 0xF0;   /* IFERO: Attempt to remove protection -> IVOR1 */
    /* Next instruction will cause a Machine Check on cut 2 or later silicon */
  SIUL2.IFER0.R = 0x66666666;         /* IFERO: Attempt new value -> IVOR1 */
    /* Next instruction will cause a Machine Check on cut 2 or later silicon */
  SIUL2_REGLOCK.IFER0.R = 0x66666666; /* IFERO: Attempt new value with lock -> IVOR1 */
  if (SIUL2.IFER0.R==0x55555555) {    /* IFER0: Verify prior value maintained*/
    RegisterLockMaintained = 1;       /* IFER0: retained prior value */
  }

  while(1){
	  i++;
	  }

  return 0;
}
/***************************** End of Main ***********************************/
