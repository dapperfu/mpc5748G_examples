/*
 * smpu.c
 *
 *  Created on: Apr 5, 2016
 *      Author: B55457
 */

#include "smpu.h"

void initSMPU(void) {
  /* SMPU0 handles PFLASH, PBRIDGE_A, PBRIDGE_B accessed by XBAR0 masters plus
     debug interfaces: Z4a (M0), Z4b (M1), Z2 (M2), HSM (M3), DMA (M4), Z4a
     debug (M8), Z4b debug (M9), Z2 debug (M10) SMPU1 handles all 3 SRAM blocks
     accessed by XBAR1 masters plus debug interfaces: Z4a (M0), Z4b (M1), Z2
     (M2), HSM (M3), DMA (M4), Ethernet (M5), FlexRay(M6), MLB (M7), Z4a debug
     (M8), Z4b debug (M9), Z2 debug (M10), USB0 (M11), USB1 (M12), uSDHC (M13)
   */

  /* UTest (16kB) */
  SMPU_0.RGD[0].WORD0.R = 0x00400000; /* Start address */
  SMPU_0.RGD[0].WORD1.R = 0x00403fff; /* End address */
  SMPU_0.RGD[0].WORD2.FMT0.R =
      0xff00fc00; /* rw set for masters: M0, M1, M2, M3, M8, M9, M10 */
  SMPU_0.RGD[0].WORD5.B.VLD = 1; /* Enable descriptor[0] */

  // BAF (16kB)
  SMPU_0.RGD[1].WORD0.R = 0x00404000; /* Start address */
  SMPU_0.RGD[1].WORD1.R = 0x00407fff; /* End address */
  SMPU_0.RGD[1].WORD2.FMT0.R =
      0xff00fc00; /* rw set for masters: M0, M1, M2, M3, M8, M9, M10 */
  SMPU_0.RGD[1].WORD5.B.VLD = 1; /* Enable descriptor[1] */

  // HSM code (128kB)
  SMPU_0.RGD[2].WORD0.R = 0x00610000; /* Start address */
  SMPU_0.RGD[2].WORD1.R = 0x0062ffff; /* End address */
  SMPU_0.RGD[2].WORD2.FMT0.R =
      0xff00fc00; /* rw set for masters: M0, M1, M2, M3, M8, M9, M10 */
  SMPU_0.RGD[2].WORD5.B.VLD = 1; /* Enable descriptor[2] */

  // HSM data (32kB)
  SMPU_0.RGD[3].WORD0.R = 0x00f80000; /* Start address */
  SMPU_0.RGD[3].WORD1.R = 0x00f87fff; /* End address */
  SMPU_0.RGD[3].WORD2.FMT0.R =
      0xff00fc00; /* rw set for masters: M0, M1, M2, M3, M8, M9, M10 */
  SMPU_0.RGD[3].WORD5.B.VLD = 1; /* Enable descriptor[3] */

  // NVM area used by all cores (6MB)
  SMPU_0.RGD[4].WORD0.R = 0x00f8C000; /* Start address */
  SMPU_0.RGD[4].WORD1.R = 0x0157ffff; /* End address */
  SMPU_0.RGD[4].WORD2.FMT0.R =
      0xff00fc00; /* rw set for masters: M0, M1, M2, M3, M8, M9, M10 */
  SMPU_0.RGD[4].WORD5.B.VLD = 1; /* Enable descriptor[4] */

  // Peripheral Address Space (256MB)
  SMPU_0.RGD[5].WORD0.R = 0xf0000000; /* Start address */
  SMPU_0.RGD[5].WORD1.R = 0xffffffff; /* End address */
  SMPU_0.RGD[5].WORD2.FMT0.R =
      0xffc0fc00; /* rw set for masters: M0, M1, M2, M3, M4, M8, M9, M10 */
  SMPU_0.RGD[5].WORD3.B.CI = 1;  /* Set cache inhibit */
  SMPU_0.RGD[5].WORD5.B.VLD = 1; /* Enable descriptor[5] */

  // SRAM used by all cores (768kB - 4 kB)
  SMPU_1.RGD[0].WORD0.R = 0x40000000; /* Start address */
  SMPU_1.RGD[0].WORD1.R = 0x400befff; /* End address */
  SMPU_1.RGD[0].WORD2.FMT0.R =
      0xffffff00; /* rw set for masters: M0, M1, M2, M3, M4, M5, M6, M7, M8, M9,
                     M10, M11, M12, M13 */
  SMPU_1.RGD[0].WORD5.B.VLD = 1; /* Enable descriptor[0] */

  // SRAM used by all cores (last 4kB)
  SMPU_1.RGD[1].WORD0.R = 0x400bf000; /* Start address */
  SMPU_1.RGD[1].WORD1.R = 0x400bffff; /* End address */
  SMPU_1.RGD[1].WORD2.FMT0.R =
      0xbfffff00; /* Read only for M0, rw set for masters: M1, M2, M3, M4, M5,
                     M6, M7, M8, M9, M10, M11, M12, M13 */
  SMPU_1.RGD[1].WORD5.B.VLD = 1; /* Enable descriptor[1] */

  // global enable of both MPUs
  SMPU_0.CES0.B.GVLD = 1;
  SMPU_1.CES0.B.GVLD = 1;
}
