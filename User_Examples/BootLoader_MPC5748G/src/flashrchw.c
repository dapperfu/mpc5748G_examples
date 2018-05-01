#if defined(START_FROM_FLASH)
#include "typedefs.h"

#define MPC57xx_ID 0x005A0000 /* RCHW boot ID for MPC57xx devices     */
#define VLE_ENABLE 0x01000000 /* VLE is enabled                       */

#define CPU2_ENABLED 0x00000001
#define CPU0_ENABLED 0x00000002
#define CPU1_ENABLED 0x00000008

extern void _start(void);
extern void _startcore1(void);
extern void _startcore2(void);

#define TARGET_CORES (CPU0_ENABLED | CPU1_ENABLED | CPU2_ENABLED)

//#define RCHW_VAL (MPC57xx_ID | TARGET_CORES)
#define RCHW_VAL 0x0000005A

const volatile uint32_t APPKEY __attribute__((section(".appkey"))) = 0x55AA55AA;

const uint32_t __attribute__((section(".rchw")))
RCHW[] = {RCHW_VAL,
          /* cpu0_reset_vector */ (uint32_t)_start,
          /* delay */ 0x2DC6C0, // 3 sec
          /* application key */ (uint32_t)&APPKEY,
          /* cpu2_reset_vector */ (uint32_t)_startcore2,
          /* cpu1_reset_vector */ (uint32_t)_startcore1};

#endif /* defined(START_FROM_FLASH) */
