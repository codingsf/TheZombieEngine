#include "precompiled/pchnnebula.h"
//------------------------------------------------------------------------------
//  nbinscriptserver_main.cc
//  (C) 2002 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "kernel/nfileserver2.h"
#include "kernel/nfile.h"
#include "util/nstring.h"
#include "script/nbinscriptserver.h"

nNebulaClass(nBinScriptServer, "nscriptserver");

//------------------------------------------------------------------------------
/**
*/
nBinScriptServer::nBinScriptServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nBinScriptServer::~nBinScriptServer()
{
    // empty
}

//------------------------------------------------------------------------------
/**
    Begin writing a persistent object.

    27-Feb-04   Johannes    added for nMax: check for already existing file and 
                            delete before creating the new
*/
bool
nBinScriptServer::BeginWrite(nFile * file, nObject* obj, nCmd * cmd)
{
    n_assert(file);
    n_assert(obj);

    // write magic number
    this->PutInt(file, 'NOB0');

    // write parser class and wrapper object class
    char buf[N_MAXPATH];
    if (cmd)
    {
        nString cmdStr = nKernelServer::Instance()->GetPersistServer()->MakeNewCmdString(cmd);
        snprintf(buf, sizeof(buf), "$parser:nluaserver$ $class:%s$", cmdStr.Get());
    }
    else
    {
        snprintf(buf, sizeof(buf), "$parser:nbinscriptserver$ $class:%s$", obj->GetClass()->GetName());
    }
    this->PutString(file, buf);

    return true;
}

//------------------------------------------------------------------------------
/**
    Finish writing a persistent object.
*/
bool
nBinScriptServer::EndWrite(nFile* file)
{
    n_assert(file);
    return true;
}

//------------------------------------------------------------------------------
/**
    Write start of persistent object without constructor, only the
    select statement.
*/
#if 0
bool
nBinScriptServer::WriteBeginSelObject(nFile* file, nRoot* obj, nRoot* owner)
{
    n_assert(file);
    n_assert(obj);
    n_assert(owner);

    this->WriteSelect(file, obj, owner, this->GetSelectMethod());

    return true;
}
#endif

//------------------------------------------------------------------------------
/**
    Write a select statement which changes the cwd from 'obj1' to 'obj0'.
*/
void
nBinScriptServer::WriteSelect(nFile* file, nRoot* obj0, nRoot* obj1, nScriptServer::SelectMethod selMethod)
{
    n_assert(file);
    n_assert(obj0);
    n_assert(obj1);

    switch (selMethod)
    {
        case SELCOMMAND:
            {
                // get relative path from obj1 to obj0 and write select statement
                nString relPath = obj1->GetRelPath(obj0);
                this->PutInt(file, '_sel');
                this->PutString(file, relPath.Get());
            }
            break;

        case NOSELCOMMAND:
            break;
    }
}

//------------------------------------------------------------------------------
/**
    Returns the length in bytes which the nCmd's arguments would take
    in the persistent object file.
*/
int
nBinScriptServer::GetArgLength(nCmd* cmd)
{
    int len = 0;

    cmd->Rewind();
    int numArgs = cmd->GetNumInArgs();
    int i;
    for (i = 0; i < numArgs; i++)
    {
        nArg* arg = cmd->In();

        switch (arg->GetType())
        {
            case nArg::Int:
                len += sizeof(int);
                break;

            case nArg::Float:
                len += sizeof(float);
                break;

            case nArg::String:
                len += static_cast<int>( strlen(arg->GetS()) + sizeof(ushort) );
                break;

            case nArg::Bool:
                len += sizeof(char);
                break;

            case nArg::Object:
                {
                    nRoot* obj = (nRoot*) arg->GetO();
                    n_assert(obj);
                    nString str = obj->GetFullName();
                    len += str.Length() + sizeof(ushort);
                }
                break;

            case nArg::Void:
                break;

            default:
                n_error("nBinScriptServer::GetArgLength(): unsupported data type!");
                break;
        }
    }
    n_assert(len < (1<<15));
    return len;
}

