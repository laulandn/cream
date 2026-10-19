/* FinderOpenSel 1.1 */
/* This sample shows how to use the Finder OpenSelection AppleEvent. */
/* You can use this event to launch applications or files, or open Finder windows */
/* Version 1.1 here also shows you how to launch an application remotely */
/* Written  by C.K. Haun <TR> */
/* Apple Developer Tech Support */
/* April 1992, Cupertino, CA USA */
/* Of course, Copyright 1991-1992, Apple Computer Inc. */
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
#include <Balloons.h>
#include <ALiases.h>
#include <Processes.h>
#include <StandardFile.h>
#include <Folders.h>

/* prototypes */
void InitalizeApp(void);
void DoDiskEvents(long dinfo);                              /* hi word is error code, lo word is drive number */
void DrawMain(WindowPtr drawIt);
Boolean DoSelected(long val);
OSErr FindAProcess(OSType typeToFind, OSType creatorToFind, ProcessSerialNumberPtr processSN, ProcessInfoRecPtr infoRecToFill);
OSErr OpenSelection(short which);
void InitAEStuff(void);
void DoHighLevel(EventRecord *AERecord);
void DoDaCall(MenuHandle themenu, long theit);
pascal OSErr AEOpenHandler(AppleEvent *messagein, AppleEvent *reply, long refIn);
pascal OSErr AEOpenDocHandler(AppleEvent *messagein, AppleEvent *reply, long refIn);
pascal OSErr AEPrintHandler(AppleEvent *messagein, AppleEvent *reply, long refIn);
pascal OSErr AEQuitHandler(AppleEvent *messagein, AppleEvent *reply, long refIn);
void SampleHelpDialog(void);
OSErr GetPathAndName(Str255 fullPath, Str255 appName);
OSErr BrowseFerMe(void);
pascal Boolean FinderFilter(LocationNamePtr locationName, PortInfoPtr thePortInfo);
ControlHandle SnatchHandle(DialogPtr thebox, short theGetItem);
pascal Boolean idleProc(EventRecord *eventIn, long *sleep, RgnHandle *mouseRgn);

long trashDirID;
short trashVRef;
/* These are the new Dialog Manager calls described in Tech note #304 */
/* ONLY used if System 7 or later, of course */
pascal OSErr GetStdFilterProc(ModalFilterProcPtr *theProc) = 
{
    0x303C, 0x0203, 0xAA68
};




/* Indicates to the dialog manager which item is default.  Will then alias the return key */
/* to this item, and also bold border it for you (yaaaaa!) */
pascal OSErr SetDialogDefaultItem(DialogPtr theDialog, short newItem)
= 
{
    0x303C, 0x0304, 0xAA68
};




/* Indicates which item should be aliased to escape or Command - . */
pascal OSErr SetDialogCancelItem(DialogPtr theDialog, short newItem)
= 
{
    0x303C, 0x0305, 0xAA68
};




/* Tells the dialog manager that there is an edit line in this dialog, and */
/* it should track and change to an I-Beam cursor when over the edit line */
pascal OSErr SetDialogTracksCursor(DialogPtr theDialog, Boolean tracks) = 
{
    0x303C, 0x0306, 0xAA68
};




/* one external */
extern void _DataInit();                                    /* this is the C initialization code */

#define kWaitDialog 132
#define kMBarID 128
#define kAppleMenu 128
#define kFileMenu 129
#define kEditMenu 130
#define kToolsMenu 131
#define kResumeMask 1 /* bit of message field for resume vs. suspend */
#define kSampHelp 129
#define kAboutBox 128
#define kHelpString 128
#define kNewItem 1
#define kOpenItem 2
#define kCloseItem 3
#define kSaveItem 4
#define kSaveAsItem 5
#define kFileBlank1 6
#define kPageSetupItem 7
#define kPrintItem 8
#define kFileBlank2 9
#define kQuitItem 10
#define kBadSystem 130
#define kOpenAppItem 1
#define kOpenWindowItem 2
#define kRemoteLaunchItem 3
#define kTextType 'TEXT'
#define kTextResID 128
#define kMyWindowResID 128
#define aeSelectionKeyword 'fsel'
#define kAEOpenSelection 'sope'
#define kFinderType 'FNDR'
#define kSysCreator 'MACS'
#define kPathDialog 131
#define kEditLineItem 4
#define kColon ':'
enum  {
    kGenStrings = 128, kPPCPrompt1 = 1, kPPCPrompt2
};

