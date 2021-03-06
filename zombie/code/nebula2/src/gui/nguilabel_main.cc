#include "precompiled/pchngui.h"
//------------------------------------------------------------------------------
//  nguilabel_main.cc
//  (C) 2003 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "gui/nguilabel.h"
#include "gui/nguiserver.h"

nNebulaScriptClass(nGuiLabel, "nguiwidget");

//---  MetaInfo  ---------------------------------------------------------------
/**
    @scriptclass
    nguilabel

    @cppclass
    nGuiLabel
    
    @superclass
    nguiwidget
    
    @classinfo
    Base class for text widgets.
*/

//------------------------------------------------------------------------------
/**
*/
nGuiLabel::nGuiLabel() :
    mouseOver(false),
    colorSet(false),
    color(1.0f, 1.0f, 1.0f, 1.0f),
    blinkColor(0.9f, 0.9f, 0.9f, 1.0f)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nGuiLabel::~nGuiLabel()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    OnMouseMoved() simply sets the mouseOver flag, if a highlight resource
    is set, rendering will then render the highlighted state of the
    label.
*/
bool
nGuiLabel::OnMouseMoved(const vector2& mousePos)
{
    if (this->Inside(mousePos))
    {
        this->mouseOver = true;
    }
    else
    {
        this->mouseOver = false;
    }
    return nGuiWidget::OnMouseMoved(mousePos);
}

//------------------------------------------------------------------------------
/**
    This simply renders the label using the default bitmap resource.
*/
bool
nGuiLabel::Render()
{
    if (this->IsShown())
    {
        nGuiBrush* brush = &this->defaultBrush;
        if (!this->enabled)
        {
            brush = &this->disabledBrush;
        }
        else if (this->blinking)
        {
            nTime time = nGuiServer::Instance()->GetTime();
            if (fmod(time, this->blinkRate) > this->blinkRate/2.0)
            {
                brush = &this->highlightBrush;
            }
        }
        if ( ! this->colorSet )
        {
            // Draw with brush resource color
            nGuiServer::Instance()->DrawBrush(this->GetScreenSpaceRect(), *brush);
        }
        else
        {
            // Draw with this widget's color
            nGuiServer::Instance()->DrawBrush(this->GetScreenSpaceRect(), *brush, this->color);
        }

        return true;
    }
    return false;

    //return nGuiWidget::Render();
}