//------------------------------------------------------------------------------
/**
    Write a nCmd object to the file.
*/
bool
nBinScriptServer::WriteCmd(nFile* file, nCmd* cmd)
{
    n_assert(file);
    n_assert(cmd);

    // write cmd fourcc
    this->PutInt(file, cmd->GetProto()->GetId());

    // get summed byte length of arguments, and write length (needed to
    // skip cmd if the target object doesn't know about the command)
    this->PutShort(file, static_cast<short>( this->GetArgLength(cmd) ) );

    // write command args
    cmd->Rewind();
    int numArgs = cmd->GetNumInArgs();
    int i;
    for (i = 0; i < numArgs; i++)
    {
        nArg* arg = cmd->In();

        switch (arg->GetType())
        {
            case nArg::Int:
                this->PutInt(file, arg->GetI());
                break;

            case nArg::Float:
                this->PutFloat(file, arg->GetF());
                break;

            case nArg::String:
                this->PutString(file, arg->GetS());
                break;

            case nArg::Bool:
                this->PutBool(file, arg->GetB());
                break;

            case nArg::Object:
                this->PutObject(file, (nRoot*) arg->GetO());
                break;

            case nArg::Void:
                break;

            default:
                n_error("nBinScriptServer::WriteCmd(): unsupported data type!");
                break;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Write an 32 bit int to the file.
*/
void
nBinScriptServer::PutInt(nFile* file, int val)
{
    n_assert(file);
    file->Write(&val, sizeof(int));
}

//------------------------------------------------------------------------------
/**
    Write a 16 bit int to the file
*/
void
nBinScriptServer::PutShort(nFile* file, short val)
{
    n_assert(file);
    file->Write(&val, sizeof(short));
}

//------------------------------------------------------------------------------
/**
    Write an float to the file.
*/
void
nBinScriptServer::PutFloat(nFile* file, float val)
{
    n_assert(file);
    file->Write(&val, sizeof(float));
}

//------------------------------------------------------------------------------
/**
    Write a string to the file.
*/
void
nBinScriptServer::PutString(nFile* file, const char* str)
{
    n_assert(file);
    n_assert(str);

    // write string length
    ushort strLen = static_cast<ushort>( strlen(str) );
    file->Write(&strLen, sizeof(ushort));

    // write string
    file->Write(str, strLen);
}

//------------------------------------------------------------------------------
/**
    Write a bool to the file.
*/
void
nBinScriptServer::PutBool(nFile* file, bool b)
{
    n_assert(file);

    char c = b ? 1 : 0;
    file->Write(&c, sizeof(char));
}

//------------------------------------------------------------------------------
/**
    Write object handle to the file.
*/
void
nBinScriptServer::PutObject(nFile* file, nRoot* obj)
{
    n_assert(file);
    if (obj)
    {
        nString str = obj->GetFullName();
        this->PutString(file, str.Get());
    }
    else
    {
        this->PutString(file, "null");
    }
}

//------------------------------------------------------------------------------
/**
    Read a 32 bit int from the file.

    @param  file    [in]  nFile object to read from
    @param  val     [out] read value
    @return         false if EOF reached
*/
bool
nBinScriptServer::GetInt(nFile* file, int& val)
{
    n_assert(file);
    int bytesRead = file->Read(&val, sizeof(int));
    return sizeof(int) == bytesRead;
}

//------------------------------------------------------------------------------
/**
    Read a 16 bit short from the file.

    @param  file    [in]  nFile object to read from
    @param  val     [out] read value
    @return         false if EOF reached
*/
bool
nBinScriptServer::GetShort(nFile* file, short& val)
{
    n_assert(file);
    int bytesRead = file->Read(&val, sizeof(short));
    return sizeof(short) == bytesRead;
}

//------------------------------------------------------------------------------
/**
    Read a float from the file.

    @param  file    [in]  nFile object to read from
    @param  val     [out] read value
    @return         false if EOF reached
*/
bool
nBinScriptServer::GetFloat(nFile* file, float& val)
{
    n_assert(file);
    int bytesRead = file->Read(&val, sizeof(float));
    return sizeof(float) == bytesRead;
}

//------------------------------------------------------------------------------
/**
    Read a string from the file.

    @param  file    [in]  nFile object to read from
    @param  val     [out] read value
    @return         false if EOF reached
*/
bool
nBinScriptServer::GetString(nFile* file, nString& val)
{
    n_assert(file);
    int bytesRead;

    // read length of string
    ushort strLen;
    bytesRead = file->Read(&strLen, sizeof(ushort));
    if (sizeof(ushort) != bytesRead)
    {
        return false;
    }

    // read string
    char* buf = n_new_array(char, strLen + 1);
    bytesRead = file->Read(buf, strLen);
    buf[bytesRead] = 0;
    if (bytesRead == strLen)
    {
        // copy string to embedded string object and clean up
        val.Set(buf);
    }
    else
    {
        val.Set(0);
    }
    n_delete_array(buf);
    return (bytesRead == strLen);
}

//------------------------------------------------------------------------------
/**
    Read a bool from the file.

    @param  file    [in]  nFile object to read from
    @param  val     [out] read value
    @return         false if EOF reached
*/
bool
nBinScriptServer::GetBool(nFile* file, bool& val)
{
    n_assert(file);
    char c;
    int bytesRead = file->Read(&c, sizeof(char));
    val = (0 == c) ? false : true;
    return (sizeof(char) == bytesRead);
}

//------------------------------------------------------------------------------
/**
    Read an object handle from the file.

    @param  file    [in]  nFile object to read from
    @param  val     [out] read value
    @return         false if EOF reached
*/
bool
nBinScriptServer::GetObj(nFile* file, nRoot*& val)
{
    n_assert(file);

    // get object string handle from file
    nString objHandle;
    if (this->GetString(file, objHandle))
    {
        if (strcmp(objHandle.Get(), "null") == 0)
        {
            // special case null object
            val = 0;
        }
        else
        {
            // lookup object
            val = kernelServer->Lookup(objHandle.Get());
        }
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**    
    Verify that this is a NOB0 file, and skip the header. Return false
    if EOF reached for some reason.
*/
bool
nBinScriptServer::GetHeader(nFile* file)
{
    n_assert(file);

    // read and verify the magic number
    int magic;
    if (this->GetInt(file, magic))
    {
        if ('NOB0' != magic)
        {
            n_printf("nBinScriptServer::GetHeader(): not a binary Nebula persistent object file!\n");
            return false;
        }

        // skip the header string
        nString dummyStr;
        return this->GetString(file, dummyStr);
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------------
/**    
    Read input args from file and write to nCmd object
    
    @param  file    file to read from
    @param  cmd     nCmd object to initialize
    @return         false if eof reached
*/
bool
nBinScriptServer::GetInArgs(nFile* file, nCmd* cmd)
{
    n_assert(file);
    n_assert(cmd);

    int i;
    int iArg;
    float fArg;
    nString sArg;
    nString cArg;
    bool bArg;
    nRoot* oArg;

    cmd->Rewind();
    int numArgs = cmd->GetNumInArgs();
    for (i = 0; i < numArgs; i++)
    {
        bool notEof = true;
        nArg* arg = cmd->In();

        switch(arg->GetType())
        {
        
            case nArg::Int:
                notEof = this->GetInt(file, iArg);
                arg->SetI(iArg);
                break;

            case nArg::Float:
                notEof = this->GetFloat(file, fArg);
                arg->SetF(fArg);
                break;

            case nArg::String:
                notEof = this->GetString(file, sArg);
                arg->SetS(sArg.Get());
                break;

            case nArg::Bool:
                notEof = this->GetBool(file, bArg);
                arg->SetB(bArg);
                break;

            case nArg::Object:
                notEof = this->GetObj(file, oArg);
                arg->SetO(oArg);
                break;

            case nArg::Void:
                break;

            default:
                n_error("nBinScriptServer::GetInArgs(): unsupported data type!");
                break;
        }

        if (!notEof)
        {
            // eof reached
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**    
    Read and execute a cmd block. This may create new objects and change
    Nebula's currently selected object.

    @param  file    file to read from
    @return         false if EOF reached
*/
bool
nBinScriptServer::ReadBlock(nFile* file)
{
    bool notEof;

    // read next fourcc, return false if EOF reached
    int fourcc;
    notEof = this->GetInt(file, fourcc);
    if (!notEof)
    {
        // eof reached
        return false;
    }

    // handle special commands '_new' and '_sel'
    if ('_new' == fourcc)
    {

        // read class and object name (necessary to COPY the strings!)
        nString objClass, objName;
        notEof = this->GetString(file, objClass);
        n_assert(notEof);
        notEof = this->GetString(file, objName);
        n_assert(notEof);

        // create object and select it
        nRoot* obj = kernelServer->New(objClass.Get(), objName.Get());
        if (obj)
        {
            kernelServer->SetCwd(obj);
        }
        else
        {
            n_error("nBinScriptServer::ReadBlock(): '_new %s %s' failed!\n",
                    objClass.Get(), objName.Get());
        }
    }
    else if ('_sel' == fourcc)
    {
        // read relative path
        nString relPath;
        notEof = this->GetString(file, relPath);
        n_assert(notEof);

        nRoot* obj = kernelServer->Lookup(relPath.Get());
        if (obj)
        {
            kernelServer->SetCwd(obj);
        }
        else
        {
            n_error("nBinScriptServer::ReadBlock(): '_sel %s' failed!\n",
                    relPath.Get());
        }
    }
    else
    {
        // this is a normal cmd
        nObject* obj = nScriptServer::GetCurrentTargetObject(); // use the current nObject if one is set
        if (!obj)
            obj = nKernelServer::Instance()->GetCwd(); // otherwise use the current nRoot
        n_assert(obj);
        nClass* objClass = obj->GetClass();
        nCmdProto* cmdProto = objClass->FindCmdById(fourcc);
        if (cmdProto)
        {
            nCmd* cmd = cmdProto->NewCmd();
            n_assert(cmd);

            // skip in args length
            short dummy;
            notEof = this->GetShort(file, dummy);
            n_assert(notEof);

            // read input args into cmd object
            this->GetInArgs(file, cmd);

            // invoke cmd on current object
            bool success = obj->Dispatch(cmd);
            if (!success)
            {
                if (obj->IsA("nroot"))
                {
                    n_printf("nBinScriptServer::ReadBlock(): obj '%s' of class '%s' doesn't accept cmd '%s'\n", 
                             ((nRoot *)obj)->GetFullName().Get(), 
                             obj->GetClass()->GetName(), 
                             cmdProto->GetName());
                }
                else
                {
                    n_printf("nBinScriptServer::ReadBlock(): obj of class '%s' doesn't accept cmd '%s'\n", 
                             obj->GetClass()->GetName(),
                             cmdProto->GetName());
                }
            }
            cmdProto->RelCmd(cmd);
        }
        else
        {
            // the object doesn't know the command, skip it
            short argLen;
            notEof = this->GetShort(file, argLen);
            n_assert(notEof);
            notEof = file->Seek(argLen, nFile::CURRENT);
            n_assert(notEof);
        }
    }

    return true;
}

//------------------------------------------------------------------------------
/**    
    Evaluate a NOB0 file.
*/
bool
nBinScriptServer::RunScript(nFile * file, nString& result)
{
    n_assert(file);
    n_assert(file->IsOpen());
    n_assert(strchr(file->GetAccessMode().Get(),'b') != 0);

    bool retval = false;
    result.Clear();

    // verify header
    if (!this->GetHeader(file))
    {
        n_printf("nBinScriptServer::RunScript(): '%s' not a NOB0 file!\n", file->GetFilename().Get());
        return false;
    }

    // push cwd
    kernelServer->PushCwd(kernelServer->GetCwd());

    // read and execute blocks
    while (this->ReadBlock(file));

    // pop cwd
    kernelServer->PopCwd();

    retval = true;

    return retval;
}

//------------------------------------------------------------------------------