struct AEinstalls {
    AEEventClass theClass;
    AEEventID theEvent;
    EventHandlerProcPtr theProc;
};
typedef struct AEinstalls AEinstalls;

Handle gMymenu;                                             /* my menu bar handle */
MenuHandle gAppleMenuHandle, gFileMenuHandle, gEditMenuHandle, gToolMenuHandle;
Boolean gQuit, gInBackground;
EventRecord gERecord;
AEDesc gTheAddress;
WindowPtr myWindow = 0;
ProcessSerialNumber gOurSN;
short gHelpItem;
LocationNameRec theLoc;
PortInfoRec theRec;
PPCPortRec myPortName;
LocationNameRec myLoc;
AEDesc myAddressDesc;
TargetID theID;

#pragma segment Main

int main(int argc,char **argv)
{
    WindowPtr twindow;
    
    UnloadSeg((Ptr)_DataInit);                              /* throw out setup code */
    InitalizeApp();
    UnloadSeg((Ptr)InitalizeApp);                           /* get rid of my initialization code */
    
    do {
        WaitNextEvent(everyEvent, &gERecord, 5, nil);
        switch (gERecord.what) {
            
            case nullEvent:
                
                /* no nul processing in this sample */
                break;
            case updateEvt:
                DrawMain((WindowPtr)gERecord.message);      /* draw whatever window needs an update */
                break;
            case mouseDown:
                
                /* first see where the hit was */
                
                switch (FindWindow(gERecord.where, &twindow)) {
                    
                    case inDesk:                            /* if they hit in desk, then the process manager */
                        break;                              /* will switch us out, we don't need to do anything */
                    case inMenuBar:
                        DoSelected(MenuSelect(gERecord.where));
                        break;
                        
                    case inSysWindow:
                        /* pass to the system */
                        SystemClick(&gERecord, twindow);
                        break;
                        
                    case inContent:
                        /* Handle content and control clicks here */
                        break;
                        
                    case inDrag:
                        if (twindow == FrontWindow())
                            DragWindow(twindow, gERecord.where, &qd.screenBits.bounds);
                        break;
                        
                    case inGrow:
                        /* Call GrowWindow here if you have a grow box */
                        break;
                        
                    case inGoAway:
                        /* Click in Close box */
                        break;
                        
                }
            case mouseUp:
                /* don't care */
                break;
                
                /* same action for key or auto key */
                
            case keyDown:
            case autoKey:
                if (gERecord.modifiers & cmdKey)
                    DoSelected(MenuKey(gERecord.message & charCodeMask));
                break;
                
            case keyUp:
                /* don't care */
                break;
                
            case diskEvt:
                /* I don't do anything special for disk events, this just passes them */
                /* to a function that checks for an error on the mount */
                DoDiskEvents(gERecord.message);
                break;
                
            case activateEvt:
                if (gERecord.modifiers & activeFlag)
                    DrawMain((WindowPtr)gERecord.message);
                break;
                
            case networkEvt:
                /* don't care */
                break;
                
            case driverEvt:
                /* don't care */
                break;
                
            case app4Evt:
                switch ((gERecord.message >> 24) & 0x0FF) {     /* high byte of message */
                    case suspendResumeMessage:              /* suspend/resume is also an activate/deactivate */
                        gInBackground = (gERecord.message & kResumeMask) == 0;
                        break;
                }
                break;
                
                /* This dispatches high level events (AppleEvents, for example) */
                /* to our dispatch routine.  This is NEW in the event loop for */
                /* System 7 */
                
            case kHighLevelEvent:
                DoHighLevel(&gERecord);
                break;
                
            default:
                break;
                
        }
    }
            while (gQuit != true);
    
}

