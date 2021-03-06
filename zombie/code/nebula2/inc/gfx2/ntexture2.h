#ifndef N_TEXTURE2_H
#define N_TEXTURE2_H
//------------------------------------------------------------------------------
/**
    @class nTexture2
    @ingroup Gfx2

    Contains image data used by the gfx api's texture samplers. Textures
    are normal named, shared resources which are usually loaded from disk.
    Textures can also be declared as render targets.

    The following code snip shows the creation of 16bit empty texture and
    filling it with white pixel:
    @code
    tex = (nTexture2 *)refGfx2->NewTexture("mytexture");
    if (!tex->IsUnloaded())
    {
        int width, height;
        width = height = 128;
        // create a 16bit 2D empty texture.
        tex->SetUsage(nTexture2::CreateEmpty);
        tex->SetType(nTexture2::TEXTURE_2D);
        tex->SetWidth(width);
        tex->SetHeight(height);
        tex->SetFormat(nTexture2::A1R5G5B5);
        tex->Load();

        // fill the created texture with white pixel.
        struct nTexture2::LockInfo surf;
        if (tex->Lock(nTexture2::WriteOnly, 0, surf))
        {
            unsigned short *surface = (unsigned short *)surf.surfPointer;
            for (unsigned int pixelByte=0; pixelByte < width*height; pixelByte++)
                surface[pixelByte] = 0xffff;
            tex->Unlock(0);
        }
    }
    @endcode

    You also can read pixel of the texture like the following:
    @code
    struct nTexture2::LockInfo surf;
    tex->Lock(nTexture2::ReadOnly, 0, surf)
    ushort *surface = (ushort*)surf.surfPointer;
    ushort color = surface[x + y*surf.surfPitch];
    tex->Unlock(0);  
    @endcode

    The following code shows that another way of copying the image data of the memory 
    to the created texture:
    @code
    // create an empty texture like above code.
    ...

    // get the surface of the texture
    nSurface *surface; 

    // get the surface of level 0.
    tex->GetSurfaceLevel("/tmp/surface", 0, &surface);

    // copy the imageData which is the source image to the texture
    surface->LoadFromMemory(imageData, dstFormat, width, height, imagePitch);
    ...
    @endcode

    See @ref nSurface class for more details about the surface and its usage.

    (C) 2002 RadonLabs GmbH
*/
#include "resource/nresource.h"
#include "kernel/nfile.h"

class nSurface;
class rectanglei;

//------------------------------------------------------------------------------
class nTexture2 : public nResource
{
public:
    /// texture type
    enum Type
    {
        TEXTURE_NOTYPE,
        TEXTURE_2D,                 // 2-dimensional
        TEXTURE_3D,                 // 3-dimensional
        TEXTURE_CUBE,               // cube
    };

    enum Filter
    {
        FILTER_POINT,
        FILTER_LINEAR,
        FILTER_TRIANGLE,
    };

    // pixel formats
    enum Format
    {
        NOFORMAT,
        X8R8G8B8,
        A8R8G8B8,
        R5G6B5,
        A1R5G5B5,
        A4R4G4B4,
        P8,
        G16R16,
        DXT1,
        DXT2,
        DXT3,
        DXT4,
        DXT5,
        R16F,                       // 16 bit float, red only
        G16R16F,                    // 32 bit float, 16 bit red, 16 bit green
        A16B16G16R16F,              // 64 bit float, 16 bit rgba each
        R32F,                       // 32 bit float, red only
        G32R32F,                    // 64 bit float, 32 bit red, 32 bit green
        A32B32G32R32F,              // 128 bit float, 32 bit rgba each
        A8,
		L8,
    };

    // the sides of a cube map
    enum CubeFace
    {
        PosX = 0,
        NegX,
        PosY,
        NegY,
        PosZ,
        NegZ,
    };

    // usage flags
    enum Usage
    {
        CreateEmpty = (1<<0),               // don't load from disk, instead create empty texture
        CreateFromRawCompoundFile = (1<<1), // create from a compound file as raw ARGB pixel chunk
        CreateFromDDSCompoundFile = (1<<2), // create from dds file inside a compound file
        RenderTargetColor = (1<<3),         // is render target, has color buffer
        RenderTargetDepth = (1<<4),         // is render target, has depth buffer
        RenderTargetStencil = (1<<5),       // is render target, has stencil buffer
        Dynamic = (1<<6),                   // is a dynamic texture (for write access with CPU)
    };

