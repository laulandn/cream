/* This snippet shows you how send a Finder OpenSelection event. */
/* A Finder OpenSelection allows you to have the Finder launch an Application, or */
/* open a document, which will of course cause the owning Application to launch and */
/* open that document.  Kinda just like the user had double-clicked on the file */
/* from a Finder window. */
/* This is an alternate to using the LaunchWithDoc code (somewhere else on this CD ) */
/* which shows you how to launch an application with a document.  Look for that sample and */
/* contrast the two methods, and pick the best for your use. */
/* C.K. Haun */
/* Apple Developer Tech Support */

/* include the ususal suspects, all the .h's you need  */
#include <Types.h>
#include <memory.h>
#include <Packages.h>
#include <Errors.h>
#include <quickdraw.h>
#include <fonts.h>
#include <Dialogs.h>
#include <windows.h>
#include <menus.h>
#include <events.h>
#include <Events.h>
#include <Desk.h>
#include <diskinit.h>
#include <OSUtils.h>
#include <resources.h>
#include <toolutils.h>
#include <AppleEvents.h>
#include <EPPC.h>
#include <Gestalt.h>
#include <PPCToolbox.h> 
#include <Processes.h>
#include <Aliases.h>
#include <Files.h>

#define aeSelectionKeyword		 'fsel'
#define aeOpenSelection 'sope'
#define kFinderSig 'FNDR'
#define kSystemType 'MACS'
/* prototype for process finding routine */
OSErr FindAProcess(OSType typeToFind, OSType creatorToFind, ProcessSerialNumberPtr processSN);

/* OpenSelection takes a FSSpec pointer, and creates a Finder Open Selection */
/* AppleEvent for the document described by the FSSpec.  This can be an */
/* application or document. */
OSErr OpenSelection(FSSpecPtr theDocToOpen)
{
/* temp variables I'll be using */
AppleEvent aeEvent;                                         /* the event to create */
AEDesc myAddressDesc, aeDirDesc, listElem;                  /* some descriptors I'll need */
FSSpec dirSpec;                                             /* FSSpec for the 'parent' directory of the file I'm opening */
AEDesc fileList;                                            /* my list */
OSErr myErr;                                                /* guess */
ProcessSerialNumber process;                                /* This will hold the process serial number of the Finder */
AliasHandle DirAlias, FileAlias;                            /* some aliases */

/* go find the Finder's process information, please */
if (FindAProcess(kFinderSig, kSystemType, &process))
	return(procNotFound);     /* if I can't find the Finder, quit.  Always check this, someone else */
/* could have shut down the finder */
/* Create an address descriptor so the AppleEvent manager knows where to send this event */
if (myErr = AECreateDesc(typeProcessSerialNumber, (Ptr)&process, sizeof(process), &myAddressDesc))
return(myErr);

/* Create the empty FinderEvent */
/* it's a Finder 'FNDR', Open Selection 'sope' event */
if (myErr = AECreateAppleEvent(kFinderSig, aeOpenSelection, &myAddressDesc, kAutoGenerateReturnID, kAnyTransactionID, &aeEvent))
return(myErr);

/* make a FSSpec for the parent folder (see the OpenSeletion description in the AE Registry ) */
/* using the information in the document FSSpec */
FSMakeFSSpec(theDocToOpen->vRefNum, theDocToOpen->parID, nil, &dirSpec);
NewAlias(nil, &dirSpec, &DirAlias);

/* Create alias for file */
NewAlias(nil, theDocToOpen, &FileAlias);
/* Create the file  list */
if (myErr = AECreateList(nil, 0, false, &fileList))
return(myErr);

/*  create the folder  descriptor */
HLock((Handle)DirAlias);
AECreateDesc(typeAlias, (Ptr)*DirAlias, GetHandleSize((Handle)DirAlias), &aeDirDesc);
HUnlock((Handle)DirAlias);
DisposHandle((Handle)DirAlias);
/* put the Directory Desc in the event as the direct object */
if ((myErr = AEPutParamDesc(&aeEvent, keyDirectObject, &aeDirDesc)) == noErr)
{
    /* done with the desc, kill it */
    AEDisposeDesc(&aeDirDesc)
        ;
    /*  create the file descriptor and add to aliasList */
    HLock((Handle)FileAlias);
    AECreateDesc(typeAlias, (Ptr)*FileAlias, GetHandleSize((Handle)FileAlias), &listElem);
    HLock((Handle)FileAlias);
    DisposHandle((Handle)FileAlias);
    myErr = AEPutDesc(&fileList, 0, &listElem);
}

if (myErr)
return(myErr);
AEDisposeDesc(&listElem);
/* Add the file alias list to the event */
if (myErr = AEPutParamDesc(&aeEvent, aeSelectionKeyword, &fileList))
return(myErr);
myErr = AEDisposeDesc(&fileList);

/* And now send the event! */
myErr = AESend(&aeEvent, nil, kAENoReply + kAEAlwaysInteract + kAECanSwitchLayer, kAENormalPriority, kAEDefaultTimeout, nil, nil);

/* and kill the memory used */
AEDisposeDesc(&aeEvent);
}


/* This runs through the process list looking for the indicated application */
OSErr FindAProcess(OSType typeToFind, OSType creatorToFind, ProcessSerialNumberPtr processSN)
{
    ProcessInfoRec tempInfo;
    FSSpec procSpec;
    Str31 processName;
    OSErr myErr = noErr;
    /* nul out the PSN so we're starting at the beginning of the list */
    processSN->lowLongOfPSN = kNoProcess;
    processSN->highLongOfPSN = kNoProcess;
    /* initialize the process information record */
    tempInfo.processInfoLength = sizeof(ProcessInfoRec);
    tempInfo.processName = &processName;
    tempInfo.processAppSpec = &procSpec;
    /* loop through all the processes until we */
    /* 1) find the process we want */
    /* 2) error out because of some reason (usually, no more processes */
    do {
        myErr = GetNextProcess(processSN);
        if (myErr == noErr)
            GetProcessInformation(processSN, &tempInfo);
    }
            while ((tempInfo.processSignature != creatorToFind || tempInfo.processType != typeToFind) ||
                   myErr != noErr);
    return(myErr);
}


