#include "demos/demoapp.h"

#include "kernel/nfileserver2.h"
#include "kernel/ntimeserver.h"
#include "input/ninputserver.h"
#include "gfx2/ngfxserver2.h"
#include "gfx2/nmesh2.h"
#include "gfx2/nshader2.h"
#include "util/nrandomlogic.h"

//------------------------------------------------------------------------------
/**
    run script that loads required resources, etc.
	and sets initial position of everything
*/
void DemoApp::Init()
{
    //empty
}

//------------------------------------------------------------------------------
/**
*/
bool DemoApp::Open()
{
    return true;
}

//------------------------------------------------------------------------------

void DemoApp::Close()
{
    //empty
}

//------------------------------------------------------------------------------

void DemoApp::Tick( float /*fTimeElapsed*/ )
{
    //empty
}

//------------------------------------------------------------------------------

void DemoApp::Render()
{
    //empty
}

//------------------------------------------------------------------------------

bool
DemoApp::LoadResource( nResource* pResource, const nString& strFilename )
{
    pResource->SetFilename( strFilename );
    return pResource->Load();
}

//------------------------------------------------------------------------------

int
DemoApp::BeginDraw(nShader2 *pShader, nMesh2 *pMesh)
{
    //setup shader
    nGfxServer2::Instance()->SetShader( pShader );
    int nPasses = pShader->Begin( false );

    //draw one sphere mesh for every joint
    nGfxServer2* pGfxServer = nGfxServer2::Instance();
    pGfxServer->SetMesh( pMesh, pMesh );
    pGfxServer->SetVertexRange( 0, pMesh->GetNumVertices() );
    pGfxServer->SetIndexRange( 0, pMesh->GetNumIndices() );

    return nPasses;
}

//------------------------------------------------------------------------------

void
DemoApp::BeginPass(nShader2 *pShader, int passIndex)
{
    pShader->BeginPass( passIndex );
}

//------------------------------------------------------------------------------

void
DemoApp::Draw( const vector3& vPosition, const vector3& vScale )
{
    matrix44 matWorld;
    matWorld.scale( vScale );
    matWorld.translate( vPosition );
    nGfxServer2* pGfxServer( nGfxServer2::Instance() );
    pGfxServer->SetTransform( nGfxServer2::Model, matWorld );
    pGfxServer->DrawIndexedNS( nGfxServer2::TriangleList );
}

//------------------------------------------------------------------------------

void
DemoApp::EndPass( nShader2* pShader )
{
    pShader->EndPass();
}

//------------------------------------------------------------------------------

void
DemoApp::EndDraw( nShader2* pShader )
{
    pShader->End();
}
