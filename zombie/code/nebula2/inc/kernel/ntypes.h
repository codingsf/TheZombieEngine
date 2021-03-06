#ifndef N_TYPES_H
#define N_TYPES_H
//------------------------------------------------------------------------------
/**
    Lowlevel Nebula defs.

    (C) 2002 RadonLabs GmbH
*/
#ifndef __XBxX__
#include <errno.h>
#include <stdio.h>
#endif

// Uncomment this line if you want windows crash report support
#define __ERROR_CRASH_REPORT__

#include "kernel/nsystem.h"
#include "kernel/ndebug.h"
#include "kernel/ndefclass.h"

// Shortcut Typedefs
#ifdef __VC__
    typedef __int8 nint8;
    typedef __int16 nint16;
    typedef __int32 nint32;
    typedef __int64 nint64;
	typedef int __w64 nptrdiff;

    //typedef __w64 nptrdiff;

    typedef unsigned __int8 nuint8;
    typedef unsigned __int16 nuint16;
    typedef unsigned __int32 nuint32;
    typedef unsigned __int64 nuint64;
    typedef unsigned __w64 nuptrdiff;

    // define a variable as thread local storage (depends on compiler)
    #define nthread __declspec(thread)
#else
    //defined in the C99 <inttypes.h> 
    #include <inttypes.h>

    typedef int8_t nint8;
    typedef int16_t nint16;
    typedef int32_t nint32;
    typedef int64_t nint64;
    typedef __w64 nptrdiff;

    typedef uint8_t nuint8;
    typedef uint16_t nuint16;
    typedef uint32_t nuint32;
    typedef uint64_t nuint64;
    typedef unsigned __w64 nuptrdiff;

    // define a variable as thread local storage (depends on compiler)
    #define nthread __thread
#endif

typedef unsigned long  ulong;
typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef float float2[2];
typedef float float3[3];
typedef float float4[4];

//------------------------------------------------------------------------------
/**
    Typical type to type C++ template 
*/
template <class T>
struct nType
{
    typedef T type;
};

//------------------------------------------------------------------------------

struct nFloat4
{
    float x;
    float y;
    float z;
    float w;
};
struct nFloat3
{
    float x;
    float y;
    float z;
};
struct nFloat2
{
    float x;
    float y;
};
/**
    An nFourCC is usualy represented in textual form in the source code
    as a character constant comprised of 4 characters:

    @verbatim
    'HELO'@endverbatim

    These four characters are interpreted as an integer value, making
    comparisons much faster than using a string identifier, while
    retaining, roughly, the readability of a string.

    These are used in the script interface when defining a script
    command via nClass::AddCmd(), as well as identifiers in the
    nVariableServer (and related nVariableContext and nVariable)
    classes.
*/
typedef unsigned int nFourCC;
typedef double nTime;

//-----------------------------------------------------------------------------

#ifndef NULL
#define NULL (0L)
#endif

//------------------------------------------------------------------------------
#define N_MAXPATH (512)     // maximum length for complete path
#define N_MAXNAMELEN (32)   // maximum length for single path component
//------------------------------------------------------------------------------
#ifdef NGAME
#define N_IFDEF_NGAME(x) x
#else
#define N_IFDEF_NGAME(x)
#endif

//------------------------------------------------------------------------------
#ifndef NGAME
#define N_IFNDEF_NGAME(x) x
#else
#define N_IFNDEF_NGAME(x)
#endif

//------------------------------------------------------------------------------
#define nID(a,b,c,d) ((a<<24)|(b<<16)|(c<<8)|(d))
#define MAKE_FOURCC(ch0,ch1,ch2,ch3) (ch0 | ch1<<8 | ch2<<16 | ch3<<24)
#define FOURCC(i) (((i&0xff000000)>>24) | ((i&0x00ff0000)>>8) | ((i&0x0000ff00)<<8) | ((i&0x000000ff)<<24))
#define N_WHITESPACE " \r\n\t"

#if defined(__LINUX__) || defined(__MACOSX__)
#define n_stricmp strcasecmp
#else
#define n_stricmp stricmp
#endif

#ifdef __WIN32__
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

#ifdef _MSC_VER
#define va_copy(d, s) d = s
#endif

// maps unsigned 8 bits/channel to D3DCOLOR
#define N_ARGB(a,r,g,b) ((uint)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define N_RGBA(r,g,b,a) N_ARGB(a,r,g,b)
#define N_XRGB(r,g,b)   N_ARGB(0xff,r,g,b)
#define N_COLORVALUE(r,g,b,a) N_RGBA((uint)((r)*255.f),(uint)((g)*255.f),(uint)((b)*255.f),(uint)((a)*255.f))

