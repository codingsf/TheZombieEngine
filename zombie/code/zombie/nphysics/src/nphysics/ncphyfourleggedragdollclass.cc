//------------------------------------------------------------------------------
//  ncphyfourleggedragdollclass.cc
//  (C) 2005 Conjurer Services, S.A.
//------------------------------------------------------------------------------
#include "precompiled/pchnphysics.h"
#include "nphysics/ncphyfourleggedragdollclass.h"

//-----------------------------------------------------------------------------
nNebulaComponentClass(ncPhyFourleggedRagDollClass,ncPhysicsObjClass);

//-----------------------------------------------------------------------------
NSCRIPT_INITCMDS_BEGIN(ncPhyFourleggedRagDollClass)
    NSCRIPT_ADDCMD_COMPCLASS('DC00', void, SetNeckFrontAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC01', void, SetNeckFrontAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC02', void, SetNeckSidesAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC03', void, SetNeckSidesAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC04', void, SetShoulderUpDownAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC05', void, SetShoulderUpDownAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC06', void, SetShoulderFrontAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC07', void, SetShoulderFrontAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC08', void, SetElbowUpDownAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC09', void, SetElbowUpDownAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC10', void, SetWristUpDownAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC11', void, SetWristUpDownAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC12', void, SetWristSidesAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC13', void, SetWristSidesAngleMax, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC14', void, SetKneeUpDownAngleMin, 1, (const phyreal), 0, ());
    NSCRIPT_ADDCMD_COMPCLASS('DC15', void, SetKneeUpDownAngleMax, 1, (const phyreal), 0, ());
NSCRIPT_INITCMDS_END()


//-----------------------------------------------------------------------------

namespace {
    const phyreal pi_value( phyreal(M_PI) ); // NOTE: if pi is too exact can overflow
}

//-----------------------------------------------------------------------------
/**
    Constructor.

    history:
        - 06-May-2005   Zombie         created
*/

ncPhyFourleggedRagDollClass::ncPhyFourleggedRagDollClass() : 
    neckFrontAngleMin(phyreal(-.3)), // over x-axis
    neckFrontAngleMax(phyreal(.3)), // over x-axis
    neckSidesAngleMin(phyreal(-.3)), // over z-axis
    neckSidesAngleMax(phyreal(.3)), // over z-axis
    shoulderUpDownAngleMin(-pi_value*phyreal(.75)*phyreal(.5)), // over z-axis
    shoulderUpDownAngleMax(phyreal(.1)*phyreal(.5)), // over z-axis
    shoulderFrontAngleMin(phyreal(-.2)*phyreal(.5)), // over x-axis
    shoulderFrontAngleMax(pi_value*phyreal(.75)*phyreal(.5)), // over x-axis
    elbowUpDownAngleMin(phyreal(pi_value/2.)*phyreal(.5)), // over x-axis
    elbowUpDownAngleMax(phyreal(0)), // over x-axis
    wristUpDownAngleMin(phyreal(-pi_value/2.5)*phyreal(.5)), // over x-axis
    wristUpDownAngleMax(phyreal(pi_value/2.5)*phyreal(.5)), // over x-axis
    wristSidesAngleMin(phyreal(-.6)*phyreal(.5)), // over z-axis
    wristSidesAngleMax(phyreal(.6)*phyreal(.5)), // over z-axis
    kneeUpDownAngleMin(phyreal(0)), // over x-axis
    kneeUpDownAngleMax(phyreal(-pi_value/2.)*phyreal(.5)) // over x-axis
{
};

//-----------------------------------------------------------------------------
/**
    Destructor.

    history:
        - 06-May-2005   Zombie         created
*/
ncPhyFourleggedRagDollClass::~ncPhyFourleggedRagDollClass()
{
}

//-----------------------------------------------------------------------------
/**
    Returns the neck front minimun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetNeckFrontAngleMin() const
{
    return this->neckFrontAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck front maximun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetNeckFrontAngleMax() const
{
    return this->neckFrontAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck sides minumun angle (z-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetNeckSidesAngleMin() const
{
    return this->neckSidesAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck sides maximun angle (z-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetNeckSidesAngleMax() const
{
    return this->neckSidesAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder up-down minimun angle (z-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetShoulderUpDownAngleMin() const
{
    return this->shoulderUpDownAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder up-down maximun angle (z-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetShoulderUpDownAngleMax() const
{
    return this->shoulderUpDownAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder front minimun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetShoulderFrontAngleMin() const
{
    return this->shoulderFrontAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder front maximun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetShoulderFrontAngleMax() const
{
    return this->shoulderFrontAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the elbow up-down minimun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetElbowUpDownAngleMin() const
{
    return this->elbowUpDownAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the elbow up-down maximun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetElbowUpDownAngleMax() const
{
    return this->elbowUpDownAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist up-down minimun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetWristUpDownAngleMin() const
{
    return this->wristUpDownAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist up-down maximun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetWristUpDownAngleMax() const
{
    return this->wristUpDownAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist sides minimun angle (y-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetWristSidesAngleMin() const
{
    return this->wristSidesAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist sides maximun angle (y-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetWristSidesAngleMax() const
{
    return this->wristSidesAngleMax;
}

//-----------------------------------------------------------------------------
/**
    Returns the knee up-down minimun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetKneeUpDownAngleMin() const
{
    return this->kneeUpDownAngleMin;
}

//-----------------------------------------------------------------------------
/**
    Returns the knee up-down maximun angle (x-axis).

    @return angle

    history:
        - 09-May-2005   Zombie         created
*/
phyreal ncPhyFourleggedRagDollClass::GetKneeUpDownAngleMax() const
{
    return this->kneeUpDownAngleMax;
}


