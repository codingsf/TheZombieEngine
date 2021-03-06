#include "precompiled/pchnscene.h"
//------------------------------------------------------------------------------
//  nsurfacenode_cmds.cc
//  (C) 2002 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "nscene/nsurfacenode.h"
#include "kernel/npersistserver.h"

static void n_setshader(void* slf, nCmd* cmd);
static void n_getshader(void* slf, nCmd* cmd);
static void n_getnumshaders(void* slf, nCmd* cmd);
static void n_getshaderat(void* slf, nCmd* cmd);

//------------------------------------------------------------------------------
/**
    @scriptclass
    nsurfacenode

    @cppclass
    nSurfaceNode

    @superclass
    nabstractshadernode

    @classinfo
    A surface node defines a shader and associated shader variables. Note
    that a nsurfacenode itself cannot render, it must be attached to some
    geometry node that can, namely, a shape node.
*/
void
n_initcmds_nSurfaceNode(nClass* cl)
{
    cl->BeginCmds();
    cl->AddCmd("v_setshader_ss",    'SSHD', n_setshader);
    cl->AddCmd("s_getshader_s",     'GSHD', n_getshader);
    cl->AddCmd("i_getnumshaders_v", 'GNSD', n_getnumshaders);
    cl->AddCmd("ss_getshaderat_i",  'GSAT', n_getshaderat);
    cl->EndCmds();
}

//------------------------------------------------------------------------------
/**
    @cmd
    setshader
    @input
    s(FourCC), s(ShaderResource)
    @output
    v
    @info
    Set the name of the shader resource associated with a fourcc code.
*/
static void
n_setshader(void* slf, nCmd* cmd)
{
    nSurfaceNode* self = (nSurfaceNode*) slf;
    const char* s0 = cmd->In()->GetS();
    const char* s1 = cmd->In()->GetS();
    self->SetShader(nVariableServer::StringToFourCC(s0), s1);
}

//------------------------------------------------------------------------------
/**
    @cmd
    getshader
    @input
    s(FourCC)
    @output
    s(ShaderResource)
    @info
    Get the name of the shader resource associated with a fourcc code.
*/
static void
n_getshader(void* slf, nCmd* cmd)
{
    nSurfaceNode* self = (nSurfaceNode*) slf;
    cmd->Out()->SetS(self->GetShader(nVariableServer::StringToFourCC(cmd->In()->GetS())));
}

//------------------------------------------------------------------------------
/**
    @cmd
    getnumshaders
    @input
    v
    @output
    i(NumShaders)
    @info
    Get number of shaders in the object.
*/
static void
n_getnumshaders(void* slf, nCmd* cmd)
{
    nSurfaceNode* self = (nSurfaceNode*) slf;
    cmd->Out()->SetI(self->GetNumShaders());
}

//------------------------------------------------------------------------------
/**
    @cmd
    getshaderat
    @input
    i(Index)
    @output
    s(FourCC), s(ResourceName)
    @info
    Get fourcc code and resource name of shader at given index.
*/
static void
n_getshaderat(void* slf, nCmd* cmd)
{
    nSurfaceNode* self = (nSurfaceNode*) slf;
    uint i0;
    const char* s0;
    char buf[5];
    self->GetShaderAt(cmd->In()->GetI(), i0, s0);
    cmd->Out()->SetS(nVariableServer::FourCCToString(i0, buf, sizeof(buf)));
    cmd->Out()->SetS(s0);
}

//------------------------------------------------------------------------------
/**
*/
bool
nSurfaceNode::SaveCmds(nPersistServer* ps)
{
    if (nAbstractShaderNode::SaveCmds(ps))
    {
        nCmd* cmd;
        
        //--- setshader ---
        int i;
        int numShaders = this->GetNumShaders();
        for (i = 0; i < numShaders; i++)
        {
            nFourCC fourcc;
            const char* name;
            char buf[5];
            
            this->GetShaderAt(i, fourcc, name);
            cmd = ps->GetCmd(this, 'SSHD');
            cmd->In()->SetS(nVariableServer::FourCCToString(fourcc, buf, sizeof(buf)));
            cmd->In()->SetS(name);
            
            ps->PutCmd(cmd);
        }
        return true;
    }
    return false;
}