/* DoDaCall opens the requested DA.  It's here as a seperate routine if you'd */
/* like to perform some action or just know when a DA is opened in your */
/* layer.  Can be handy to track memory problems when a DA is opened */
/* with an Option-open */
void DoDaCall(MenuHandle themenu, long theit)
{
    long qq;
    char DAname[255];
    GetItem(themenu, theit, &DAname);
    qq = OpenDeskAcc(DAname);
}

/* end DoDaCall */

/* DoDiskEvents just checks the error code from the disk mount, */
/* and puts up the 'Format' dialog (through DIBadMount) if need be */
/* You can do much more here if you care about what disks are */
/* in the drive */
void DoDiskEvents(long dinfo)                               /* hi word is error code, lo word is drive number */
{
    short hival, loval, tommy;
    Point fredpoint =  {
        40, 40
    };
    hival = HiWord(dinfo);
    loval = LoWord(dinfo);
    if (hival != noErr)                                     /* something happened */ {
        tommy = DIBadMount(fredpoint, dinfo);
    }
}

/* draws my window.  Pretty simple */
void DrawMain(WindowPtr drawIt)
{
    Handle theText;
    BeginUpdate(drawIt);
    SetPort(drawIt);
    EraseRect(&drawIt->portRect);
    theText = GetResource(kTextType, kTextResID);
    if (theText) {
        Rect textRect;
        HLock(theText);
        HUnlock(theText);
        textRect = drawIt->portRect;
        InsetRect(&textRect, 5, 5);
        TextBox(*theText, GetHandleSize(theText), &textRect, 0);
        ReleaseResource(theText);
    }
    EndUpdate(drawIt);
}

