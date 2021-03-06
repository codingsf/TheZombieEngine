//------------------------------------------------------------------------------
//  nfile.cc
//  (C) 2002 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "precompiled/pchnkernel.h"
#include "kernel/nfile.h"
#include "kernel/nfileserver2.h"
#include "kernel/nlogclass.h"

#if defined(__LINUX__) || defined(__MACOSX__)
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

NCREATELOGLEVEL(file, "File Operation", false, 0);

//------------------------------------------------------------------------------
/**
    history:
     - 30-Jan-2002   peter   created
     - 11-Feb-2002   floh    Linux stuff
*/
nFile::nFile() :
    lineNumber(0),
    isOpen(false)
{
#ifdef __WIN32__
    this->handle = 0;
#else
    this->fp = 0;
#endif
}

//------------------------------------------------------------------------------
/**

    history:
     - 30-Jan-2002   peter    created
*/
nFile::~nFile()
{
    if(this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
    Check if file exists physically on disk by opening it in read-only mode.
    Close file if it was opened.

     - 05-Jan-05   floh    Bugfix: missing GENERIC_READ access mode didn't work in Win98
*/
bool
nFile::Exists(const nString& fileName) const
{
    n_assert_return( !fileName.IsEmpty(), false );

    nString fullName = nFileServer2::Instance()->ManglePath( fileName );

#ifdef __WIN32__
    HANDLE fh = CreateFile(fullName.Get(),       // filename
                           GENERIC_READ,         // access mode
                           FILE_SHARE_READ,      // share mode
                           0,                    // security flags
                           OPEN_EXISTING,        // what to do if file doesn't exist
                           FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,  // flags'n'attributes
                           0);                   // template file
    if (fh != INVALID_HANDLE_VALUE)
    {
        CloseHandle(fh);
        return true;
    }
    else
    {
        return false;
    }
#else
    FILE* fp = fopen(fullName.Get(), "r");
    if (fp != 0)
    {
        fclose(fp);
        return true;
    }
    else
    {
        return false;
    }
#endif
    //return false;
}

//------------------------------------------------------------------------------
/**
    opens the specified file

    @param fileName     the name of the file to open
    @param accessMode   the access mode ("(r|w|a)[+]")
    @return             success

    history:
     - 30-Jan-2002   peter   created
     - 11-Feb-2002   floh    Linux stuff
*/
bool
nFile::Open(const nString& fileName, const char* accessMode)
{
    n_assert_return(!this->IsOpen(), false);
    n_assert_return(!fileName.IsEmpty(), false);
    n_assert_return(accessMode, false);

    nString mangledPath;
    if (strchr(accessMode,'w') || strchr(accessMode,'W') || strchr(accessMode,'a') || strchr(accessMode,'A'))
    {
        NLOG(file, (NLOGUSER | 0, "Opening for writing %s", fileName.Get() ));
        mangledPath = nFileServer2::Instance()->ManglePath(fileName, false);
    }
    else
    {
        NLOG(file, (NLOGUSER | 0, "Opening for reading %s", fileName.Get() ));
        mangledPath = nFileServer2::Instance()->ManglePath(fileName);
    }
    this->lineNumber = 0;

#ifdef __WIN32__
    DWORD access = 0;
    DWORD disposition = 0;
    DWORD shareMode = 0;
    const char* ptr = accessMode;
    char c;
    while( 0 != (c = *ptr++))
    {
        if ((c == 'r') || (c == 'R'))
        {
            access |= GENERIC_READ;
        }
        else if ((c == 'w') || (c == 'W'))
        {
            access |= GENERIC_WRITE;
        }
    }
    if (access & GENERIC_WRITE)
    {
        disposition = CREATE_ALWAYS;
    }
    else
    {
        disposition = OPEN_EXISTING;
        shareMode   = FILE_SHARE_READ;
    }
    this->handle = CreateFile(mangledPath.Get(),    // filename
                              access,           // access mode
                              shareMode,        // share mode
                              0,                // security flags
                              disposition,      // what to do if file doesn't exist
                              FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,  // flags'n'attributes
                              0);               // template file
    if (this->handle == INVALID_HANDLE_VALUE)
    {
        this->handle = 0;
        NLOG(file, (NLOGUSER | 0, "Could not open  %s, error %s", fileName.Get(),n_getlastsystemerror().Get() ));
        return false;
    }
#else
    this->fp = fopen(mangledPath.Get(), accessMode);
    if (!this->fp)
    {
        return false;
    }
#endif
    this->isOpen = true;
    this->fname = fileName;
    this->accessMode = accessMode;
    return true;
}

//------------------------------------------------------------------------------
/**
    closes the file

    history:
     - 30-Jan-2002   peter   created
     - 11-Feb-2002   floh    Linux stuff
*/
void
nFile::Close()
{
    n_assert(this->IsOpen());

#ifdef __WIN32__
    if (this->handle)
    {
        CloseHandle(this->handle);
        this->handle = 0;
    }
#else
    if (this->fp)
    {
        fclose(this->fp);
        this->fp = 0;
    }
#endif
    this->isOpen = false;
    this->fname.Clear();
}

//------------------------------------------------------------------------------
/**
    writes a number of bytes to the file

    @param buffer        buffer with data
    @param numBytes        number of bytes to write
    @return                number of bytes written

    history:
     - 30-Jan-2002   peter    created
     - 11-Feb-2002   floh    Linux stuff
*/
int
nFile::Write(const void* buffer, int numBytes)
{
    n_assert(this->IsOpen());

    // statistics
    nFileServer2::Instance()->AddBytesWritten(numBytes);

#ifdef __WIN32__
    DWORD written;
    WriteFile(this->handle, buffer, numBytes, &written, NULL);
    return written;
#else
    return fwrite(buffer, 1, numBytes, this->fp);
#endif
}

//------------------------------------------------------------------------------
/**
    reads a number of bytes from the file

    @param buffer          buffer for data
    @param numBytes        number of bytes to read
    @return                number of bytes read

    history:
     - 30-Jan-2002   peter    created
*/
int
nFile::Read(void* buffer, int numBytes)
{
    n_assert(this->IsOpen());

    // statistics
    nFileServer2::Instance()->AddBytesRead(numBytes);

#ifdef __WIN32__
    DWORD read;
    ReadFile(this->handle, buffer, numBytes, &read, NULL);
    return read;
#else
    return fread(buffer, 1, numBytes, this->fp);
#endif
}


//------------------------------------------------------------------------------
/**
    gets current position of file pointer

    @return          position of pointer

    history:
     - 30-Jan-2002   peter    created
*/
int
nFile::Tell() const
{
    n_assert(this->IsOpen());
#ifdef __WIN32__
    return SetFilePointer(this->handle, 0, NULL, FILE_CURRENT);
#else
    return ftell(this->fp);
#endif
}

//------------------------------------------------------------------------------
/**
    sets the file pointer to given absolute or relative position

    @param byteOffset        the offset
    @param origin            position from which to count
    @return                  success

    history:
     - 30-Jan-2002   peter    created
*/
bool
nFile::Seek(int byteOffset, nSeekType origin)
{
    n_assert(this->IsOpen());

    nFileServer2::Instance()->AddSeek();

#ifdef __WIN32__
    DWORD method = FILE_BEGIN;
    switch (origin)
    {
        case CURRENT:
            method = FILE_CURRENT;
            break;
        case START:
            method = FILE_BEGIN;
            break;
        case END:
            method = FILE_END;
            break;
    }

    DWORD ret = SetFilePointer(this->handle, (LONG)byteOffset, NULL, method);
    return (ret != 0xffffffff);
#else
    int whence = SEEK_SET;
    switch (origin)
    {
        case CURRENT:
            whence = SEEK_CUR;
            break;
        case START:
            whence = SEEK_SET;
            break;
        case END:
            whence = SEEK_END;
            break;
    }
    return (0 == fseek(this->fp, byteOffset, whence)) ? true : false;
#endif
}

//------------------------------------------------------------------------------
/**
*/
bool
nFile::Eof() const
{
    n_assert(this->IsOpen());

#ifdef __WIN32__
    DWORD fpos = SetFilePointer(this->handle,0,NULL,FILE_CURRENT);
    DWORD size = GetFileSize(this->handle,NULL);

    return (fpos == size)? true:false;
#else
    return (!feof(this->fp))? false:true;
#endif
}

//------------------------------------------------------------------------------
/**
    Returns size of file in bytes.

    @return     byte-size of file
*/
int
nFile::GetSize() const
{
    n_assert(this->IsOpen());

#ifdef __WIN32__
    return GetFileSize(this->handle, NULL);
#elif defined(__LINUX__) || defined(__MACOSX__)
    struct stat s;
    fstat(fileno(this->fp), &s);
    return s.st_size;
#else
#error "nFile::GetSize(): NOT IMPLEMENTED!"
#endif
}

//------------------------------------------------------------------------------
/**
    Returns time of last write access. The file must be opened in "read" mode
    before this function can be called!
*/
nFileTime
nFile::GetLastWriteTime() const
{
    n_assert(this->IsOpen());

#ifdef __WIN32__
    nFileTime fileTime;
    GetFileTime(this->handle, NULL, NULL, &(fileTime.time));
    return fileTime;
#elif defined(__LINUX__) || defined(__MACOSX__)
    nFileTime fileTime;
    struct stat s;
    fstat(fileno(this->fp), &s);
    fileTime.time = s.st_mtime;
    return fileTime;
#else
#error "nFile::GetLastWriteTime(): NOT IMPLEMENTED!"
#endif
}

//------------------------------------------------------------------------------
/**
    writes a string to the file

    @param buffer        the string to write
    @return              success

    history:
     - 30-Jan-02   peter   created
     - 29-Jan-03   floh    the method suddenly wrote a newLine. WRONG!
*/
bool
nFile::PutS(const char* buffer)
{
    n_assert(this->IsOpen());
    if (buffer)
    {
        int len = static_cast<int>( strlen(buffer) );
        int written = this->Write(buffer, len);
        if (written != len)
        {
            return false;
        }
        else
        {
            this->lineNumber++;
            return true;
        }
    } 
    return true;
}

//------------------------------------------------------------------------------
/**
    reads a string from the file up to and including the first newline character
    or up to the end of the buffer

    @param buffer            buffer for string
    @param numChars          maximum number of chars to read
    @return                  success (false if eof is reached)

    history:
     - 30-Jan-2002   peter    created
*/
bool
nFile::GetS(char* buffer, int numChars)
{
    n_assert(this->IsOpen());
    n_assert(buffer);
    n_assert(numChars > 0);

    // store start filepointer position
    int seekPos = this->Tell();

    // read 64 bytes at once, and scan for newlines
    const int chunkSize = 64;
    int readSize  = chunkSize;
    char* readPos = buffer;

    bool retval = false;
    int bytesRead = 0;
    int curIndex = 0;
    for (curIndex = 0; curIndex < (numChars - 1); curIndex++)
    {
        // read next chunk of data?
        if (0 == (curIndex % chunkSize))
        {
            readSize = chunkSize;
            if ((curIndex + readSize) >= numChars)
            {
                readSize = numChars - curIndex;
            }
            bytesRead = this->Read(readPos, readSize);
            readPos[bytesRead] = 0;
            readPos += readSize;
        }

        // end of line reached?
        if (0 == bytesRead)
        {
            retval = false;
            break;
        }

        // newline?
        if ((buffer[curIndex] == '\n') || (buffer[curIndex] == 0))
        {
            retval = true;
            this->Seek(seekPos + curIndex + 1, START);
            break;
        }
    }

    // terminate buffer
    buffer[curIndex] = 0;
    this->lineNumber++;
    return retval;
}

//------------------------------------------------------------------------------
/**
    Append the contents of another file to this file. This and the 'other' file
    must both be open! Returns number of bytes copied.
    Warning: current implementation reads the complete source file
    into a ram buffer.

    @return     number of bytes appended
*/
int
nFile::AppendFile(nFile* other, int inputNumBytes)
{
    n_assert(other);

    int numBytes(inputNumBytes >= 0 ?  inputNumBytes : other->GetSize() );

    if (numBytes == 0)
    {
        // nothing to do
        return 0;
    }

    // allocate temp buffer and read bytes
    void* buffer = n_malloc(numBytes);
    n_assert(buffer);
    n_verify( other->Read(buffer, numBytes) == numBytes);

    // write to this file
    n_verify( this->Write(buffer, numBytes) == numBytes);

    // cleanup
    n_free(buffer);
    return numBytes;
}

//------------------------------------------------------------------------------
/**
    Return the filename of currently opened file
    @return     filename string
*/
const nString & 
nFile::GetFilename() const
{
    return this->fname;
}

//------------------------------------------------------------------------------
/**
    Return the access mode in which the file was opened
    @return     access mode string
*/
const nString & 
nFile::GetAccessMode() const
{
    return this->accessMode;
}
