//------------------------------------------------------------------------------
//  nd3d9windowhandler.cc
//  (C) 2004 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "precompiled/pchndirect3d9.h"
#include "gfx2/nd3d9windowhandler.h"
#include "gfx2/nd3d9server.h"

//------------------------------------------------------------------------------
/**
*/
nD3D9WindowHandler::nD3D9WindowHandler(nD3D9Server* ptr) :
    d3d9Server(ptr)
{
    n_assert(this->d3d9Server);
}

//------------------------------------------------------------------------------
/**
*/
nD3D9WindowHandler::~nD3D9WindowHandler()
{
    this->d3d9Server = 0;
}

//------------------------------------------------------------------------------
/**
    FIXME: this method directly accesses D3D9Server members!
*/
bool
nD3D9WindowHandler::OnSetCursor()
{
    n_assert(this->d3d9Server);
    if (this->d3d9Server->d3d9Device)
    {
        switch (this->d3d9Server->cursorVisibility)
        {
            case nGfxServer2::None:
                SetCursor(NULL);
                this->d3d9Server->d3d9Device->ShowCursor(FALSE);
                return true;

            case nGfxServer2::System:
                this->d3d9Server->d3d9Device->ShowCursor(FALSE);
                break;

            case nGfxServer2::Custom:
                SetCursor(NULL);
                this->d3d9Server->d3d9Device->ShowCursor(TRUE);
                return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
    FIXME: this method directly accesses D3D9Server members!
*/
bool
nD3D9WindowHandler::OnMouseMove()
{
    n_assert(this->d3d9Server);
    if (this->d3d9Server->d3d9Device && d3d9Server->GetDisplayMode().GetType() == nDisplayMode2::Fullscreen)
    {
        // in fullscreen mode, update the cursor position myself
        POINT ptCursor;
        if (!GetCursorPos( &ptCursor ))
        {
            // Display the string.
            n_printf("Could not get cursor position.\nError was:\n%s\n", n_getlastsystemerror().Get());
        }
        else
        {
            // position the cursor
            d3d9Server->d3d9Device->SetCursorPosition(ptCursor.x, ptCursor.y, D3DCURSOR_IMMEDIATE_UPDATE);
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
nD3D9WindowHandler::OnSetFocus()
{
    n_assert( this->d3d9Server );
    this->d3d9Server->SetFocus( true );
    return false;
}

//------------------------------------------------------------------------------
/**
*/
bool
nD3D9WindowHandler::OnKillFocus()
{
    n_assert( this->d3d9Server );
    this->d3d9Server->SetFocus( false );
    return false;
}

//------------------------------------------------------------------------------
/**
    FIXME: this method directly accesses D3D9Server members!
*/
void
nD3D9WindowHandler::OnPaint()
{
    n_assert(this->d3d9Server);
    if ((this->GetDisplayMode().GetType() != nDisplayMode2::Fullscreen) && this->d3d9Server->d3d9Device) 
    {
        if (!this->d3d9Server->InDialogBoxMode())
        {
            this->d3d9Server->d3d9Device->Present(0, 0, 0, 0);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nD3D9WindowHandler::OnToggleFullscreenWindowed()
{
    n_assert(this->d3d9Server);

    // toggle fullscreen/windowed
    nDisplayMode2 newMode = this->GetDisplayMode();
    if (newMode.GetType() == nDisplayMode2::Fullscreen)
    {
        newMode.SetType(nDisplayMode2::Windowed);
    }
    else
    {
        newMode.SetType(nDisplayMode2::Fullscreen);
    }
    this->d3d9Server->CloseDisplay();
    this->d3d9Server->SetDisplayMode(newMode);
    this->d3d9Server->OpenDisplay();
}

//------------------------------------------------------------------------------
/**
*/
bool
nD3D9WindowHandler::OnClose()
{
    n_assert(this->d3d9Server);

    // *** HACK *** HACK *** HACK ***
    /*
        int res = MessageBox(this->hWnd, "Really Quit?", "Hack Hack Hack", MB_OKCANCEL);
        return (res == IDOK);
    */
    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
nD3D9WindowHandler::OnSize(bool minimize)
{
    // only handle show requests when the d3ddevice is initiated
    if (!minimize && this->d3d9Server->d3d9Device)
    {
        nWin32WindowHandler::OnSize(minimize);
    }
    else
    {
        nWin32WindowHandler::OnSize(minimize);
    }
}