    // lock types
    enum LockType
    {
        ReadOnly,       // cpu will only read from texture
        WriteOnly,      // cpu will only write to texture (an overwrite everything!)
    };

    // lock information
    struct LockInfo
    {
        void* surfPointer;
        int   surfPitch;
    };

    /// constructor
    nTexture2();
    /// destructor
    virtual ~nTexture2();
    /// set combination of usage flags
    void SetUsage(ushort useFlags);
    /// get usage flags combination
    ushort GetUsage() const;
    /// check usage flags if this is a render target
    bool IsRenderTarget() const;
    /// set compound file read data
    void SetCompoundFileData(nFile* file, int filePos, int byteSize);
    /// set texture type (render target only!)
    void SetType(Type t);
    /// get texture type
    Type GetType() const;
    /// set texture's pixel format (render target only!)
    void SetFormat(Format f);
    /// get texture's pixel format
    Format GetFormat() const;
    /// set width (render target only!)
    void SetWidth(ushort w);
    /// get width
    ushort GetWidth() const;
    /// set height (render target only!)
    void SetHeight(ushort h);
    /// get height
    ushort GetHeight() const;
    /// set depth (render target only! oops: 3d render targets?)
    void SetDepth(ushort d);
    /// get depth
    ushort GetDepth() const;
    /// get number of mipmaps
    ushort GetNumMipLevels() const;
    /// get bytes per pixel (computed from pixel format)
    int GetBytesPerPixel() const;
    /// lock a 2D texture, returns pointer and pitch
    virtual bool Lock(LockType lockType, int level, LockInfo& lockInfo);
    /// lock a rectangle in a 2D texture, returns pointer and pitch
    virtual bool LockRect(LockType lockType, int level, int left, int top, int right, int bottom, LockInfo& lockInfo);
    /// unlock 2D texture
    virtual void Unlock(int level);
    /// lock a cube face
    virtual bool LockCubeFace(LockType lockType, CubeFace face, int level, LockInfo& lockInfo);
    /// unlock a cube face
    virtual void UnlockCubeFace(CubeFace face, int level);
    /// convert string to pixel format
    static Format StringToFormat(const char* str);
    /// convert pixel format to string
    static const char* FormatToString(Format fmt);

    /// @name Surface supporting functions
    /// @{
    virtual void GetSurfaceLevel(const char* objName, uint level, nSurface** surface);
    virtual void GenerateMipMaps();
    /// @}

    /// debug visualization of the texture
    virtual void DisplayOnFramebuffer();
    /// copy to another compatible texture
    virtual void Copy( nTexture2* dest, const rectanglei& src , const rectanglei& dst, Filter filter );

    /// save image under different name and format
    virtual bool SaveResourceAs(const char* name, nTexture2::Format format);

protected:
    /// set number of mipmaps
    void SetNumMipLevels(ushort num);

