/* Entry Point */
ENTRY(_start)

/* define heap and stack size */
__HEAP_SIZE            = 0 ;
__STACK_SIZE           = 4096 ;

SRAM_SIZE =  256K;    
/* Define SRAM Base Address */
SRAM_BASE_ADDR = 0x40040000;
    
MEMORY
{

    flash_rchw : org = 0x00FA0000,   len = 0x4
    cpu0_reset_vec : org = 0x00FA0000+0x10,   len = 0x4
    cpu1_reset_vec : org = 0x00FA0000+0x14,   len = 0x4
    cpu2_reset_vec : org = 0x00FA0000+0x04,   len = 0x4      
        
    m_text :	org = 0x11d0000, len = 1856K  
    m_data :	org = 0x40040000,   len = 256K
}


SECTIONS
{
    .rchw   : 
    {
        KEEP(*(.rchw))
    } > flash_rchw

    .cpu0_reset_vector  : 
    {
        KEEP(*(.cpu0_reset_vector))
    } > cpu0_reset_vec

    .cpu1_reset_vector  : 
    {
        KEEP(*(.cpu1_reset_vector))
    } > cpu1_reset_vec

    .cpu2_reset_vector  : 
    {
        KEEP(*(.cpu2_reset_vector))
    } > cpu2_reset_vec

    /* Note: if you move the 'startup' section shall modify the RCHW2_1 value for the corresponding core in the flashrchw.c file. */
    .startup : ALIGN(0x400)
    {
    __start = . ;
    	*(.startup)
    } > m_text
    
    .core_exceptions_table   : ALIGN(4096)
    {
      __IVPR_VALUE = . ;
      *(.core_exceptions_table)
    } > m_text

    .intc_vector_table   : ALIGN(4096)
    {
      *(.intc_vector_table)
    } > m_text
      
    .text : 
    {
      *(.text.startup)
      *(.text)
      *(.init)
      *(.text.*)
      . = ALIGN(16);
    } > m_text
       
    .rodata :
    {
      *(.rodata)
      *(.rodata.*)
    } > m_text
    
    .eh_frame_hdr : { *(.eh_frame_hdr) } > m_text
    .eh_frame     : { KEEP (*(.eh_frame)) } > m_text    
   
	.data   : 
	{
	  *(.data)
	  *(.data.*)
	}  > m_data AT>m_text
    
    .sdata2  :
	{  
	  *(.sdata2)
	  *(.sdata2.*)
	} > m_data AT>m_text

	.sbss2    (NOLOAD)   :
	{
      /* _SDA2_BASE_ = .; */ 
	  *(.sbss2)
	  *(.sbss2.*)
	} > m_data
    
    .sdata  :
	{
	  *(.sdata)
	  *(.sdata.*)
	} > m_data AT>m_text
    
	.bss   (NOLOAD)  :
	{
	  __BSS_START = .;
	  *(.sbss)
	  *(.sbss.*)	
      *(.bss)
      *(.bss.*)
      *(COMMON)
      __BSS_END = .;
    } > m_data
    
    .stack (NOLOAD) : ALIGN(16) 
    {
      __HEAP = . ;
      PROVIDE (_end = . );
      PROVIDE (end = . );            
      . += __HEAP_SIZE ;
      __HEAP_END = . ;
      _stack_end = . ;
      . +=  __STACK_SIZE ;
      _stack_addr = . ;
      __SP_INIT = . ;
      . += 4;
    } > m_data
 
/*-------- LABELS USED IN CODE -------------------------------*/
        
/* Labels for Copying Initialised Data from Flash to RAM */
__DATA_SRAM_ADDR  = ADDR(.data);
__SDATA_SRAM_ADDR = ADDR(.sdata);

__DATA_SIZE   = SIZEOF(.data);
__SDATA_SIZE  = SIZEOF(.sdata);

__DATA_ROM_ADDR  = LOADADDR(.data);
__SDATA_ROM_ADDR = LOADADDR(.sdata);
   
/* Labels Used for Initialising SRAM ECC */
__SRAM_SIZE = SRAM_SIZE;
__SRAM_BASE_ADDR = SRAM_BASE_ADDR;

__BSS_SIZE    = __BSS_END - __BSS_START;      
      
}

