// 1999.11.23
#ifndef __KIPHOOK_H__
#define __KIPHOOK_H__

#define INCL_PM
#include <os2.h>

#ifdef __cplusplus
extern "C" {
#endif

BOOL EXPENTRY installHook( HAB );
VOID EXPENTRY uninstallHook( VOID );

#ifdef __cplusplus
}
#endif

#endif

