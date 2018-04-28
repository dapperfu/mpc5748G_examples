/* Entry Point */
ENTRY(_start)

/* define heap and stack size */
__HEAP_SIZE            = 0 ;
__STACK_SIZE           = 4096 ;

SRAM_SIZE =  256K;    
/* Define SRAM Base Address */
SRAM_BASE_ADDR = 0x40000000;
    
MEMORY
{
    flash_rchw : org = 0x00FC0000,   len = 16K
    m_text :	org = 0x1000000, 	len = 1856K  
    m_data :	org = 0x40000000,   len = 256K
}

