#ifndef N_DISPLAYMODE2_H
#define N_DISPLAYMODE2_H
//------------------------------------------------------------------------------
/**
    @class nDisplayMode2
    @ingroup Gfx2

    Contains display mode parameters.

    (C) 2002 RadonLabs GmbH
*/
#include "kernel/ntypes.h"
#include "util/nstring.h"

//------------------------------------------------------------------------------
class nDisplayMode2
{
public:
    /// display mode types
    enum Type
    {
        Windowed,       // windowed mode
        Fullscreen,     // fullscreen mode
        AlwaysOnTop,    // windowed, always on top
        ChildWindow,    // windowed, as child window
    };

    /// bit depths
    enum Bpp
    {
        Bpp16,
        Bpp32,
    };

    /// constructor
    nDisplayMode2();
    /// constructor
    nDisplayMode2(const char* winTitle, Type t, ushort x, ushort y, ushort w, ushort h, bool vSync, bool dialogBoxMode, const char* iconResName);
    /// set display mode
    void Set(const char* winTitle, Type t, ushort x, ushort y, ushort w, ushort h, bool vSync, bool dialogBoxMode, const char* iconResName);
    /// set x position
    void SetXPos(ushort x);
    /// get x position
    ushort GetXPos() const;
    /// set y position
    void SetYPos(ushort y);
    /// get y position
    ushort GetYPos() const;
    /// set display width
    void SetWidth(ushort w);
    /// get display width
    ushort GetWidth() const;
    /// set display height
    void SetHeight(ushort h);
    /// get display height
    ushort GetHeight() const;
    /// set display type
    void SetType(Type t);
    /// get display type
    Type GetType() const;
    /// set bit depth
    void SetBpp(Bpp depth);
    /// get bit depth
    Bpp GetBpp() const;
    /// set window title
    void SetWindowTitle(const char* t);
    /// get window title
    const char* GetWindowTitle() const;
    /// set vertical sync flag
    void SetVerticalSync(bool b);
    /// get vertical sync flag
    bool GetVerticalSync() const;
    /// set optional window icon resource
    void SetIcon(const char* resName);
    /// get optional window icon resource
    const char* GetIcon() const;
    /// make this mode compatible with host system dialog boxes
    void SetDialogBoxMode(bool b);
    /// get dialog box mode
    bool GetDialogBoxMode() const;
    /// convert type to string
    static const char* TypeToString(Type t);
    /// convert string to type
    static Type StringToType(const char* str);

private:
    nString windowTitle;
    nString iconName;
    Type type;
    ushort xpos;
    ushort ypos;
    ushort width;
    ushort height;
    Bpp bitsPerPixel;
    bool verticalSync;
    bool dialogBoxMode;
};

//------------------------------------------------------------------------------
/**
*/
inline
nDisplayMode2::nDisplayMode2() :
    windowTitle("Nebula2 Viewer"),
    type(Windowed),
    xpos(0),
    ypos(0),
    width(640),
    height(480),
    bitsPerPixel(Bpp32),
    verticalSync(true),
    dialogBoxMode(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
nDisplayMode2::nDisplayMode2(const char* winTitle, Type t, ushort x, ushort y, ushort w, ushort h, bool vSync, bool dialogBoxMode, const char* iconResName) :
    windowTitle(winTitle),
    type(t),
    xpos(x),
    ypos(y),
    width(w),
    height(h),
    bitsPerPixel(Bpp32),
    verticalSync(vSync),
    dialogBoxMode(dialogBoxMode),
    iconName(iconResName)
{
    n_assert(w > 0);
    n_assert(h > 0);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::Set(const char* winTitle, Type t, ushort x, ushort y, ushort w, ushort h, bool vSync, bool dialogBoxMode, const char* iconResName)
{
    n_assert(w > 0);
    n_assert(h > 0);

    this->windowTitle = winTitle;
    this->type   = t;
    this->xpos   = x;
    this->ypos   = y;
    this->width  = w;
    this->height = h;
    this->verticalSync = vSync;
    this->dialogBoxMode = dialogBoxMode;
    this->iconName = iconResName;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetXPos(ushort x)
{
    this->xpos = x;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nDisplayMode2::GetXPos() const
{
    return this->xpos;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetYPos(ushort y)
{
    this->ypos = y;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nDisplayMode2::GetYPos() const
{
    return this->ypos;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetWidth(ushort w)
{
    n_assert(w > 0);
    this->width = w;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nDisplayMode2::GetWidth() const
{
    n_assert(this->width > 0);
    return this->width;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetHeight(ushort h)
{
    n_assert(h > 0);
    this->height = h;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nDisplayMode2::GetHeight() const
{
    n_assert(this->height > 0);
    return this->height;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetType(Type t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
nDisplayMode2::Type
nDisplayMode2::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetWindowTitle(const char* t)
{
    this->windowTitle = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
nDisplayMode2::GetWindowTitle() const
{
    return this->windowTitle.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetVerticalSync(bool b)
{
    this->verticalSync = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nDisplayMode2::GetVerticalSync() const
{
    return this->verticalSync;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
nDisplayMode2::TypeToString(Type t)
{
    switch (t)
    {
        case Windowed:      return "windowed";
        case AlwaysOnTop:   return "alwaysontop";
        case ChildWindow:   return "childwindow";
        default:            return "fullscreen";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
nDisplayMode2::Type
nDisplayMode2::StringToType(const char* str)
{
    n_assert(str);
    if (0 == strcmp(str, "windowed"))
    {
        return Windowed;
    }
    else if (0 == strcmp(str, "alwaysontop"))
    {
        return AlwaysOnTop;
    }
    else if (0 == strcmp(str, "childwindow"))
    {
        return ChildWindow;
    }
    else
    {
        return Fullscreen;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetIcon(const char* resName)
{
    this->iconName = resName;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
nDisplayMode2::GetIcon() const
{
    return this->iconName.IsEmpty() ? 0 : this->iconName.Get();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetDialogBoxMode(bool b)
{
    this->dialogBoxMode = b;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nDisplayMode2::GetDialogBoxMode() const
{
    return this->dialogBoxMode;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nDisplayMode2::SetBpp(Bpp depth)
{
    this->bitsPerPixel = depth;
}

//------------------------------------------------------------------------------
/**
*/
inline
nDisplayMode2::Bpp
nDisplayMode2::GetBpp() const
{
    return this->bitsPerPixel;
}

//------------------------------------------------------------------------------
#endif
