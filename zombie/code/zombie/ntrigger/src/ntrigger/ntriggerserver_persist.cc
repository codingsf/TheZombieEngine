//-----------------------------------------------------------------------------
//  ntriggerserver_persist.cc
//  (C) 2005 Conjurer Services, S.A.
//-----------------------------------------------------------------------------
#include "precompiled/pchntrigger.h"
#include "ntrigger/ntriggerserver.h"
#include "ntrigger/nscriptoperation.h"
#include "ntrigger/ntriggerstatecondition.h"

//-----------------------------------------------------------------------------
namespace
{
    /// Root path in NOH where all objects are placed
    const char* RootObjPath = "/trigger";
    /// Relative path to root where all trigger scripts are placed
    const char* TriggerScriptsSubpath = "triggerscripts";
    /// Relative path to root where all script operations are placed
    const char* ScriptOperationsSubpath = "scriptoperations";
    /// Relative path to root where all script operations are placed
    const char* TriggerConditionsSubpath = "triggerconditions";
}

//-----------------------------------------------------------------------------
/**
    Return an object, creating it if needed

    @param subpath Relative path where the object is in
    @param name Name of the object to get (matches the class name)
*/
nRoot*
nTriggerServer::GetObject( const char* subpath, const char* name ) const
{
    // Look for the object in the NOH
    nString objPath;
    this->MakeObjPath( subpath, name, objPath );
    nRoot* obj = nKernelServer::Instance()->Lookup( objPath.Get() );

    // If the object isn't in the NOH, create it
    if ( !obj )
    {
        obj = nKernelServer::Instance()->New( name, objPath.Get() );
        n_assert( obj );
    }

    return obj;
}

//-----------------------------------------------------------------------------
/**
    Return a script operation object, loading it if needed
*/
nScriptOperation*
nTriggerServer::GetScriptOperation( const char* scriptOperationName ) const
{
    return static_cast<nScriptOperation*>( this->GetObject( ScriptOperationsSubpath, scriptOperationName ) );
}

//-----------------------------------------------------------------------------
/**
    Return a trigger condition script object, loading it if needed
*/
nTriggerStateCondition*
nTriggerServer::GetTriggerCondition( const char* triggerConditionName ) const
{
    return static_cast<nTriggerStateCondition*>( this->GetObject( TriggerConditionsSubpath, triggerConditionName ) );
}

//-----------------------------------------------------------------------------
/**
    Build a NOH path from subpath and name (path = obj_root/subpath/name)
*/
void
nTriggerServer::MakeObjPath( const char* subpath, const char* name, nString& resultingPath ) const
{
    n_assert( RootObjPath );
    n_assert( subpath );
    n_assert( name );

    resultingPath = RootObjPath;
    resultingPath.StripTrailingSlash();
    resultingPath += "/";
    resultingPath += subpath;
    resultingPath.StripTrailingSlash();
    resultingPath += "/";
    resultingPath += name;
}

#ifndef NGAME

//-----------------------------------------------------------------------------
/**
    Get the number of available event types
*/
int
nTriggerServer::GetEventTypesNumber() const
{
    return nGameEvent::EVENTS_NUMBER;
}

//-----------------------------------------------------------------------------
/**
    Tell if an event may be handled by a FSM
*/
bool
nTriggerServer::IsAFsmInEvent( int eventType ) const
{
    return ( nGameEvent::GetEventScope( nGameEvent::Type(eventType) ) & nGameEvent::Info::FSM_IN ) != 0;
}

//-----------------------------------------------------------------------------
/**
    Tell if an event may be handled by an area trigger
*/
bool
nTriggerServer::IsAnAreaTriggerInEvent( int eventType ) const
{
    return ( nGameEvent::GetEventScope( nGameEvent::Type(eventType) ) & nGameEvent::Info::AREA_TRIGGER_IN ) != 0;
}

//-----------------------------------------------------------------------------
/**
    Tell if an event may be generated by an area trigger
*/
bool
nTriggerServer::IsAnAreaTriggerOutEvent( int eventType ) const
{
    return ( nGameEvent::GetEventScope( nGameEvent::Type(eventType) ) & nGameEvent::Info::AREA_TRIGGER_OUT ) != 0;
}

//-----------------------------------------------------------------------------
/**
    Tell if an event may be handled by a generic trigger
*/
bool
nTriggerServer::IsAGenericTriggerInEvent( int eventType ) const
{
    return ( nGameEvent::GetEventScope( nGameEvent::Type(eventType) ) & nGameEvent::Info::GENERIC_TRIGGER_IN ) != 0;
}

//-----------------------------------------------------------------------------
/**
    Return the persistent id of an event type
*/
const char*
nTriggerServer::GetEventPersistentId( int eventType ) const
{
    return nGameEvent::GetEventPersistentId( nGameEvent::Type(eventType) );
}

//-----------------------------------------------------------------------------
/**
    Return the transient id of an event type
*/
int
nTriggerServer::GetEventTransientId( const char* eventType ) const
{
    return nGameEvent::GetEventTransientId( eventType );
}

//-----------------------------------------------------------------------------
/**
    Return the label of an event type
*/
const char*
nTriggerServer::GetEventLabel( int eventType ) const
{
    return nGameEvent::GetEventLabel( nGameEvent::Type(eventType) );
}

//-----------------------------------------------------------------------------

#endif //NGAME