    Type type;
    Format format;
    ushort usage;
    ushort width;
    ushort height;
    ushort depth;
    ushort numMipMaps;
    nFile* compoundFile;
    int compoundFilePos;
    int compoundFileDataSize;
};

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetCompoundFileData(nFile* file, int filePos, int byteSize)
{
    n_assert(file);
    n_assert(byteSize > 0);
    if (this->compoundFile)
    {
        this->compoundFile->Release();
        this->compoundFile = 0;
    }
    this->compoundFile = file;
    this->compoundFile->AddRef();
    this->compoundFilePos = filePos;
    this->compoundFileDataSize = byteSize;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetUsage(ushort useFlags)
{
    this->usage = useFlags;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nTexture2::GetUsage() const
{
    return this->usage;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nTexture2::IsRenderTarget() const
{
    return (0 != (this->usage & (RenderTargetColor | RenderTargetDepth | RenderTargetStencil)));
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetType(Type t)
{
    this->type = t;
}

//------------------------------------------------------------------------------
/**
*/
inline
nTexture2::Type
nTexture2::GetType() const
{
    return this->type;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetFormat(Format f)
{
    this->format = f;
}

//------------------------------------------------------------------------------
/**
*/
inline
nTexture2::Format
nTexture2::GetFormat() const
{
    return this->format;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetWidth(ushort w)
{
    this->width = w;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nTexture2::GetWidth() const
{
    return this->width;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetHeight(ushort h)
{
    this->height = h;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nTexture2::GetHeight() const
{
    return this->height;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetDepth(ushort d)
{
    this->depth = d;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nTexture2::GetDepth() const
{
    return this->depth;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::SetNumMipLevels(ushort num)
{
    this->numMipMaps = num;
}

//------------------------------------------------------------------------------
/**
*/
inline
ushort
nTexture2::GetNumMipLevels() const
{
    return this->numMipMaps;
}

//------------------------------------------------------------------------------
/**
    Returns the bytes per pixel for the current pixel format. May be
    incorrect for compressed textures!

    - 17-Jun-05    kims    Added 'A8' to return its bytes per pixel.
*/
inline
int
nTexture2::GetBytesPerPixel() const
{
    switch (this->format)
    {
        case X8R8G8B8:  
        case A8R8G8B8:
            return 4;

        case R5G6B5:
        case A1R5G5B5:
        case A4R4G4B4:
            return 2;

		case L8:
        case P8:
        case A8:
            return 1;

        case DXT1:
        case DXT2:
        case DXT3:
        case DXT4:
        case DXT5:
            n_error("nTexture2::GetBytesPerPixel(): compressed pixel format!");
            return 1;

        case R16F:
            return 2;
 
        case G16R16:
        case G16R16F:
            return 4;

        case A16B16G16R16F:
            return 8;

        case R32F:
            return 4;

        case G32R32F:
            return 8;

        case A32B32G32R32F:
            return 16;
        
        default:
            n_error("nTexture2::GetBytesPerPixel(): invalid pixel format!");
            return 1;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nTexture2::DisplayOnFramebuffer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
nTexture2::FormatToString(Format fmt)
{
    switch (fmt)
    {
        case NOFORMAT:      return "NOFORMAT";
        case X8R8G8B8:      return "X8R8G8B8";
        case A8R8G8B8:      return "A8R8G8B8";
        case R5G6B5:        return "R5G6B5";
        case A1R5G5B5:      return "A1R5G5B5";
        case A4R4G4B4:      return "A4R4G4B4";
        case P8:            return "P8";
        case G16R16:        return "G16R16";
        case DXT1:          return "DXT1";
        case DXT2:          return "DXT2";
        case DXT3:          return "DXT3";
        case DXT4:          return "DXT4";
        case DXT5:          return "DXT5";
        case R16F:          return "R16F";
        case G16R16F:       return "G16R16F";
        case A16B16G16R16F: return "A16B16G16R16F";
        case R32F:          return "R32F";
        case G32R32F:       return "G32R32F";
        case A32B32G32R32F: return "A32B32G32R32F";
        case A8:            return "A8";
	    case L8:            return "L8";
        default:            return "NOFOUND";
    }
    //return 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
nTexture2::Format
nTexture2::StringToFormat(const char* str)
{
    n_assert(str);
    if (0 == strcmp("NOFORMAT", str))           return NOFORMAT;
    else if (0 == strcmp("X8R8G8B8", str))      return X8R8G8B8;
    else if (0 == strcmp("A8R8G8B8", str))      return A8R8G8B8;
    else if (0 == strcmp("R5G6B5", str))        return R5G6B5;
    else if (0 == strcmp("A1R5G5B5", str))      return A1R5G5B5;
    else if (0 == strcmp("A4R4G4B4", str))      return A4R4G4B4;
    else if (0 == strcmp("P8", str))            return P8;
	else if (0 == strcmp("L8", str))            return L8;
    else if (0 == strcmp("G16R16", str))        return G16R16;
    else if (0 == strcmp("DXT1", str))          return DXT1;
    else if (0 == strcmp("DXT2", str))          return DXT2;
    else if (0 == strcmp("DXT3", str))          return DXT3;
    else if (0 == strcmp("DXT4", str))          return DXT4;
    else if (0 == strcmp("DXT5", str))          return DXT5;
    else if (0 == strcmp("R16F", str))          return R16F;
    else if (0 == strcmp("G16R16F", str))       return G16R16F;
    else if (0 == strcmp("A16B16G16R16F", str)) return A16B16G16R16F;
    else if (0 == strcmp("R32F", str))          return R32F;
    else if (0 == strcmp("G32R32F", str))       return G32R32F;
    else if (0 == strcmp("A32B32G32R32F", str)) return A32B32G32R32F;
    else if (0 == strcmp("A8", str))            return A8;
    else
    {
        n_error("nTexture2::StringToFormat(): invalid string '%s'", str);
        return NOFORMAT;
    }
}

//------------------------------------------------------------------------------
#endif    

