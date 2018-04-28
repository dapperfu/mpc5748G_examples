#**************************************************************************/-
#* FILE NAME: intc_sw_handlers.s            COPYRIGHT (c) Freescale 2012  */
#*                                                All Rights Reserved     */
#* DESCRIPTION:                                                           */
#*        This file creates prolog, epilog for C ISR and enables nested   */
#*        interrupts. This file starts in memory at the beginning of the  */
#*        ".xcptn" section designated by the label "IVOR4Handler".        */
#* WARNING:  This stack frame does not save the SPEs Accumulator, which   */
#*           is required if SPE instructions are used in ISRs.   If SPE   */
#*           instructions are used, the stack frame must include the      */
#*           accumulator, and prologue and epilogue must be modified.     */
#=========================================================================*/
    .globl   IVOR4_Handler
    
    .equ  INTC_IACKR, 0xfc040020  # Interrupt Acknowledge Register address
    .equ  INTC_EOIR,  0xfc040030   # End Of Interrupt Register address

    .align 4

IVOR4_Handler:
prologue:
    e_stwu      r1,-0x50 (r1)           # Create stack frame and store back chain
    e_stmvsrrw      0x0c (r1)           # Save SRR[0-1] (must be done before enabling MSR[EE])
    se_stw      r3, 0x08 (r1)           # Save working register (r3)
    e_lis       r3, INTC_IACKR@ha       # Save address  of INTC_IACKR in r3
    e_lwz       r3, INTC_IACKR@l(r3)    # Save contents of INTC_IACKR in r3 (this is vector table address)
    wrteei      1                       # Set MSR[EE] (must wait a couple clocks after reading IACKR)
    se_lwz      r3, 0x0(r3)             # Read ISR address from Interrupt Vector Table using pointer
    e_stmvsprw      0x14 (r1)           # Save CR, LR, CTR, XER
    se_mtLR     r3                      # Copy ISR address (from IACKR) to LR for next branch
    e_stmvgprw      0x24 (r1)           # Save GPRs, r[0,3-12]
    se_blrl                             # Branch to ISR, with return to next instruction (epilogue)

epilogue:
    e_lmvsprw       0x14 (r1)           # Restore CR, LR, CTR, XER
    e_lmvgprw       0x24 (r1)           # Restore GPRs, r[0,3-12]
    e_lis       r3, INTC_EOIR@ha        # Load upper half of INTC_EOIR address to r3
    mbar                                # Ensure prior clearing of interrupt flag conmpleted.
    wrteei      0                       # Disable interrupts
    e_stw       r3, INTC_EOIR@l(r3)     # Load lower half of INTC_EOIR address to r3 and
                                        # write contents of r3 to INTC_EOIR
    se_lwz      r3, 0x08 (r1)           # Restore working register (r3) (original value)
    e_lmvsrrw       0x0c (r1)           # Restore SRR[0-1]
    e_add16i    r1, r1, 0x50            # Reclaim stack space
    se_rfi                              # End of Interrupt Handler - re-enables interrupts



#=================================================
#     IVOR 1 (Machine Check) Handler for Core 0
#=================================================

   .globl   IVOR1_Handler
   .align 4


IVOR1_Handler:

                              # PROLOGUE:
  e_stwu     r1, 0x10 (r1)    # Create stack frame (16B alignment required) & store backchain
  se_stw     r3, 0x4 (r1)     # Temporarily save r3 & r4 on stack
  se_stw     r4, 0x8 (r1)     #

			      # CLEAR EXCEPTION FLAGS:
			      # Simple case here - just clear all flags
  e_lis      r3, 0xFFFF       # Build mask to clear all flags
  e_or2i     r3, 0xFFFF
  mtspr 	 572, r3;		  # MCSR is SPR 572, so mtspr moves data from r3 to spr572

			      # ADJUST RETURN INSTRUCTION POINTER (in MCSRR0)
                              # Assumption: all 32 bit VLE instructions for
			      # this core only have opcodes with the most
			      # significant nibble value of 1, 3, 5 or 7. Other
			      # instructions are all 16 bits.
  mfspr	   r3,570;			  # Copy return address in MCSRR0 to r3, MCSRRO is SPR 570, so mfspr moves data from spr570 to r3
  e_lbz    r3, 0(r3)          # Load r3 with first byte of instruction's opcode
  se_srwi  r3, 4              # Shift nibble to least signifant position
  se_cmpli r3, 1              # Test if nibble = 1
  se_beq   add_4_bytes        # If nibble = 1, opcode is 32 bit.
  se_cmpli r3, 3              # Test if nibble = 3
  se_beq   add_4_bytes        # If nibble Add 4 to the return address
  se_cmpli r3, 5              # Test if nibble = 5
  se_beq   add_4_bytes        # If equal Add 4 to the return address
  se_cmpli r3, 7              # Test if nibble = 7
  se_bne   add_2_bytes        # If not equal, go add 2 to the return address
add_4_bytes:                  # Increment return address by 4 bytes
  mfspr    r3,570;			  # Copy return address in MCSRR0 to r3
  se_addi  r3, 4              # Add 4 bytes to current value
  se_b     adjust_MCSSR0      # Make adjustment to return address
add_2_bytes:                  # Increment return address by 2 bytes
  mfspr    r3,570;			  # Copy return address in MCSRR0 to r3
  se_addi  r3, 2              # Add 2 bytes to current value
adjust_MCSSR0:
  mtspr 570,r3;				  # Copy return address in MCSRR0 to r3

                              # EPILOGUE
  se_lwz    r3, 0x04 (r1)     # Restore r3
  se_lwz    r4, 0x08 (r1)     # Restore r4
  se_rfmci                    # Return from Machine Check interrupt

