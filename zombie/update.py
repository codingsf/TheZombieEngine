#--------------------------------------------------------------------------
# 3rd Generation Nebula 2 Build System
#
# (c) 2005 Vadim Macagon
#
# Contents are licensed under the Nebula license.
#--------------------------------------------------------------------------

import sys
import zlib
import os
import _winreg

if not hasattr(sys, 'hexversion') or sys.hexversion < 0x020400f0:
    sys.exit('Python 2.4 or later is required to run the build system.')

from buildsys3.bldscanner import *
from buildsys3.buildsys import *
import os, dircache, string, time, shutil, glob
try:
    import hotshot, hotshot.stats
except:
    hotshot = None

updateDir = os.getcwd()

#--------------------------------------------------------------------------
def PrintHelp():
    print """\
Usage:
update.py [-help] [-build generator] [workspaces]
          [-listworkspaces] [-listgenerators] [-verbose]
    
Examples:
(1) To generate Visual Studio 2003 .vcproj/.sln files for
    all available workspaces do...
    
    update.py -build vstudio71
    
(2) To generate Visual Studio 2003 files for just the Nebula2
    workspace do...
    
    update.py -build vstudio71 Nebula2
    
(3) To generate Visual Studio 2003 files for the Nebula2 and
    luaserver workspaces do...
    
    update.py -build vstudio71 Nebula2 luaserver
	
(4) To generate a xmlschema doc
	update.py -build xmlschema nkernel nroot -nogui workspace
	update.py -build xmlschema nkernel all -nogui workspace
	update.py -build xmlschema all all -nogui workspace
	
Details:
-help
    You're reading it.
-build
    Specify which build system generator should be used.
    If this isn't specified, then all available generators
    will be run (not recommended).
workspaces
    The list of workspaces that should be generated.
    By default, all workspaces will be generated.
-listworkspaces
    List the available workspaces.
-listgenerators
    List the available build system generators.
-nogui
    Stops the GUI from showing up, use this if you don't have wxPython
    installed.
-nocopydlls
	Not copy dll in bin directories
-nogetsvnrev
	Not get the svn revision and modified appinfo.h
"""

#--------------------------------------------------------------------------
# Return the filename of the build config that should be used by the build
# system. build.cfg.py should not be modified by the end user, any
# user customization should go into user.build.cfg.py
def GetStandardBuildCfgFileName():
    if os.path.isfile(os.path.join(updateDir, 'user.build.cfg.py')):
        return 'user.build.cfg.py'
    else:
        return 'build.cfg.py'

#--------------------------------------------------------------------------
def PrintWorkspaceList():
    buildSys = BuildSys(updateDir, GetStandardBuildCfgFileName())
    if buildSys.Prepare():
        numWorkspaces = len(buildSys.workspaces)
        if numWorkspaces > 0:
            print '************************\n' \
                  '* Available Workspaces *\n' \
                  '************************'
            for workspaceName in buildSys.workspaces.keys():
                print workspaceName
            print '************************'
        else:
            print '************************\n' \
                  '* No Workspaces Found  *\n' \
                  '************************'
    else:
        # print out errors
        for err in buildSys.errors:
            print err

#--------------------------------------------------------------------------
def PrintGeneratorList():
    buildSys = BuildSys(updateDir, GetStandardBuildCfgFileName())
    generators = buildSys.GetGenerators()
    if len(generators) > 0:
        print '************************\n' \
              '* Available Generators *\n' \
              '************************'
        for generatorName, generator in generators.items():
            print generatorName + '\n    ' + generator.description
        print '************************'
    else:
        print 'No Generators Found'

#--------------------------------------------------------------------------
def Build(generatorName, workspaces, extraArgs={}):
    buildSys = BuildSys(updateDir, GetStandardBuildCfgFileName())
    if buildSys.HasGenerator(generatorName):
        if buildSys.Prepare():
            buildSys.logger.info('Validation Successful!')
            buildSys.Run(generatorName, workspaces, extraArgs)
    else:
        buildSys.logger.error('Unknown generator ' + generatorName)

#--------------------------------------------------------------------------
# To profile a build use the -build flag with desired parameters and also
# add -profile to the command line.
# e.g. python update.py -profile -build vstudio71 Nebula2
def Profile(generatorName, workspaces):
    if hotshot is None:
        return
    prof = hotshot.Profile('build.prof')
    prof.runcall(Build, generatorName, workspaces)
    prof.close()
    stats = hotshot.stats.load('build.prof')
    stats.strip_dirs()
    #stats.sort_stats('cumulative')
    stats.sort_stats('time')
    stats.print_stats()

#--------------------------------------------------------------------------
def LaunchGUI(generatorName, workspaceNames):
    try:
        from buildsys3.gui.main import DisplayGUI
    except ImportError, err:
        print 'Error: Failed to import buildsys3.gui'
        print str(err)
    else:        
        buildSys = BuildSys(updateDir, GetStandardBuildCfgFileName())
        DisplayGUI(buildSys, generatorName, workspaceNames)

