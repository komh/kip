/*
        DLL for menu auto selecting by mouse position
        Copyright (C) 1999 by KO Myung-Hun

        This library is free software; you can redistribute it and/or
        modify it under the terms of the GNU Library General Public
        License as published by the Free Software Foundation; either
        version 2 of the License, or any later version.

        This library is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
        Library General Public License for more details.

        You should have received a copy of the GNU Library General Public
        License along with this library; if not, write to the Free
        Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    Change Log :

        Written by KO Myung-Hun
        Term of programming : 2001.12.06

        Source file   : kiphook.c
        Used compiler : emx 0.9d + gcc 2.8.1
*/

#define INCL_DOSMODULEMGR
#define INCL_PM
#include <os2.h>
#include <stdio.h>
#include <string.h>

#include "kiphook.h"

#define DLL_NAME    "KIPHOOK"

#define WC_INJOY    "Firewall.Main.Client"
#define IDM_DISCONNECT  0x12F

static HAB hab = 0;
static HMODULE hm = NULLHANDLE;

BOOL isInjoy( HWND hwnd )
{
    char className[ 80 ];

    WinQueryClassName( hwnd, sizeof( className ), className );

    return ( strcmp( className, WC_INJOY ) == 0 ? TRUE : FALSE );
}

VOID EXPENTRY sendMsgHook( HAB habAB, PSMHSTRUCT psmhs, BOOL fInterTask )
{
    if(( psmhs->msg == WM_DESTROY ) && isInjoy( psmhs->hwnd ))
    {
        WinSendMsg( psmhs->hwnd, WM_COMMAND,
                    MPFROMSHORT( IDM_DISCONNECT ), MPFROM2SHORT( CMDSRC_MENU, TRUE ));
    }

}

BOOL EXPENTRY installHook( HAB habAB )
{
    if( DosQueryModuleHandle( DLL_NAME, &hm ) != 0 )
        return FALSE;

    hab = habAB;
    WinSetHook( hab, NULLHANDLE, HK_SENDMSG, ( PFN)sendMsgHook, hm );

    return TRUE;
}

VOID EXPENTRY uninstallHook( VOID )
{
    WinReleaseHook( hab, NULLHANDLE, HK_SENDMSG, ( PFN )sendMsgHook, hm );

    WinBroadcastMsg(HWND_DESKTOP, WM_NULL, 0, 0, BMSG_FRAMEONLY | BMSG_POST);
}

