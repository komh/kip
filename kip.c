// 2001.12.06
#define INCL_DOSMEMMGR
#define INCL_WIN
#include <os2.h>

#include <stdio.h>

#include "kip.h"
#include "kiphook.h"

static PVOID checkKip = NULL;

MRESULT EXPENTRY windowProc( HWND, ULONG, MPARAM, MPARAM );

static BOOL already( VOID );

INT main( VOID )
{
    HAB hab;
    HMQ hmq;
    ULONG flFrameFlags;
    HWND hwndFrame;
    HWND hwndClient;
    QMSG qm;

    if( already())
        return 0;

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0);

    WinRegisterClass(
        hab,
        WC_KIP,
        windowProc,
        CS_SIZEREDRAW,
        sizeof( PVOID )
    );

    flFrameFlags = 0;

    hwndFrame = WinCreateStdWindow(
                    HWND_DESKTOP,               // parent window handle
                    WS_VISIBLE,                 // frame window style
                    &flFrameFlags,              // window style
                    WC_KIP,                     // class name
                    "K Injoy Patch",            // window title
                    0L,                         // default client style
                    NULLHANDLE,                 // resource in exe file
                    ID_KIP,                     // frame window id
                    &hwndClient                 // client window handle
                );

    if( hwndFrame != NULLHANDLE )
    {
        while( WinGetMsg( hab, &qm, NULLHANDLE, 0, 0 ))
            WinDispatchMsg( hab, &qm );

        WinDestroyWindow( hwndFrame );
    }

    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    DosFreeMem( checkKip );

    return 0;
}


MRESULT EXPENTRY windowProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
    switch( msg )
    {
        case WM_CREATE :
            return MRFROMSHORT( !installHook( WinQueryAnchorBlock( hwnd )));

        case WM_DESTROY :
            uninstallHook();

            return MRFROMSHORT( FALSE );
    }

    return WinDefWindowProc( hwnd, msg, mp1, mp2 );
}

BOOL already( VOID )
{
    PSZ     pszKipSig = "\\SHAREMEM\\KIP.SIG";
    APIRET  rc;

    rc = DosGetNamedSharedMem( &checkKip, pszKipSig, PAG_READ );
    if( rc == 0 )
        return TRUE;

    DosAllocSharedMem( &checkKip, pszKipSig, sizeof( HAB ), fALLOC );

    return FALSE;
}

