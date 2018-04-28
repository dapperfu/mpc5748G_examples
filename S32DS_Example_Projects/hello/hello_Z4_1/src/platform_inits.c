/*****************************************************************************/
/* FILE NAME: platform_inits.c                 COPYRIGHT (c) Freescale 2013  */
/*                                                      All Rights Reserved  */
/* DESCRIPTION: Various MPC5748G platform initializations including:         */
/*       - RAM, Flash configuration: wait states per frequency, etc.         */
/*       - Crossbar configuration                                            */
/*                                                                           */
/*****************************************************************************/
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  SM            01 Aug 2013   Initial Version                        */
/*****************************************************************************/

#include "project.h"
#include "platform_inits.h"

#define PFLASH_PFCR1_REG  PFLASH.PFCR1.R  /* PFLASH registers */
#define PFLASH_PFCR2_REG  PFLASH.PFCR2.R
#define PFLASH_PFCR3_REG  PFLASH.PFCR3.R
#define PFLASH_PFAPR_REG  PFLASH.PFAPR.R
#define PFLASH_PFCR4_REG  PFLASH.PFCR4.R

#define PFLASH_PFCR1_VALUE_16MHz 0x00152117  /* Flash port p0 configuration:  */
    /* Port 0 prefetch enables for masters 0,3,5 (core I-busses) only,       */
    /* APC=1 Pipelined access can be intiazted 1 cyc before prev data valid, */
    /* RWSC=1 Read Wait State Control: 1 add'l wait states (16 MHz),         */
    /* P0_DPFEN=0 No prefetching is triggerd by a data read access,          */
    /* P0_IPFEN=1 Prefetching may be triggered by any instruction read access*/
    /* P0_PFLIM=3 Prefetch on miss or hit,                                   */
    /* P0_BFEN=1 Line read buffers are enabled.                              */
#define PFLASH_PFCR1_VALUE_160MHz 0x00152417  /* Flash port p0 configuration: */
    /* RWSC=4 r Read Wait State Control: 4 add'l wait states (160 MHz),       */
    /* All other fields same as for 16 MHz.                                  */
#define PFLASH_PFCR2_VALUE_BASIC 0x00150017  /* Flash port p1 configuration:  */
    /* Configured same as Port 0 except for not present fields of APC, RWSC. */
#define PFLASH_PFCR3_VALUE_BASIC 0x00000000  /* Flash way cfg, arb., BAF dis. */
    /* All ports have both buffers available for any flash access,           */
    /* BAF_DIS=0 Executable access to BAF flash region is allowed,           */
    /* ARBM=0 Fixed priority arbitration with AHB p0>p1>p2.                  */
#define PFLASH_PFAPR_VALUE_BASIC 0xFFFFFFFF  /* R/W access to flash array:    */
    /* All masters have both read/write access to flash array.               */
#define PFLASH_PFCR4_VALUE_BASIC 0x00150017  /* Flash port p2 configuration:  */
    /* Configured same as Port 0 except for not present fields of APC, RWSC. */

/*****************************************************************************/
/* memory_config_[x]mhz                                                      */
/* Description: Configures flash and SRAM for wait states, access, etc.      */
/*     Includes RAM based function to write value to register. This function */
/*     avoids modifying flash attribute registers while executing from flash.*/
/*     Ensure the flash is not being accessed when modifying these registers.*/
/*****************************************************************************/