//-----------------------------------------------------------------------------
/**
    Save state of the component.

    history:
        - 06-May-2005   Zombie         created
*/
bool ncPhyFourleggedRagDollClass::SaveCmds(nPersistServer *server)
{
    if( !nComponentClass::SaveCmds( server ) )
    {
        return false;
    }

    nCmd *cmd(0);

    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC00' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetNeckFrontAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC01' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetNeckFrontAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC02' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetNeckSidesAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC03' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetNeckSidesAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC04' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetShoulderUpDownAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC05' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetShoulderUpDownAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC06' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetShoulderFrontAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC07' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetShoulderFrontAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC08' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetElbowUpDownAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC09' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetElbowUpDownAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC10' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetWristUpDownAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC11' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetWristUpDownAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC12' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetWristSidesAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC13' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetWristSidesAngleMax() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC14' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetKneeUpDownAngleMin() );

    server->PutCmd(cmd);        
    ///------
    cmd = server->GetCmd( this->GetEntityClass(), 'DC15' );

    n_assert2( cmd, "Failed to find command." );

    cmd->In()->SetF( this->GetKneeUpDownAngleMax() );

    server->PutCmd(cmd);        

    return true;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck front minimun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetNeckFrontAngleMin( const phyreal value )
{
    this->neckFrontAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck front maximun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetNeckFrontAngleMax( const phyreal value )
{
    this->neckFrontAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck sides minumun angle (z-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetNeckSidesAngleMin( const phyreal value )
{
    this->neckSidesAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the neck sides maximun angle (z-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetNeckSidesAngleMax( const phyreal value )
{
    this->neckSidesAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder up-down minimun angle (z-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetShoulderUpDownAngleMin( const phyreal value )
{
    this->shoulderUpDownAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder up-down maximun angle (z-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetShoulderUpDownAngleMax( const phyreal value )
{
    this->shoulderUpDownAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder front minimun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetShoulderFrontAngleMin( const phyreal value )
{
    this->shoulderFrontAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the shoulder front maximun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetShoulderFrontAngleMax( const phyreal value )
{
    this->shoulderFrontAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the elbow up-down minimun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetElbowUpDownAngleMin( const phyreal value )
{
    this->elbowUpDownAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the elbow up-down maximun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetElbowUpDownAngleMax( const phyreal value )
{
    this->elbowUpDownAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist up-down minimun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetWristUpDownAngleMin( const phyreal value )
{
    this->wristUpDownAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist up-down maximun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetWristUpDownAngleMax( const phyreal value )
{
    this->wristUpDownAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist sides minimun angle (y-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetWristSidesAngleMin( const phyreal value )
{
    this->wristSidesAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the wrist sides maximun angle (y-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetWristSidesAngleMax( const phyreal value )
{
    this->wristSidesAngleMax = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the knee up-down minimun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetKneeUpDownAngleMin( const phyreal value )
{
    this->kneeUpDownAngleMin = value;
}

//-----------------------------------------------------------------------------
/**
    Returns the knee up-down maximun angle (x-axis).

    @param value angle

    history:
        - 09-May-2005   Zombie         created
*/
void ncPhyFourleggedRagDollClass::SetKneeUpDownAngleMax( const phyreal value )
{
    this->kneeUpDownAngleMax = value;
}

#ifdef __ZOMBIE_EXPORTER__
//-----------------------------------------------------------------------------
/**
*/
void 
ncPhyFourleggedRagDollClass::SetScavengerAngles()
{
    this->neckFrontAngleMin = phyreal(-.1); // over x-axis
    this->neckFrontAngleMax = phyreal(.1); // over x-axis
    this->neckSidesAngleMin = phyreal(-0.1); // over z-axis
    this->neckSidesAngleMax = phyreal(0.1); // over z-axis
    this->shoulderUpDownAngleMin = phyreal(-.05); // over z-axis
    this->shoulderUpDownAngleMax = phyreal(.05); // over z-axis
    this->shoulderFrontAngleMin = phyreal(-.10); // over x-axis
    this->shoulderFrontAngleMax = phyreal(.10); // over x-axis
    this->elbowUpDownAngleMin = phyreal(0); // over x-axis
    this->elbowUpDownAngleMax = phyreal(0.4); // over x-axis
    this->wristUpDownAngleMin = phyreal(-.05); // over x-axis
    this->wristUpDownAngleMax = phyreal(.05); // over x-axis
    this->wristSidesAngleMin = phyreal(-.3); // over z-axis
    this->wristSidesAngleMax = phyreal(.3); // over z-axis
    this->kneeUpDownAngleMin = phyreal(-0.4); // over x-axis
    this->kneeUpDownAngleMax = phyreal(0); // over x-axis
}
#endif