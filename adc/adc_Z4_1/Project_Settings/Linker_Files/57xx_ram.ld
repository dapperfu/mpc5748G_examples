/* Entry Point */
ENTRY(_start)

/* define heap and stack size */
__HEAP_SIZE            = 0 ;
__STACK_SIZE           = 4096 ;

SRAM_SIZE =  256K;    
/* Define SRAM Base Address */
SRAM_BASE_ADDR = 0x40080000;
    
MEMORY
{
    SRAM :	org = 0x40080000,   len = 256K
}


SECTIONS
{

    .startup : ALIGN(0x400)
    {
    __start = . ;
    	*(.startup)
    } > SRAM
    
    .core_exceptions_table   : ALIGN(4096)
    {
      __IVPR_VALUE = . ;
      *(.core_exceptions_table)
    } > SRAM

    .intc_vector_table   : ALIGN(4096)
    {
      *(.intc_vector_table)
    } > SRAM
      
    .text : 
    {
      *(.text.startup)
      *(.text)
      *(.init)
      *(.text.*)
      . = ALIGN(16);
    } > SRAM
       
    .rodata :
    {
      *(.rodata)
      *(.rodata.*)
    } > SRAM
    
    .eh_frame_hdr : { *(.eh_frame_hdr) } > SRAM
    .eh_frame     : { KEEP (*(.eh_frame)) } > SRAM    
   
	.data   : 
	{
	  *(.data)
	  *(.data.*)
	}  > SRAM
    
    .sdata2  :
	{  
	  *(.sdata2)
	  *(.sdata2.*)
	} > SRAM

	.sbss2    (NOLOAD)   :
	{
      /* _SDA2_BASE_ = .; */ 
	  *(.sbss2)
	  *(.sbss2.*)
	} > SRAM
    
    .sdata  :
	{
	  *(.sdata)
	  *(.sdata.*)
	} > SRAM
    
	.bss   (NOLOAD)  :
	{
	  __BSS_START = .;
	  *(.sbss)
	  *(.sbss.*)	
      *(.bss)
      *(.bss.*)
      *(COMMON)
      __BSS_END = .;
    } > SRAM
    
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
    } > SRAM
 
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

