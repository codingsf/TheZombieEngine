//------------------------------------------------------------------------------
//  nramfileserver_cmds.cc
//  (C) 2004 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "precompiled/pchnnebula.h"
#define N_IMPLEMENTS nRamFileServer

#include "file/nramfileserver.h"

//static void n_xxx(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nramfileserver
    
    @cppclass
    nRamFileServer
    
    @superclass
    nfileserver2

    @classinfo
    Transparent filesystem access into RAM.
*/
void
n_initcmds_nRamFileServer(nClass* cl)
{
    cl->BeginCmds();
//    cl->AddCmd("v_xxx_v", 'XXXX', n_xxx);
    cl->EndCmds();
}

