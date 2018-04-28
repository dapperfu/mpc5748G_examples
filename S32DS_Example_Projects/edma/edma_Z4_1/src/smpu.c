/*****************************************************************************/
/* FILE NAME: smpu.c                  COPYRIGHT (c) NXP Semiconductors 2016  */
/*                                                      All Rights Reserved  */
/* Description: Simple basic system memory protection unit and enable.       */
/*              Defines chip memory regions, including cache a inhibited     */
/*              region for data shared among masters.                        */
/*                                                                           */
/*****************************************************************************/	
/* REV      AUTHOR        DATE        DESCRIPTION OF CHANGE                  */
/* ---   -----------    ----------    ---------------------                  */
/* 1.0	  S Mihalik     09 Apr 2015   Initial Version                        */
/*****************************************************************************/

#include "project.h" 
#include "smpu.h" 
#include "edma.h" 

/*****************************************************************************/
/* smpu_config                                                               */
/*                                                                           */  
/* SMPU 0 Region Descriptor Summary: (disabled - not used here)              */
/*                                                                           */  
/* SMPU 1 Region Descriptor Summary:                                         */
/*   0  0x4000_0000 to 0x4000_BFFF  All 768K SRAM for MPC5748G               */
/*   1  Variable TCD0_Destination array, 16B, inside SRAM, cache inhibited.  */   
/*      Note: SMPU attributes like CI are OR'd together in overlaped space. */
/*****************************************************************************/

void smpu_config(void) {

  /* Ensure SMPU modules are disabled */
    SMPU_0.CES0.B.GVLD = 0;   /* Allow all accesses from all masters to SMPU0 */
    SMPU_1.CES0.B.GVLD = 0;   /* Allow all accesses from all masters to SMPU1 */
    
  /* Create desired memory regions */
    
    /* Region 0:  All internal SRAM of 768KB */
    SMPU_1.RGD[0].WORD0.R = 0x40000000;  /* Region start addr- start of SRAM */
    SMPU_1.RGD[0].WORD1.R = 0x400BFFFF;  /* Region end addr- end of SRAM  */
    SMPU_1.RGD[0].WORD2.FMT0.R = 0xFFFFFFFF; /* ALL masters can read/write */
    SMPU_1.RGD[0].WORD3.R = 0x00000000;  /* Region cacheable: Cache Inhibit=0*/
    SMPU_1.RGD[0].WORD4.R = 0x00000000;  /* PID not included in region eval. */
    SMPU_1.RGD[0].WORD5.R = 0x00000001;  /* Region is valid without lock */
    
    /* Region 1:  Shared data 16 bytes long inside SRAM, cache inhibited */
    SMPU_1.RGD[1].WORD0.R = (uint32_t)&TCD0_Destination[0]; /* Reg start addr*/
    SMPU_1.RGD[1].WORD1.R = (uint32_t)(&TCD0_Destination[0]) ; /*Region end */
    SMPU_1.RGD[1].WORD2.FMT0.R = 0xFFFFFFFF; /* ALL masters can read/write */
    SMPU_1.RGD[1].WORD3.R = 0x00000002;  /* Region cacheable: Cache Inhibit=2*/
    SMPU_1.RGD[1].WORD4.R = 0x00000000;  /* PID not included in region eval. */
    SMPU_1.RGD[1].WORD5.R = 0x00000001;  /* Region is valid without lock */

  /* Enable all SMPU regions in module */
/*  SMPU_0.CES0.B.GVLD = 1;*/ /* -- NOT USED IN CODE EXAMPLE --SMPU0  */
    SMPU_1.CES0.B.GVLD = 1;   /* SMPU1 is enabled */
   
}
