#include "precompiled/pchconjurerapp.h"
//------------------------------------------------------------------------------
//  nconjurerapp_main.cc
//  (C) 2005 Conjurer Services, S.A.
//------------------------------------------------------------------------------
#include "conjurer/nconjurerapp.h"
#include "conjurer/nconjurersceneserver.h"
#include "conjurer/nassetloadstate.h"
#include "conjurer/npreviewviewport.h"
#include "conjurer/nviewerparams.h"
#include "conjurer/neditorviewport.h"
#include "conjurer/nglobalvariableeditor.h"
#include "napplication/nappviewportui.h"
#include "napplication/nappviewport.h"
#include "appinfo/appinfo.h"

#include "kernel/nfileserver2.h"
#include "file/nmemfile.h"
#include "kernel/nscriptserver.h"
#include "kernel/nremoteserver.h"
#include "kernel/nkernelserverproxy.h"
#include "nundo/nundoserver.h"
#include "nundo/undocmdbyscript.h"
#include "misc/nconserver.h"
#include "gfx2/ngfxutils.h"
#include "input/ninputserver.h"
#include "variable/nvariableserver.h"
#include "particle/nparticleserver.h"
#include "nscene/nscenenode.h"
#include "nscene/ntransformnode.h"
#include "ndebug/ndebugserver.h"
#include "ndebug/ndebugcomponentserver.h"
#include "tools/nmonitorserver.h"
#include "nlevel/nlevelmanager.h"
#include "nlayermanager/nlayermanager.h"
#include "nfsmserver/nfsmserver.h"
#include "ntrigger/ntriggerserver.h"
#include "nworldinterface/nworldinterface.h"
#include "nspecialfx/nfxserver.h"

#include "ngeomipmap/ngeomipmapnode.h"
#include "ngeomipmap/nfloatmap.h"
#include "nmaterial/nmaterialserver.h"
#include "nphysics/nphysicsserver.h"
#include "nphysics/nphysicsworld.h"
#include "nspatial/nspatialserver.h"
#include "nspatial/ncspatial.h"
#include "kernel/ndependencyserver.h"
#include "entity/nentityclassserver.h"
#include "file/nwatcherdirserver.h"
#include "nsoundscheduler/nsoundscheduler.h"
#include "nmusictable/nmusictable.h"
#include "nscriptclassserver/nscriptclassserver.h"

#include "zombieentity/nloaderserver.h"
#include "rnsstates/naitester.h"
#include "nsavemanager/nsavemanager.h"
#include "kernel/nlogclass.h"
#include "kernel/logclassregistry.h"
#include "nphysics/ncphysicsobjclass.h"
#include "nphysics/ncphypickableobj.h"

#include "nnetworkenet/nnetutils.h"
#include "rnsgameplay/ncgameplay.h"

#include "gameplay/nmissionhandler.h"

nNebulaScriptClass(nConjurerApp, "ncommonapp");

//------------------------------------------------------------------------------
NCREATELOGLEVEL_REL(conjurer, "Conjurer", true, 1);

//------------------------------------------------------------------------------
static const char sessionFileName[] = "user:editorstate.n2";

//------------------------------------------------------------------------------
/**
*/
nConjurerApp::nConjurerApp() :
    isOverlayEnabled(true),
    tempModeEnabled(false),
    controlMode(FreeCam),
    screenshotID(0),
    animStateIndex(0),
    captureFrame(false),
    runPhysics(false),
    attachEnabled(true),
    renderEnabled(true),
    refScriptServer("/sys/servers/script"),
    renderWindowEmbedded(false),
    outguiDummyPath("/editor/outguiisopened"),
    quitConfirmationPending(false),
    saveManager(0),
    stateFile(0)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