#--------------------------------------------------------------------------
def WxPythonAvailable():
    try:
        import wx
    except ImportError:
        return False
    else:
        if wx.VERSION < (2, 5):
            return False
        return True

#--------------------------------------------------------------------------
def main():
    generatorName = ''
    doProfile = False
    workspaceList = []
    useGUI = True
    numCmdLineArgs = len(sys.argv)
    copydlls = True
    getsvnrev = False
    #copywidgets = False
    extraArgs = {}
    if numCmdLineArgs > 1:
        i = 1
        while i < numCmdLineArgs:
            if '-help' == sys.argv[i]:
                PrintHelp()
                return
            elif '-nogui' == sys.argv[i]:
                useGUI = False
            elif '-build' == sys.argv[i]:
                if (i + 1) < numCmdLineArgs:
                    i += 1
                    generatorName = sys.argv[i]
                else:
                    print 'Error: No generator name given.'
                    return
                if generatorName == "xmlschema":
                    if (i + 1) < numCmdLineArgs:
                        i += 1
                        extraArgs['target'] = sys.argv[i]
                    else:
                        print 'Error: No target name given for xmlparser'
                        return
                    if (i + 1) < numCmdLineArgs:
                        i += 1
                        extraArgs['module'] = sys.argv[i]
                    else:
                        print 'Error: No module name given for xmlparser'
                        return
            elif '-profile' == sys.argv[i]:
                doProfile = True
            elif '-listworkspaces' == sys.argv[i]:
                PrintWorkspaceList()
                return
            elif '-listgenerators' == sys.argv[i]:
                PrintGeneratorList()
                return
            elif '-nocopydlls' == sys.argv[i]:
                copydlls = False
            elif '-nogetsvnrev' == sys.argv[i]:
                getsvnrev = False
            elif '-getsvnrev' == sys.argv[i]:
                getsvnrev = True
            #elif '-copywidgets' == sys.argv[i]:
            #    copywidgets = True
            else:
                workspaceList.append(sys.argv[i])
            i += 1    

    if useGUI:
        if WxPythonAvailable():
            LaunchGUI(generatorName, workspaceList)
        else:
            print 'wxPython 2.5.x or later is required (2.5.3.1 recommended)' \
                  ' to run the build system GUI.'
            print 'Falling back to command line only interface.'
            useGUI = False
    
    if not useGUI:
        if numCmdLineArgs > 1:
            if doProfile:
                Profile(generatorName, workspaceList)
            else:
                Build(generatorName, workspaceList, extraArgs)
        else:
            # build everything for everything? naaah :)
            print 'Clueless? Use update.py -help'

    if (copydlls):
        print "\nCopying dlls to bin directories..."
        for name in glob.glob("bin/dll/*.dll"):
            shutil.copy2(name, "bin/win32")
            shutil.copy2(name, "bin/win32a")
            shutil.copy2(name, "bin/win32d")
            shutil.copy2(name, "bin/win32do")
            shutil.copy2(name, "bin/win32gold")
        print "done."

    if (getsvnrev):
        print "\nGetting SVN revision..."		
        try:
            cmdstr = updateDir + "\\bin\\subwcrev.exe " + updateDir
            cmdstr = cmdstr + " " + updateDir + "\\code\\zombie\\appinfo\\inc\\appinfo\\appinfo.tpl"
            cmdstr = cmdstr + " " + updateDir + "\\code\\zombie\\appinfo\\inc\\appinfo\\appinfo.h"
            exitCode = os.system(cmdstr)
            if exitCode < 0:
                print >>sys.stderr, "Child was terminated by signal", -exitCode
            elif exitCode != 0:
                print >>sys.stderr, "Child returned", exitCode
        except OSError, e:
            print >>sys.stderr, "Execution failed:", e		

    #if (copywidgets):
    #    pReg = _winreg.ConnectRegistry(None, _winreg.HKEY_LOCAL_MACHINE)
    #    pKey = _winreg.OpenKey(pReg, r"SOFTWARE\Python\PythonCore\2.4\InstallPath")
    #    pythonpath = _winreg.EnumValue(pKey, 0)[1]
    #    widgetspath = str(pythonpath) + "Lib/site-packages/wx-2.6-msw-unicode/"        
    #    if os.path.exists(widgetspath):
    #        os.path.removedirs(widgetspath + "/wx")
    #        os.path.removedirs(widgetspath + "/wxPython")
    #        print "Coping wxWidgets to directory " + widgetspath + ":"
    #        for name in glob.glob("../wxWidgets/"):
    #            print name
    #            shutil.copytree(name, widgetspath)
    #        print path


# Entry Point    
if __name__ == '__main__':
    main()

#--------------------------------------------------------------------------
# EOF
#--------------------------------------------------------------------------