/* my menu action taker.  It returns a Boolean which I usually ignore, but it */
/* mught be handy someday */
Boolean DoSelected(long val)
{
    short loval, hival;
    Boolean returnVal = false;
    loval = LoWord(val);
    hival = HiWord(val);
    
    switch (hival) {                                        /* switch off the menu number selected */
        
        case kAppleMenu:                                    /* Apple menu */
            if (loval != 1) {                               /* if this was not About, it's a DA */
                DoDaCall(gAppleMenuHandle, loval);
            } else {
                Alert(kAboutBox, nil);                      /* do about box */
            }
            returnVal = true;
            break;
            
        case kFileMenu:                                     /* File menu */
            switch (loval) {
                
                case kQuitItem:
                    gQuit = true;                           /* only  item */
                    returnVal = true;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case kEditMenu:
            /* edit menu junk */
            /* don't care */
            break;
            
        case kToolsMenu:
            /* add all your test stuff here */
            OpenSelection(loval);
            break;
            
        case kHMHelpMenuID:                                 /* Defined in Balloons.h */
            /* I only care about this item.  If anything else is returned here, I don't know what */
            /* it is, so I leave it alone.  Remember, the Help Manager chapter says that */
            /* Apple reserves the right to add and change things in the Help menu */
            if (loval == gHelpItem)
                SampleHelpDialog();
            break;
            
    }
    HiliteMenu(0);
    return(returnVal);
}

/* InitAEStuff installs my appleevent handlers */
void InitAEStuff(void)
{
    static AEinstalls HandlersToInstall[] =  {
        {
            kCoreEventClass, kAEOpenApplication, AEOpenHandler
        },  {
            kCoreEventClass, kAEOpenDocuments, AEOpenDocHandler
        },  {
            kCoreEventClass, kAEQuitApplication, AEQuitHandler
        },  {
            kCoreEventClass, kAEPrintDocuments, AEPrintHandler
        }, 
        /* The above are the four required AppleEvents. */
        
    };
    
    OSErr aevtErr = noErr;
    long aLong = 0;
    Boolean gHasAppleEvents = false;
    
    /* Check this machine for AppleEvents.  If they are not here (ie not 7.0)
    *   then we exit */
    
    gHasAppleEvents = (Gestalt(gestaltAppleEventsAttr, &aLong) == noErr);
    
    /* The following series of calls installs all our AppleEvent Handlers.
    *   These handlers are added to the application event handler list that 
    *   the AppleEvent manager maintains.  So, whenever an AppleEvent happens
    *   and we call AEProcessEvent, the AppleEvent manager will check our
    *   list of handlers and dispatch to it if there is one.
    */
    
    if (gHasAppleEvents) {
        register qq;
        for (qq = 0; qq < ((sizeof(HandlersToInstall) / sizeof(AEinstalls))); qq++) {
            
            aevtErr = AEInstallEventHandler(HandlersToInstall[qq].theClass, HandlersToInstall[qq].theEvent,
                                            HandlersToInstall[qq].theProc, 0, false);
            if (aevtErr) {
                ExitToShell();                              /* just fail, baby */
            }
        }
    } else {
        ExitToShell();
    }
}

/* end InitAEStuff */
/* I'm not doing error handling in this sample for clarities sake, you should. Hah, */
/* easy for me to say, huh? */
void DoHighLevel(EventRecord *AERecord)
{
    
    AEProcessAppleEvent(AERecord);
    
}

/* end DoHighLevel */

/* This is the standard Open Application event.  */
pascal OSErr AEOpenHandler(AppleEvent *messagein, AppleEvent *reply, long refIn)
{
#pragma unused (messagein,reply,refIn)
    /* we of course don't do anything here in this simple app */
    /* except open our window */
    
    myWindow = GetNewWindow(kMyWindowResID, nil, (WindowPtr)-1);
    return(noErr);
}

/* end AEOpenHandler */

/* Open Doc, opens our documents.  Remember, this can happen at application start AND */
/* anytime else.  If your app is up and running and the user goes to the desktop, hilites one */
/* of your files, and double-clicks or selects Open from the finder File menu this event */
/* handler will get called. Which means you don't do any initialization of globals here, or */
/* anything else except open then doc.  */
/* SO-- Do NOT assume that you are at app start time in this */
/* routine, or bad things will surely happen to you. */

pascal OSErr AEOpenDocHandler(AppleEvent *messagein, AppleEvent *reply, long refIn)
{
#pragma unused (messagein,refIn,reply)
    /* we of course don't do anything here */
    
    return(errAEEventNotHandled);                           /* we have no docs, so no odoc events should come to us */
}

pascal OSErr AEPrintHandler(AppleEvent *messagein, AppleEvent *reply, long refIn)
{                                                           /* no printing handler in yet, so we'll ignore this */
    /* the operation is functionally identical to the ODOC event, with the additon */
    /* of calling your print routine.  */
#pragma unused (messagein,refIn,reply)
    
    /* we of course don't do anything here */
    return(errAEEventNotHandled);                           /* we have no docs, so no pdoc events should come to us */
}

/* Standard Quit event handler, to handle a Quit event from the Finder, for example.  */
/* еееее DO NOT CALL EXITTOSHELL HERE еееее or you will never have a happy life.  */
pascal OSErr AEQuitHandler(AppleEvent *messagein, AppleEvent *reply, long refIn)
{
#pragma unused (messagein,refIn,reply)
    
    /* prepQuit sets the Stop flag for us.  It does _NOT_ quit, you */
    /* should NEVER quit from an AppleEvent handler.  Calling */
    /* ExitToShell here would blow things up */
    gQuit = true;
    return(noErr);
}

/* This is my sample help dialog.  Does not do anything, expand as you need */
void SampleHelpDialog(void)
{
    DialogPtr tdial = GetNewDialog(kSampHelp, nil, (WindowPtr)-1);
    short itemhit = 0;
    while (itemhit != 1) {
        ModalDialog((ModalFilterProcPtr)nil, &itemhit);
    }
    DisposDialog(tdial);
}

/* OpenSelection perpares and sends the Finder OpenSelection event */
/* Three choices currently.  If which is 1, then we do an OpenSelection on the file itself. */
/* if which is 2, then we tell the finder to open the directory window containing the file  */
/* if it's 3, then we ask for a remote name and launch remotely */

OSErr OpenSelection(short which)
{
    AppleEvent aeEvent, aeReply;
    AEDesc aeDirDesc, listElem;
    OSErr interactErr = noErr;
    FSSpec dirSpec, procSpec;
    AEDesc fileList;
    StandardFileReply myReply;
    OSErr myErr;
    ProcessSerialNumber process;
    AliasHandle DirAlias, FileAlias;
    FSSpecPtr theFileToOpen = nil;
    ProcessInfoRec infoRec;
    Str31 processName;
    Str255 fullPath;
    Str255 appName;
    
    if (which != kRemoteLaunchItem)
        StandardGetFile(nil, -1, nil, &myReply);
    else
        myReply.sfGood = true;                              /* set this since I want to be silent this time */
    if (myReply.sfGood) {
        theFileToOpen = &myReply.sfFile;
        infoRec.processInfoLength = sizeof(ProcessInfoRec);
        infoRec.processName = &processName;
        infoRec.processAppSpec = &procSpec;
        
        /* Browse for the Finder you wish to communicate with */
        
        if (which != kRemoteLaunchItem) {
            /* if less than kRemoteLaunchItem, then we are working locally, find the finder on this machine */
            
            myErr = FindAProcess(kFinderType, kSysCreator, &process, &infoRec);
            if (myErr == noErr)
                myErr = AECreateDesc(typeProcessSerialNumber, (Ptr)&process, sizeof(process), &myAddressDesc);
            
        } else {
            
            /* Get a remote finder  */
            myErr = BrowseFerMe();
        }
        if (myErr == noErr) {
            
            /* Create the FinderEvent */
            myErr = AECreateAppleEvent(kFinderType, kAEOpenSelection, &myAddressDesc, kAutoGenerateReturnID, kAnyTransactionID,
                                       &aeEvent);
            /* If you want to keep talking to this machine, you can keep this  */
            /* address desc around */
            AEDisposeDesc(&myAddressDesc);
            
            if (myErr == noErr) {
                /* Now we build all the bits of an OpenSelection event. */
                /* Basically, we need to create an alias for the item to open, and an alias to the parent */
                /* folder (directory) of that item. */
                /* We can also pass a list of files if we want.  */
                /* You'll notice that for opening a finder window, the file and directory alias both point at the */
                /* folder itself */
                /* make a spec for the parent folder */
                
                if (which != kRemoteLaunchItem) {
                    FSMakeFSSpec(theFileToOpen->vRefNum, theFileToOpen->parID, nil, &dirSpec);
                    NewAlias(nil, &dirSpec, &DirAlias);
                }
                /* Create alias for file */
                /* if you are opening a window, then you make the file alias the same as the dir alias */
                switch (which) {
                    
                    case kOpenAppItem:
                        NewAlias(nil, theFileToOpen, &FileAlias);
                        break;
                        
                    case kOpenWindowItem:
                        NewAlias(nil, &dirSpec, &FileAlias);
                        
                        /* I want to set the Finder as the front process after the event  */
                        /* is sent (if I can) so you can see what happened.  Rememeber, app switches */
                        /* don't happen until _after_ (or during) the next WaitNextEvent call */
                        
                        interactErr = AEInteractWithUser(kAEDefaultTimeout, nil, (IdleProcPtr)idleProc);
                        if (interactErr == noErr)
                            SetFrontProcess(&process);
                        break;
                        
                    case kRemoteLaunchItem:
                        myErr = GetPathAndName(fullPath, appName);
                        if (myErr == noErr) {
                            /* NewAliasMinimalFromFullPath (geez what a lot of typing) creates an alias that does NOT */
                            /* have a file ID, just builds based on what it has, the path.  So we don't need to */
                            /* connect to another machine or file server to make this alias, which is why this thing works. */
                            
                            NewAliasMinimalFromFullPath(appName[0], (Ptr)&appName[1], "", "", &FileAlias);
                            NewAliasMinimalFromFullPath(fullPath[0], (Ptr)&fullPath[1], "", "", &DirAlias);
                        }
                        break;
                }
                /* Create the file  list */
                if (myErr == noErr) {
                    myErr = AECreateList(nil, 0, false, &fileList);
                    
                    /*  create the folder  descriptor */
                    HLock((Handle)DirAlias);
                    AECreateDesc(typeAlias, (Ptr)*DirAlias, GetHandleSize((Handle)DirAlias), &aeDirDesc);
                    HUnlock((Handle)DirAlias);
                    if ((myErr = AEPutParamDesc(&aeEvent, keyDirectObject, &aeDirDesc)) == noErr) {
                        /* done with the desc, kill it */
                        AEDisposeDesc(&aeDirDesc);
                        /*  create the file descriptor and add to aliasList */
                        HLock((Handle)FileAlias);
                        AECreateDesc(typeAlias, (Ptr)*FileAlias, GetHandleSize((Handle)FileAlias), &listElem);
                        HLock((Handle)FileAlias);
                        myErr = AEPutDesc(&fileList, 0, &listElem);
                    }
                    if (myErr == noErr) {
                        
                        AEDisposeDesc(&listElem);
                        
                        /* Add the file alias list to the event */
                        myErr = AEPutParamDesc(&aeEvent, aeSelectionKeyword, &fileList);
                        AEDisposeDesc(&fileList);
                        
                        if (myErr == noErr)
                            myErr = AESend(&aeEvent, &aeReply, kAENoReply + kAEAlwaysInteract + kAECanSwitchLayer,
                                           kAENormalPriority, kAEDefaultTimeout, nil, nil);
                    }
                }
                AEDisposeDesc(&aeEvent);
            }
        }
    }
    if ((Handle)DirAlias)
        DisposHandle((Handle)DirAlias);
    if ((Handle)FileAlias)
        DisposHandle((Handle)FileAlias);
    return(myErr);
}



/* This runs through the process list looking for the indicated application */
OSErr FindAProcess(OSType typeToFind, OSType creatorToFind, ProcessSerialNumberPtr processSN, ProcessInfoRecPtr infoRecToFill)
{
    ProcessSerialNumber tempPSN;
    OSErr myErr = noErr;
    tempPSN.lowLongOfPSN = kNoProcess;
    processSN->lowLongOfPSN = kNoProcess;
    processSN->highLongOfPSN = kNoProcess;
    do {
        myErr = GetNextProcess(processSN);
        if (myErr == noErr)
            GetProcessInformation(processSN, infoRecToFill);
    }
            while ((infoRecToFill->processSignature != creatorToFind || infoRecToFill->processType != typeToFind) ||
                   myErr != noErr);
    
    return(myErr);
}

/*  BrowseFerMe calls the PPC browser and creates an AEAddressDesc out of the Browser */
/* info if the user (you) doesn't cancel */

OSErr BrowseFerMe(void)
{
    OSErr myErr = noErr;
    Str255 tWext;
    
    Str255 t2Wext;
    GetIndString(tWext, kGenStrings, kPPCPrompt1);
    GetIndString(t2Wext, kGenStrings, kPPCPrompt2);
    myErr = PPCBrowser(tWext, &t2Wext, false, &theLoc, &theRec, (PPCFilterProcPtr)FinderFilter, nil);
    if (myErr == noErr) {
        theID.name = theRec.name;
        theID.location = theLoc;
        myErr = AECreateDesc(typeTargetID, (Ptr)&theID, sizeof(theID), &myAddressDesc);
    }
    return(myErr);
}

/* Show only FInders */
pascal Boolean FinderFilter(LocationNamePtr locationName, PortInfoPtr thePortInfo)
{
#pragma unused (locationName)
    OSType type;
    
    if (thePortInfo->name.portKindSelector == ppcByString) {
        BlockMove(thePortInfo->name.u.portTypeStr + 1, (Ptr)&type, 4);
        /* The BlockMove is so that we don't get an address error
        ** on a 68000-based machine due to referencing a long at
        ** an odd-address. */
        if (type == kSysCreator)
            return(true);
    }
    return(false);
}

OSErr GetPathAndName(Str255 fullPath, Str255 appName)
{
    DialogPtr tdial = GetNewDialog(kPathDialog, nil, (WindowPtr)-1);
    short hitItem = 0;
    Str255 tempString;
    short where;
    ModalFilterProcPtr filterIt = nil;
    
    OSErr retVal = userCanceledErr;
    SetDialogDefaultItem(tdial, ok);
    SetDialogCancelItem(tdial, cancel);
    SetDialogTracksCursor(tdial, true);
    GetStdFilterProc(&filterIt);
    ShowWindow(tdial);
    DrawDialog(tdial);
    do {
        ModalDialog((ModalFilterProcPtr)filterIt, &hitItem);
    }
            while (hitItem != ok && hitItem != cancel);
    if (hitItem == ok) {
        GetIText((Handle)SnatchHandle(tdial, kEditLineItem), tempString);
        /* now parse backwards to find the last colon, which is seperating the file name from the path */
        where = tempString[0];
        while (tempString[where] != kColon && where > 0)
            where--;
        if (where > 0) {
            BlockMove((Ptr)&tempString[0], (Ptr)&appName[0], tempString[0] + 1);
            BlockMove((Ptr)&tempString[0], (Ptr)&fullPath[0], where + 1);       /* include the colon */
            fullPath[0] = where;
            retVal = noErr;
        }
    }
    DisposDialog(tdial);
    
    return(retVal);
}

ControlHandle SnatchHandle(DialogPtr thebox, short theGetItem)
{
    short itemtype;
    Rect itemrect;
    Handle thandle;
    
    GetDItem(thebox, theGetItem, &itemtype, &thandle, &itemrect);
    return((ControlHandle)thandle);
}

/* end SnatchHandle */

pascal Boolean idleProc(EventRecord *eventIn, long *sleep, RgnHandle *mouseRgn)
{
    switch (eventIn->what) {
        case nullEvent:
            /* no nul processing in this sample */
            *sleep = 0;
            mouseRgn = nil;
            break;
        case updateEvt:
        case activateEvt:
            DrawMain((WindowPtr)eventIn->message);          /* draw whatever window needs an update */
            break;
        case app4Evt:
            switch ((gERecord.message >> 24) & 0x0FF) {     /* high byte of message */
                case suspendResumeMessage:                  /* suspend/resume is also an activate/deactivate */
                    gInBackground = (gERecord.message & kResumeMask) == 0;
                    break;
            }
            break;
            
    }
    return(false);                                          /* I'll wait forever */
}


#pragma segment Initialize
void InitalizeApp(void)
{
    MenuHandle helpHandle;
    StringHandle helpString;
    short count;
    long vers;
    MaxApplZone();
    InitGraf((Ptr)&qd.thePort);
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    InitCursor();
    /* Check system version */
    Gestalt(gestaltSystemVersion, &vers);
    vers = (vers >> 8) & 0xf;                               /* shift result over and mask out major version number */
    if (vers < 7) {
        StopAlert(kBadSystem, nil);
        ExitToShell();
    }
    InitAEStuff();
    /* set up my menu junk */
    gMymenu = GetNewMBar(kMBarID);
    SetMenuBar(gMymenu);
    gAppleMenuHandle = GetMHandle(kAppleMenu);
    gFileMenuHandle = GetMHandle(kFileMenu);
    gEditMenuHandle = GetMHandle(kEditMenu);
    gToolMenuHandle = GetMHandle(kToolsMenu);
    AddResMenu(gAppleMenuHandle, 'DRVR');
    /* now install my Help menu item in the Help Manager's menu */
    HMGetHelpMenuHandle(&helpHandle);                       /* Get the Hlpe menu handle */
    count = CountMItems(helpHandle);                        /* How many items are there? */
    helpString = GetString(kHelpString);                    /* get my help string */
    DetachResource(helpString);                             /* detach it */
    HNoPurge(helpString);
    MoveHHi((Handle)helpString);
    HLock((Handle)helpString);
    InsMenuItem(helpHandle, (Ptr)*helpString, count + 1);       /* insert my item in the Help menu */
    gHelpItem = CountMItems(helpHandle);                    /* The number of the item */
    
    DrawMenuBar();
    GetCurrentProcess(&gOurSN);                             /* Get our process serial number for later use, if needed */
}