#define N_ARRAY_SIZEOF(x) (sizeof(x)/sizeof(x[0]))

//------------------------------------------------------------------------------
//  public kernel C functions
//------------------------------------------------------------------------------
void __cdecl n_puts(const char*);
void __cdecl n_vprintf(const char*,va_list);
void __cdecl n_printf(const char *, ...)
    __attribute__((format(printf,1,2)));
#ifdef __ERROR_CRASH_REPORT__
extern bool crashReportAbortAlways  ;
bool __cdecl n_error(const char*, ...)
    __attribute__((noreturn))
    __attribute__((format(printf,1,2)));
#else
void __cdecl n_error(const char*, ...)
    __attribute__((noreturn))
    __attribute__((format(printf,2,3)));
#endif
void __cdecl n_message(const char*, ...)
    __attribute__((format(printf,1,2)));
void __cdecl n_dbgout(const char*, ...)
    __attribute__((format(printf,1,2)));
void n_sleep(double);
char *n_strdup(const char *);
char *n_strncpy2(char *, const char *, size_t);
bool n_strmatch(const char *, const char *);
void n_strcat(char *, const char *, size_t);

void n_barf(const char *, const char *, int, bool*)
    __attribute__((noreturn));
void n_barf2(const char*, const char*, const char*, int, bool*)
    __attribute__((noreturn));

void *n_dllopen(const char *);
void  n_dllclose(void *);
void *n_dllsymbol(void *, const char *);

nFourCC n_strtofourcc(const char*);
const char* n_fourcctostr(nFourCC);

class nString;
nString n_getlastsystemerror();

//------------------------------------------------------------------------------
//  Nebula memory management and debugging stuff.
//------------------------------------------------------------------------------
extern bool nMemoryLoggingEnabled;
struct nMemoryStats
{
    int highWaterSize;      // max allocated size so far
    int totalCount;         // total number of allocations
    int totalSize;          // current allocated size
};

void n_dbgmeminit();                // initialize memory debugging system
nMemoryStats n_dbgmemgetstats();    // defined in ndbgalloc.cc

#ifdef new
#undef new
#endif

#ifdef delete
#undef delete
#endif

// implemented in ndbgalloc.cc
void* operator new(size_t size);
void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size);
void* operator new[](size_t size, const char* file, int line);
void operator delete(void* p);
void operator delete(void* p, const char* file, int line);
void operator delete[](void* p);
void operator delete[](void* p, const char* file, int line);
void* n_malloc_dbg(size_t size, const char* file, int line);
void* n_calloc_dbg(size_t num, size_t size, const char* file, int line);
void* n_realloc_dbg(void* memblock, size_t size, const char* file, int line);
void n_free_dbg(void* memblock, const char* file, int line);

#if defined(_DEBUG) && defined(__WIN32__)
#define n_new(type) new(__FILE__,__LINE__) type
#define n_new_array(type,size) new(__FILE__,__LINE__) type[size]
#define n_delete(ptr) delete ptr
#define n_delete_array(ptr) delete[] ptr
#define n_malloc(size) n_malloc_dbg(size, __FILE__, __LINE__)
#define n_calloc(num, size) n_calloc_dbg(num, size, __FILE__, __LINE__)
#define n_realloc(memblock, size) n_realloc_dbg(memblock, size, __FILE__, __LINE__)
#define n_free(memblock) n_free_dbg(memblock, __FILE__, __LINE__)
#else
#define n_new(type) new type
#define n_new_array(type,size) new type[size]
#define n_delete(ptr) delete ptr
#define n_delete_array(ptr) delete[] ptr
#define n_malloc(size) malloc(size)
#define n_calloc(num, size) calloc(num, size)
#define n_realloc(memblock, size) realloc(memblock, size)
#define n_free(memblock) free(memblock)
#endif

// define an nAttribute C++ class extension, declares
// a function member, setter and getter method for the attribute
// #define __ref_attr(TYPE,NAME) private: TYPE NAME; public: void Set##NAME(const TYPE& t) {this->NAME = t; }; const TYPE& Get##NAME() const { return this->NAME; };
// #define __attr(TYPE,NAME)     private: TYPE NAME; public: void Set##NAME(TYPE t) {this->NAME = t}; TYPE Get##NAME() const { return this->NAME; };

#define nSetter(METHOD, TYPE, MEMBER) inline void METHOD(TYPE t) { this->MEMBER = t; }
#define nGetter(TYPE, METHOD, MEMBER) inline TYPE METHOD() const { return this->MEMBER; }

//--------------------------------------------------------------------
#endif