nConjurerApp::~nConjurerApp()
{
    this->bookmarks.Clear();

    if (this->saveManager)
    {
        this->saveManager->Release();
        this->saveManager = 0;
    }

    if (this->stateFile)
    {
        this->stateFile->Release();
        this->stateFile = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
nConjurerApp::Open()
{
    // before assigning the working copy, some things could be inferred from
    // the -view parameter if it is a .n2 file (a level or a class)
    this->MangleSceneFileParameter();

    if (!this->InitWorkingCopyAssign())
    {
        n_message("No valid working copy path available!\n"
                  "(use the -wc argument, or CONJURER_WC environment var)\n"
                  "Conjurer will exit now.");
        return false;
    }
    this->kernelServer->GetFileServer()->SetAssign("outgui", "home:code/conjurer/conjurer/gui/");
        
    this->refPythonScriptServer = (nScriptServer*)  kernelServer->New("npythonserver" , "/sys/servers/persist/npythonserver");
    this->refPythonScriptServer->SetFailOnError(false);
    this->refUndoServer         = (nUndoServer*)    kernelServer->New("nundoserver", "/sys/servers/undo");
    this->refDebugServer        = (nDebugServer*)   kernelServer->New("ndebugserver", "/sys/servers/debug");
    this->refLayerManager       = (nLayerManager*)  kernelServer->New("nlayermanager", "/sys/servers/layermanager");
    this->refWatcherDirServer   = (nWatcherDirServer*) kernelServer->New("nwatcherdirserver", "/sys/servers/dirwatcher");
    this->refDebugComponentServer = (nDebugComponentServer*) kernelServer->New("nconjurerdebugcompserver", "/sys/servers/debugcomp");

    kernelServer->New("nkernelserverproxy", "/sys/servers/kernelserver");

    // OUTGUI
    if (this->GetGuiScriptFile())
    {
        this->RunGUIScript(this->GetGuiScriptFile());
    }
    else
    {
        this->SetRenderWindowEmbedded( false );
    }

    nCommonApp::Open();

    this->refAITester = (nAITester*) kernelServer->New("naitester", "/sys/servers/aitester");

    // Monitor server uses a reference to gfx server, so this is created after the gfx one
    this->refMonitorServer = (nMonitorServer*) kernelServer->New("nmonitorserver", "/sys/servers/monitorserver");

    // initialize By Script undo/redo
    UndoCmdByScript::SetScriptServer(this->refPythonScriptServer);

    if (this->GetWatch())
    {
        nConServer::Instance()->Watch(this->GetWatch());
    }

    nSceneServer::Instance()->SetBgColor(nGfxUtils::color24(140, 138, 156));

    // create scene graph root node
    this->refSceneNode = (nTransformNode *) kernelServer->New("ntransformnode", "/usr/scene");

    // create root node for materials 
    this->refMaterial = kernelServer->New("nroot", "/usr/material");

    nString result;
    this->refScriptServer->RunFunction("OnMaterialStartup", result);

    // initialize viewports (set as x,y,w,h)
    const nDisplayMode2& displayMode = nGfxServer2::Instance()->GetDisplayMode();

    // load music table
    if (nFileServer2::Instance()->FileExists(musicTablePath))
    {
        this->refMusicTable = static_cast<nMusicTable*>( nKernelServer::Instance()->LoadAs( musicTablePath, musicTableNOHPath, true) );
    }

    // assign entity paths for current level (or default if none)
    n_assert(this->GetLevelFile());
    nFileServer2* fileServer = kernelServer->GetFileServer();
    if (!fileServer->FileExists(this->GetLevelFile()))
    {
        nString levelPath;
        levelPath.Format("wc:levels/%s.n2", this->GetLevelFile());
        if (fileServer->FileExists(levelPath))
        {
            this->SetLevelFile(levelPath.Get());
        }
    }

    // initialize entity class directory
    nEntityClassServer *entityServer = nEntityClassServer::Instance();
    entityServer->SetEntityClassDir("wc:classes/");

    // init sound scheduler
    this->refSoundScheduler->Init();

    if (!this->refLevelManager->LoadLevel(this->GetLevelFile()))
    {
        n_message("Couldn't load level file: '%s'\n"
                  "Conjurer will exit now.", this->GetLevelFile());
        return false;
    }
    else
    {
        // set window title to show the current level
        nString title = this->CalcTitleString("Summoner");
        this->SetWindowTitle(title.Get());
    }

    kernelServer->PushCwd(this);

    // load viewport layout from file
    n_assert(this->GetViewportLayout());
    n_assert(kernelServer->GetFileServer()->FileExists(this->GetViewportLayout()));
    this->refViewportUI = static_cast<nAppViewportUI*>(kernelServer->LoadAs(this->GetViewportLayout(), "appviewportui"));;
    n_assert(this->refViewportUI.isvalid());
    n_assert(this->refViewportUI->IsA("nappviewportui"));

    // load viewport for preview
    this->refPreview = static_cast<nPreviewViewport*>(this->refViewportUI->Find("preview"));
    n_assert(this->refPreview.isvalid());
    this->refPreview->SetRelSize(0.0f, 0.0f, 0.5f, 0.5f);

    kernelServer->PopCwd();

    // load persisted viewport layout, then open
    // FIXME viewport layout is persisted using absolute screen coordinates (ma.garcias)
    this->refViewportUI->SetClientRect(0, 0, displayMode.GetWidth(), displayMode.GetHeight());
    this->refViewportUI->Open();

    // load viewport for renaissance states
    this->refRnsViewport = static_cast<nAppViewport*>(kernelServer->LoadAs("home:data/appdata/conjurer/viewport/rnsview.n2", "/usr/rnsview"));
    n_assert(this->refRnsViewport.isvalid());
    n_assert(this->refRnsViewport->IsA("nappviewport"));
    this->refRnsViewport->Open();

    // load map vieport for both editor and game states
    this->refMapViewport = static_cast<nAppViewport*>(kernelServer->LoadAs("home:data/appdata/conjurer/viewport/mapview.n2", "/usr/mapview"));
    n_assert(this->refMapViewport.isvalid());
    n_assert(this->refMapViewport->IsA("nappviewport"));
    this->refMapViewport->Open();

    // open preview viewport
    this->refPreview->Open();

    // create debug module for editor component visualizations
    n_verify( nDebugServer::Instance()->CreateDebugModule("neditordebugmodule", "editor") );
    n_verify( nDebugServer::Instance()->CreateDebugModule("nscenedebugmodule", "scene") );

    // create application states
    this->CreateState("neditorstate", "editor");
    this->CreateState("nassetloadstate", "loader");
    this->CreateState("nterraineditorstate", "terrain");
    this->CreateState("nobjecteditorstate", "object");
    this->CreateState("nclasseditorstate", "class");
    this->CreateState("nnavmeshbuilderstate", "navbuilder");
    this->CreateState("ngeometrystreameditorstate", "geometrystream");

    this->CreateState( "rnsmenustate", "menu" );
    this->CreateState( "rnsloadstate", "load" );
    this->CreateState( "rnsgamestate", "game" );
    this->CreateState( "rnsaistate", "ai" );
    this->CreateState( "rnswanderstate", "wander" );

    // if state need to be created in the begin, use FindState to force creation
    this->FindState("editor");
    this->FindState("loader");
    this->FindState("terrain");
    this->FindState("object");
    this->FindState("class");
    this->FindState("navbuilder");
    this->FindState("geometrystream");
    this->FindState("aitest");
    this->FindState("wander");
    this->FindState("game");
    this->FindState("load");
    this->FindState("menu");

    this->refLoaderState = (nAssetLoadState*) this->FindState("loader");
    this->refLoaderState->SetLoadClassName(this->GetLoadClassName());
    //<OBSOLETE> keep for backwards compatibility
    this->refLoaderState->SetSceneFile(this->GetSceneFile());
    //<OBSOLETE>

    // check that initial state was created
    n_assert2( this->FindState( this->initState ), "No exist initial State in conjurer" );

    this->SetState(this->initState);

    // load entity from file
    // @deprecated keep "this->GetSceneFile()" for backwards compatibility
    if (this->GetSceneFile() || this->GetLoadClassName())
    {
        this->refLoaderState->SetNextState(this->initState);
        this->refLoaderState->SetAnimState(this->GetAnimState());
        this->SetState("loader");
    }
    else
    {
        this->SetState(this->initState);
    }

    // reset camera position for all viewports
    this->refViewportUI->Reset();

    // perform additional custom initialization -keep always at the end
    this->refScriptServer->RunFunction("OnConjurerStartup", result);

    /// @todo ma.garcias- this only works after setting up everything else
    this->refScriptClassServer->AddClasses();

    // create global variable editor
    this->refGlobalVarEditor = (nGlobalVariableEditor*) kernelServer->New("nglobalvariableeditor", "/usr/globalvars");
    this->refGlobalVarEditor->LoadGlobalVariables();

    // OUTGUI
    if (this->GetGuiScriptFile())
    {
        nString result;
        // restores the outgui windows state
        this->refPythonScriptServer->RunFunction("Restore", result);
    }

    /// add unique strings for network
    //this->AddNetworkUniqueStrings();

    return true;
}

//------------------------------------------------------------------------------
/**
*/
void
nConjurerApp::Close()
{
    // Close OutGUI first if it's opened
    this->CloseOutGUI(true);

    // leave current state
    nAppState* currentState = this->FindState( this->GetCurrentState() );
    if ( currentState )
    {
        currentState->OnStateLeave("");
    }

    if (this->refAITester.isvalid())
    {
        this->refAITester->Release();
    }

    if (this->refGlobalVarEditor.isvalid())
    {
        this->refGlobalVarEditor->Release();
    }

    if (this->refViewportUI.isvalid())
    {    
        this->refViewportUI->Close();
        this->refViewportUI->Release();
    }
    if (this->refRnsViewport.isvalid())
    {
        this->refRnsViewport->Release();
    }
    if (this->refMapViewport.isvalid())
    {    
        this->refMapViewport->Release();
    }

    // release undo server before level to release entity references!
    if (this->refUndoServer.isvalid())
    {
        this->refUndoServer->Release();
    }

    if (this->refMaterial.isvalid())
    {
        this->refMaterial->Release();
    }
    if (this->refSceneNode.isvalid())
    {
        this->refSceneNode->Release();
    }

    if (this->refPythonScriptServer.isvalid())
    {
        this->refPythonScriptServer->Release();
    }
    if (this->refDebugServer.isvalid())
    {
        this->refDebugServer->Release();
    }
    if (this->refMonitorServer.isvalid())
    {
        this->refMonitorServer->Release();
    }
    if (this->refWatcherDirServer.isvalid())
    {
        this->refWatcherDirServer->Release();
    }

    if (nKernelServerProxy::Instance())
    {
        nKernelServerProxy::Instance()->Release();
    }

    nCommonApp::Close();

    // destroy after level manager!
    if (this->refLayerManager.isvalid())
    {
        this->refLayerManager->Release();
    }

    // release debug component server after entity server, ncEditor components use it
    if (this->refDebugComponentServer.isvalid())
    {
        this->refDebugComponentServer->Release();
    }
}

//------------------------------------------------------------------------------
/**
    Create graphics server that asks the user for confirmation when closing.
*/
nGfxServer2*
nConjurerApp::CreateGfxServer()
{
    return static_cast<nGfxServer2*>( kernelServer->New("nconjurergfxserver", "/sys/servers/gfx") );
}

//------------------------------------------------------------------------------
/**
    Create application-specific scene server that provides debug rendering.
    TODO: This is a temporary solution while we implement a proper debug server.
*/
nSceneServer*
nConjurerApp::CreateSceneServer()
{
    return (nSceneServer*) kernelServer->New("nconjurersceneserver", "/sys/servers/scene");
}

//------------------------------------------------------------------------------
/**
    Do one complete frame.
*/
void
nConjurerApp::DoFrame()
{
    // trigger script server
    if (!this->refPythonScriptServer->Trigger())
    {
        this->SetQuitRequested(true);
    }
    
    // Ask user for saving changes when the window close icon has been clicked
    if ( this->quitConfirmationPending )
    {
        this->SetQuitRequested( this->CloseOutGUI() );
        this->quitConfirmationPending = false;
    }

    // Reload resource if have changed
    this->refWatcherDirServer->Trigger();

    // Update events and make triggers handle them
//    static nGameEvent::Time currentTime(0);
//    this->refTriggerServer->Update( currentTime++ );
    this->refAITester->Trigger();

    // perform state logic
    nCommonApp::DoFrame();

    // trigger log
    LogClassRegistry::Instance()->Trigger();

    if (this->captureFrame)
    {
        this->CaptureScreenshot();
    }
}

//------------------------------------------------------------------------------
/**
    Do one complete frame.
*/
void
nConjurerApp::OnRender2D()
{
    if (!this->captureFrame)
    {
        nCommonApp::OnRender2D();
    }

    // Trigger graphic monitor server
    this->refMonitorServer->Trigger();

    // Draw graphic monitor server
    this->refMonitorServer->Render();
}

//------------------------------------------------------------------------------
/**
*/
void
nConjurerApp::SetCurrentViewport(const char* name)
{
    nAppViewport* viewport = this->refViewportUI->FindViewport(name);
    if (viewport)
    {
        this->refViewportUI->SetCurrentViewport(viewport);
    }
}

//------------------------------------------------------------------------------
/**
*/
const char*
nConjurerApp::GetCurrentViewport()
{
    return this->refViewportUI->GetCurrentViewport()->GetName();
}

//------------------------------------------------------------------------------
/**
    Adds a viewer bookmark with the current viewport
*/
void
nConjurerApp::AddBookmark(const vector3& position, const polar2& angles)
{
    // if there is a bookmark with the exact same value, clear it
    nArray<nViewerParams>::iterator iter = this->bookmarks.Find(nViewerParams(position, angles));
    if (iter)
    {
        this->bookmarks.Erase(iter);
    }
    else
    {
        this->bookmarks.Append(nViewerParams(position, angles));
    }
}

//------------------------------------------------------------------------------
/**
    Sets a saved bookmark
*/
void
nConjurerApp::SetBookmark(int index)
{
    if (index < 0 || index > this->bookmarks.Size())
    {
        return;
    }

    nAppViewport *vp = this->refViewportUI->GetCurrentViewport();
    vp->SetViewerPos(this->bookmarks[index].position);
    vp->SetViewerAngles(this->bookmarks[index].angles);
}

//------------------------------------------------------------------------------
/**
    Sets a saved bookmark
*/
int
nConjurerApp::GetNumBookmarks()
{
    return this->bookmarks.Size();
}

//------------------------------------------------------------------------------
/**
    Sets a saved bookmark
*/
void
nConjurerApp::SaveBookmarks(const char *filename)
{
    // save bookmarks for the level, if any
    if (!this->bookmarks.Empty())
    {
        nPersistServer *ps = nKernelServer::Instance()->GetPersistServer();
        n_assert(ps);

        nCmd* cmd = ps->GetCmd(this, 'THIS');
        if (ps->BeginObjectWithCmd(this, cmd, filename))
        {
            for (int i = 0; i < this->bookmarks.Size(); ++i)
            {
                cmd = ps->GetCmd(this, 'JABM');
                cmd->In()->SetF(this->bookmarks[i].position.x);
                cmd->In()->SetF(this->bookmarks[i].position.y);
                cmd->In()->SetF(this->bookmarks[i].position.z);
                cmd->In()->SetF(this->bookmarks[i].angles.theta);
                cmd->In()->SetF(this->bookmarks[i].angles.rho);
                ps->PutCmd(cmd);
            }

            ps->EndObject(true);
        }
    }
}

//------------------------------------------------------------------------------
/**
    Sets a saved bookmark
*/
void
nConjurerApp::LoadBookmarks(const char *filename)
{
    // load bookmarks for the level
    this->bookmarks.Clear();
    if (kernelServer->GetFileServer()->FileExists(filename))
    {
        kernelServer->PushCwd(this);
        kernelServer->Load(filename, false);
        kernelServer->PopCwd();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
nConjurerApp::CaptureScreenshot()
{
    nString filename;
    const char* sceneFile = this->refLoaderState->GetSceneFile();
    if (sceneFile)
    {
        filename = sceneFile;
        filename.StripExtension();
    }
    else if (this->GetLoadClassName())
    {
        filename = this->GetLoadClassName();
    }
    else
    {
        filename = "screenshot";
    }

    nString screenshotFile;
    do
    {
        screenshotFile = filename;
        screenshotFile.AppendInt(this->screenshotID++); 
        screenshotFile.Append(".png");
    }
    while (kernelServer->GetFileServer()->FileExists(screenshotFile.Get()));

    if (nGfxServer2::Instance()->SaveScreenshot(screenshotFile.Get()))
    {
        n_message("Screenshot saved as file '%s'", screenshotFile.Get());
    }

    this->captureFrame = false;
}

//------------------------------------------------------------------------------
/**
    Runs a script with the specific script server for the gui.
*/
bool 
nConjurerApp::RunGUIScript( const char* scriptfilename )
{
    nString textResult;

    if( !this->refPythonScriptServer->RunScript( scriptfilename, textResult ) )
    {
        n_error( "Failed running: %s", textResult );
        return false;
    }

    nString PythonFunction( "StartOutterGUI(False)" );

    if( this->IsRenderWindowEmbedded() )
    {
        PythonFunction = "StartOutterGUI(True)";
    }

    if( !this->refPythonScriptServer->Run( PythonFunction.Get(), textResult ) )
    {
        n_error( "Failed running: %s", textResult );
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
/**
    Try a guess on the view parameter.
    If it inside a "classes/" or "levels/" directory, the file *could* be
    an entity class or a level file. If the parent directory *looks* like
    a working copy, assign it as such, and the file as the class or level
    file to load.
*/
void
nConjurerApp::MangleSceneFileParameter()
{
    if (!this->GetSceneFile())
    {
        return;
    }

    // is it a .n2 valid file?
    nFileServer2 *fileServer = kernelServer->GetFileServer();
    if (fileServer->FileExists(this->GetSceneFile()))
    {
        // is it a level or class in a valid working copy?
        nString pathToMangle(this->GetSceneFile());
        pathToMangle.ConvertBackslashes();
        pathToMangle = pathToMangle.ExtractDirName();
        pathToMangle.StripTrailingSlash();
        nString lastDirName(pathToMangle.ExtractFileName());

        if (lastDirName == nString("levels") || lastDirName == nString("classes"))
        {
            // is the parent path a valid working copy?
            pathToMangle = pathToMangle.ExtractDirName();

            if (fileServer->DirectoryExists(nString(pathToMangle + "classes").Get()) &&
                fileServer->DirectoryExists(nString(pathToMangle + "levels").Get()))
            {
                // set working copy path
                pathToMangle.StripTrailingSlash();
                this->SetTempWorkingCopyDir(pathToMangle.Get());
                this->SetTemporaryModeEnabled(true);
                if (lastDirName == nString("levels"))
                {
                    // set -view argument as level file path
                    this->SetLevelFile(this->GetSceneFile());
                    this->SetSceneFile("");
                }
                else if (lastDirName == nString("classes"))
                {
                    // set -view argument as class file path
                    nString className(this->GetSceneFile());
                    className = className.ExtractFileName();
                    className.StripExtension();
                    className.ToCapital();
                    this->SetLoadClassName(className.Get());
                    this->SetSceneFile("");
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
    Set the "wc" assign for use from conjurer
*/
bool
nConjurerApp::InitWorkingCopyAssign()
{
    nFileServer2 *fileServer = kernelServer->GetFileServer();
    nArray<nString> wcPaths;

    // if temporary working copy assigned, set first for write
    if (this->GetTempWorkingCopyDir() && this->GetTemporaryModeEnabled())
    {
        wcPaths.Append(nString(this->GetTempWorkingCopyDir()));
    }

    // set primary working copy
    if (this->GetWorkingCopyDir())
    {
        nString wcString(this->GetWorkingCopyDir());
        wcString.ConvertBackslashes();
        wcPaths.Append(wcString);
    }
#ifdef __WIN32__
    else
    {
        char buf[N_MAXPATH];
        char* s = getenv("CONJURER_WC");
        if (s)
        {
            n_strncpy2(buf,s,sizeof(buf));
            nString workingCopyPath(buf);
            workingCopyPath.ConvertBackslashes();
            wcPaths.Append(workingCopyPath);
        }
    }
#endif

    // HACK default primary working copy for developers
    if (wcPaths.Size() == 0 && fileServer->DirectoryExists("home:../conjurer_wc"))
    {
        wcPaths.Append(nString("home:../conjurer_wc"));
    }

    if (wcPaths.Size() > 0)
    {
        fileServer->SetAssignGroup("wc", wcPaths);
#ifndef NGAME
        // shaders and textures will be monitorized
        nArray<nString> moniPaths;
        moniPaths.Append(nString("shaders:"));
        moniPaths.Append(nString("wctextures:"));
        fileServer->SetAssignGroup("dirwatches", moniPaths);
#endif
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
/**
    Set the window title
*/
void
nConjurerApp::SetWindowTitle( const char* title )
{
    nGfxServer2::Instance()->SetWindowTitle( title );
}

//------------------------------------------------------------------------------
/**
    Calculate the window title
*/
nString
nConjurerApp::CalcTitleString( const char* prefix )
{
    nString level( this->GetLevelFile() );
    level = level.ExtractFileName();
    level.StripExtension();

    nString title;
    title.Append(prefix);
    if (this->GetInstanceName())
    {
        title.Append( " " );
        title.Append( this->GetInstanceName() );
    }
    title.Append(" - Level ");
    title.Append( level.Get() );
    title.Append( " - " N_RELEASE_SVN_REVISION_STR );
    return title;
}

//------------------------------------------------------------------------------
/**
    Try to close outgui, returning false if user vetoes it

    If forceToClose is set to true, the user isn't asked and the outgui will be
    closed without saving any data.
*/
bool
nConjurerApp::CloseOutGUI( bool forceToClose ) const
{
    // Try to close OutGUI if it's opened
    if ( nKernelServer::Instance()->Lookup( this->outguiDummyPath ) )
    {
        nString result;
        nString cmd( "EndOutterGUI(" );
        cmd += forceToClose? "True" : "False";
        cmd += ")";
        this->refPythonScriptServer->Run( cmd.Get(), result );
        this->refPythonScriptServer->Trigger();
    }

    // OutGUI is known to be closed when its dummy object doesn't exist
    return !nKernelServer::Instance()->Lookup( this->outguiDummyPath );
}

//------------------------------------------------------------------------------
/**
    Set the quit confirmation flag
*/
void
nConjurerApp::SetQuitConfirmationPending( bool b )
{
    this->quitConfirmationPending = b;
}

//------------------------------------------------------------------------------
/**
    Create new level
*/
void
nConjurerApp::NewLevel(const char *fileName)
{
    this->refLevelManager->NewLevel(fileName);

    // create default global entities in current level
    // @todo reimplement using signals?
    nString result;
    this->refScriptServer->RunFunction("OnConjurerNewLevel", result);
}

//------------------------------------------------------------------------------
/**
    Load level
*/
void
nConjurerApp::LoadLevel(const char *fileName)
{
    this->refLevelManager->LoadLevel(fileName);

    // allow customizing level loading
    // @todo reimplement using signals?
    nString result;
    this->refScriptServer->RunFunction("OnConjurerLoadLevel", result);
}

//------------------------------------------------------------------------------
/**
    Load level
*/
void
nConjurerApp::DeleteLevel(const char *fileName)
{
    this->refLevelManager->DeleteLevel(fileName);
}

//------------------------------------------------------------------------------
/**
    save current level
*/
void
nConjurerApp::SaveLevel()
{
    this->refLevelManager->SaveLevel();
}

//------------------------------------------------------------------------------
/**
    save current level under a different name
*/
bool
nConjurerApp::SaveCurrentLevelAs(const char* newLevelName)
{
    n_assert_return2(newLevelName, false, "no name supplied");

    return this->refLevelManager->SaveCurrentLevelAs(newLevelName);
}

//------------------------------------------------------------------------------
/**
    set the current instance name
*/
void 
nConjurerApp::SetInstanceName(const char * name)
{
    if (name)
    {
        this->instanceName = name;
    }
    else
    {
        this->instanceName.Clear();
    }
}

//------------------------------------------------------------------------------
/**
    get instance name
*/
const char * 
nConjurerApp::GetInstanceName() const
{
    return this->instanceName.Get();
}

//------------------------------------------------------------------------------
/**
    save the editor state before entering game
*/
void 
nConjurerApp::SaveEditorState()
{
    //this->stateFile = nFileServer2::Instance()->NewFileObject();
    //this->stateFile = n_new(nMemFile);
    //n_assert(this->stateFile);

    this->saveManager = static_cast<nSaveManager *> (nKernelServer::Instance()->New("nsavemanager"));
    n_assert(this->saveManager);

    this->saveManager->BeginSession();
    for ( nEntityObject* entity = nEntityObjectServer::Instance()->GetFirstEntityObject(); entity; entity = nEntityObjectServer::Instance()->GetNextEntityObject() )
    {
        if (this->ShouldSaveObjectState(entity))
        {
            this->saveManager->AddObjectState(entity);
        }
    }

    // save Mission Objectives
    nMissionHandler * missionHandler = nMissionHandler::Instance();
    if( missionHandler )
    {
        this->saveManager->AddObjectState( missionHandler );
    }

    this->saveManager->EndSession();

    if ( stateFile )
    {
        this->saveManager->SaveSession(stateFile, nPersistServer::SAVETYPE_RELOAD);
        
    } else
    {
        this->saveManager->SaveSession(sessionFileName, nPersistServer::SAVETYPE_RELOAD);
    }
}

//------------------------------------------------------------------------------
/**
    determine if an object state must be saved or not
*/
bool
nConjurerApp::ShouldSaveObjectState(nEntityObject * entity) const
{
    static nClass * clneoutdoor = nKernelServer::Instance()->FindClass("neoutdoor");
    n_assert(clneoutdoor);
    static nClass * clneindoor = nKernelServer::Instance()->FindClass("neindoor");
    n_assert(clneindoor);
    static nClass * clneindoorbrush = nKernelServer::Instance()->FindClass("neindoorbrush");
    n_assert(clneindoorbrush);
    static nClass * clneportal = nKernelServer::Instance()->FindClass("neportal");
    n_assert(clneportal);
    static nClass * clnewaypoint = nKernelServer::Instance()->FindClass("newaypoint");
    n_assert(clnewaypoint);
    static nClass * clnewaypointpath = nKernelServer::Instance()->FindClass("newaypointpath");
    n_assert(clnewaypointpath);
    static nClass * clnenavmeshnode = nKernelServer::Instance()->FindClass("nenavmeshnode");
    n_assert(clnenavmeshnode);
    static nClass * clnnavmesh = nKernelServer::Instance()->FindClass("nnavmesh");
    n_assert(clnnavmesh);
    static nClass * clneviewport = nKernelServer::Instance()->FindClass("neviewport");
    n_assert(clneviewport);
    static nClass * clnecamera = nKernelServer::Instance()->FindClass("necamera");
    n_assert(clnecamera);
    static nClass * clnehumragdoll = nKernelServer::Instance()->FindClass("nehumragdoll");
    n_assert(clnehumragdoll);
    static nClass * clnescene = nKernelServer::Instance()->FindClass("nescene");
    n_assert(clnescene);
    static nClass * clnespecialfx = nKernelServer::Instance()->FindClass("nespecialfx");
    n_assert(clnespecialfx);

    // not save outdoor, indoor, waypoints and navigation mesh
    if (entity->IsA(clneoutdoor) ||
        entity->IsA(clneindoor) ||
        entity->IsA(clnewaypoint) ||
        entity->IsA(clnewaypointpath) ||
        entity->IsA(clneportal) ||
        entity->IsA(clneindoorbrush) ||
        entity->IsA(clnenavmeshnode) ||
        entity->IsA(clnnavmesh) ||
        entity->IsA(clneviewport) ||
        entity->IsA(clnecamera) ||
        entity->IsA(clnehumragdoll) ||
        entity->IsA(clnescene) ||
        entity->IsA(clnespecialfx) )
    {
        return false;
    }

    // don't save the indoor facades (identified by having a valid ptr in GetIndoorSpace())
    ncSpatial * spatialComp = entity->GetComponent<ncSpatial>();
    if (spatialComp && spatialComp->GetIndoorSpace())
    {
        return false;
    }

    // please save anything that has gameplay compo
    if (entity->GetComponent<ncGameplay>())
    {
        return true;
    }

    // not save physic objects without mass
    if (entity->GetComponent<ncPhysicsObj>() && 
        !entity->GetComponent<ncPhyPickableObj>() )
    {
        ncPhysicsObj * phyObj = entity->GetComponent<ncPhysicsObj>();
        ncPhysicsObjClass * phyObjClass = entity->GetClassComponent<ncPhysicsObjClass>();
        if (phyObj->GetMass() > 0.0f || phyObjClass->GetMobile())
        {
            return true;
        }
        
        return false;
    }

    // save anything else
    return true;
}

//------------------------------------------------------------------------------
/**
    restore the editor state after entering game
*/
void 
nConjurerApp::RestoreEditorState()
{
    for ( nEntityObject* entity = nEntityObjectServer::Instance()->GetFirstEntityObject(); entity; entity = nEntityObjectServer::Instance()->GetNextEntityObject() )
    {
        if (this->ShouldSaveObjectState(entity))
        {
            this->saveManager->AddObjectAvailableBeforeRestore(entity);
        }
    }

    nMissionHandler * missionHandler = nMissionHandler::Instance();
    if( missionHandler )
    {
        this->saveManager->AddObjectAvailableBeforeRestore(missionHandler);
    }

    if ( stateFile )
    {
        this->saveManager->RestoreSession(stateFile);
        
    } else
    {
        this->saveManager->RestoreSession(sessionFileName);
    }

    for ( nEntityObject* entity = nEntityObjectServer::Instance()->GetFirstEntityObject(); entity; entity = nEntityObjectServer::Instance()->GetNextEntityObject() )
    {
        if (this->ShouldSaveObjectState(entity))
        {
            this->saveManager->AddObjectAvailableAfterRestore(entity);
        }
    }

    this->saveManager->CheckRestoredObjects();

    this->saveManager->ResetSession();
    this->saveManager->Release();
    this->saveManager = 0;

    if (this->stateFile)
    {
        this->stateFile->Release();
        this->stateFile = 0;
    }

    // Check selection and remove lost objects
    nObjectEditorState* objState = static_cast<nObjectEditorState*>( this->FindState("object") );
    n_assert( objState );
    objState->CheckSelection();
}

//------------------------------------------------------------------------------
/**
*/
void
nConjurerApp::AddNetworkUniqueStrings()
{
    nKernelServer * ks = nKernelServer::ks;

    const nHashList * classList = ks->GetClassList();

    // classes that will be inserted like unique strings
    const char * classesTable[] = { 
        "ngpbasicaction", "neweaponaddon", "neweapon",
        "neweaponaddon", "neequipment", "neammunition",
        0
    };

    nHashNode * node = classList->GetHead();
    while( node )
    {
        nClass * clazz = ks->FindClass( node->GetName() );

        if( clazz )
        {
            for( int i = 0 ; classesTable[ i ] ; ++i )
            {
                if( clazz->IsA( classesTable[ i ] ) )
                {
                    nNetUtils::InsertUniqueString( node->GetName() );
                    break;
                }
            }
        }

        node = node->GetSucc();
    }
}
