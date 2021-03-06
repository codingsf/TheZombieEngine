//------------------------------------------------------------------------------
//  nresourceloader_main.cc
//  (C) 2003 Megan Fox
//------------------------------------------------------------------------------
#include "precompiled/pchnnebula.h"
#include "kernel/nkernelserver.h"
#include "resource/nresourceloader.h"

nNebulaClass(nResourceLoader, "nroot");

//------------------------------------------------------------------------------
/**
*/
nResourceLoader::nResourceLoader()
{
}

//------------------------------------------------------------------------------
/**
*/
nResourceLoader::~nResourceLoader()
{
}

//------------------------------------------------------------------------------
/**
    Base Load() function for the nResourceLoader.  This is to be redefined by child classes

    @param filename         the full path to the to-be-loaded file
    @param callingResource  ptr to the nResource calling nResourceLoader::Load()

    @return                 success/failure
*/
bool nResourceLoader::Load(const char* /*filename*/, nResource* /*callingResource*/)
{
    return false;
}