void memory_config_16mhz(void) {

  uint32_t mem_write_code_vle [] = {
    0x54640000, /* e_stw r3,(0)r4 instr.: writes r3 contents to addr in r4   */
    0x7C0006AC, /* mbar instruction: ensure prior store completed            */
    0x00040004  /* 2 se_blr's instr.: branches to return address in link reg */
    };
      /* Structures are default aligned on a boundary which is a multiple of */
      /* the largest sized element, 4 bytes in this case.  The first two     */
      /* instructions are 4 bytes, so the last instruction is duplicated to  */
      /* avoid the compiler adding padding of 2 bytes before the instruction.*/

  typedef void (*mem_write_code_ptr_t)(uint32_t, uint32_t);
        /* create a new type def: a func pointer called mem_write_code_ptr_t */
        /* which does not return a value (void)                              */
        /* and will pass two 32 bit unsigned integer values                  */
        /* (per EABI, the first parameter will be in r3, the second r4)      */

  asm (" mbar");    /* Wait for all prior data storage accesses to complete. */

  (*((mem_write_code_ptr_t)mem_write_code_vle))  /* PFCR1 initialization */
                         /* cast mem_write_code as func ptr                  */
                         /* "*" de-references func ptr, i.e. converts to func*/
     (PFLASH_PFCR1_VALUE_16MHz,    /* which passes integer (in r3)           */
     (uint32_t)&PFLASH_PFCR1_REG); /* and address to write integer (in r4)   */

  (*((mem_write_code_ptr_t)mem_write_code_vle))  /* PFCR2 initialization */
       (PFLASH_PFCR2_VALUE_BASIC,
       (uint32_t)&PFLASH_PFCR2_REG);

  (*((mem_write_code_ptr_t)mem_write_code_vle))  /* PFCR3 initialization */
       (PFLASH_PFCR3_VALUE_BASIC,
       (uint32_t)&PFLASH_PFCR3_REG);

  (*((mem_write_code_ptr_t)mem_write_code_vle))  /* PFAPR initialization */
       (PFLASH_PFAPR_VALUE_BASIC,
       (uint32_t)&PFLASH_PFAPR_REG);

  (*((mem_write_code_ptr_t)mem_write_code_vle))  /* PFCR4 initialization */
       (PFLASH_PFCR4_VALUE_BASIC,
       (uint32_t)&PFLASH_PFCR4_REG);

  PRAMC_0.PRCR1.R = 0x00000000; /* RAM P0: P0 burst optimized, 0 wait states*/
  PRAMC_1.PRCR1.R = 0x00000000; /* RAM P1: same configuration as P0. */
  PRAMC_2.PRCR1.R = 0x00000000; /* RAM P2: same configuration as P0. */
}

void memory_config_160mhz(void) {

  uint32_t mem_write_code_vle [] = {
    0x54640000, /* e_stw r3,(0)r4 instr.: writes r3 contents to addr in r4   */
    0x7C0006AC, /* mbar instruction: ensure prior store completed            */
    0x00040004  /* 2 se_blr's instr.: branches to return address in link reg */
    };
      /* Structures are default aligned on a boundary which is a multiple of */
      /* the largest sized element, 4 bytes in this case.  The first two     */
      /* instructions are 4 bytes, so the last instruction is duplicated to  */
      /* avoid the compiler adding padding of 2 bytes before the instruction.*/

  typedef void (*mem_write_code_ptr_t)(uint32_t, uint32_t);
        /* create a new type def: a func pointer called mem_write_code_ptr_t */
        /* which does not return a value (void)                              */
        /* and will pass two 32 bit unsigned integer values                  */
        /* (per EABI, the first parameter will be in r3, the second r4)      */

  asm (" mbar");     /* Wait for prior code to complete before proceeding.    */

     (*((mem_write_code_ptr_t)mem_write_code_vle))
                         /* cast mem_write_code as func ptr                  */
                         /* "*" de-references func ptr, i.e. converts to func*/
       (PFLASH_PFCR1_VALUE_160MHz,   /* which passes integer (in r3)         */
       (uint32_t)&PFLASH_PFCR1_REG); /* and address to write integer (in r4) */

     (*((mem_write_code_ptr_t)mem_write_code_vle))
       (PFLASH_PFCR2_VALUE_BASIC,
       (uint32_t)&PFLASH_PFCR2_REG);

     (*((mem_write_code_ptr_t)mem_write_code_vle))
       (PFLASH_PFCR3_VALUE_BASIC,
       (uint32_t)&PFLASH_PFCR3_REG);

     (*((mem_write_code_ptr_t)mem_write_code_vle))
       (PFLASH_PFAPR_VALUE_BASIC,
       (uint32_t)&PFLASH_PFAPR_REG);

     (*((mem_write_code_ptr_t)mem_write_code_vle))
       (PFLASH_PFCR4_VALUE_BASIC,
       (uint32_t)&PFLASH_PFCR4_REG);

  PRAMC_0.PRCR1.R = 0x00000000; /* RAM P0: P0 burst optimized, 0 wait states*/
  PRAMC_1.PRCR1.R = 0x00000000; /* RAM P1: same configuration as P0. */
  PRAMC_2.PRCR1.R = 0x00000000; /* RAM P2: same configuration as P0. */
}

/*****************************************************************************/
/* crossbar_config                                                           */
/* Description: Configures crossbar parameters.                              */
/*****************************************************************************/

void crossbar_config(void) {
      /* Place holder for code */

}
