/* LaunchWithDoc */
/* The smallest LaunchAplication example I could come up with. */
/* this launches an application with a 'odoc' AppleEventª */
/* this little bit of code here uses two Standard File calls to get */
/* the application to launch and the file to launch with, you will of course */
/* want to replace these with whatever you want to do */
/* ¥¥¥ NOTE: This also works for launching non-System 7 applications.  */
/* If the Finderª sees an 'odoc' or 'pdoc' Apple event in the launch block */
/* and the application being launched is NOT system 7 aware, the Finder */
/* coerces the Apple event into puppetstrings.  So you */
/* don't need to special case for non-7.0 applications.  */
/* Pretty neat, huh? */
/* ¥¥¥ Important: I'm not doing any error checking, you of course should */
/* so your users don't hunt you down and burn your keyboard. */
/* C.K. Haun */
/* Apple DTS */
#include <Dialogs.h>
#include <QuickDraw.h>
#include <Windows.h>
#include <Menus.h>
#include <Fonts.h>
#include <appleevents.h>
#include <processes.h>
#include <files.h>
#include <StandardFile.h>
#include <Aliases.h>

int main(int argc,char **argv)
{
    FSSpec launchApp, fileToLaunch;   /* file spec for the app (launchApp) and the file (fileToLaunch) */
    LaunchParamBlockRec launchThis;
    AEDesc myAddress;
    AEDesc docDesc, launchDesc;
    AEDescList theList;
    AliasHandle withThis;
    StandardFileReply myReply;
    AppleEvent theEvent;
    InitGraf((Ptr)&qd.thePort);  /* standard setup stuff */
    InitFonts();
    InitWindows();
    InitMenus();
    TEInit();
    InitDialogs(nil);
    InitCursor();
    /* get the app to launch, using the Sys7 versions of Standard File */
	/* which return FSSpecs */
    StandardGetFile(nil, -1, nil, &myReply);
    if (!myReply.sfGood)
        return;
    launchApp = myReply.sfFile;
	/* stuff it in my launch parameter block */
    launchThis.launchAppSpec = &launchApp;
    
    /* get the file to pass */
    StandardGetFile(nil, -1, nil, &myReply);
    if (!myReply.sfGood)
        return;
    
    /* the caller may have already done this, but it doesn't hurt to do it again */
    fileToLaunch = myReply.sfFile;
    /* create an appleevent to carry it */
    AECreateAppleEvent(kCoreEventClass, kAEOpenDocuments, &myAddress, kAutoGenerateReturnID, kAnyTransactionID, &theEvent);
    /* create a list for the alaises.  In this case, I only have one, but you still need */
    /* a list */
    AECreateList(nil, 0, false, &theList);
    /* create an alias out of the file spec */
    /* I'm not real sure why I did this, since there is a system coercion handler for */
    /* alias to FSSpec, but I'm paranoid */
    NewAlias(nil, &fileToLaunch, &withThis);
    HLock((Handle)withThis);
    /* now create an alias descriptor */
    AECreateDesc(typeAlias, (Ptr)*withThis, GetHandleSize((Handle)withThis), &docDesc);
    HUnlock((Handle)withThis);
    /* put it in the list */
    AEPutDesc(&theList, 0, &docDesc);
    AEPutParamDesc(&theEvent, keyDirectObject, &theList);
    /* coerce the event from being an event into being appParms */
    /* ¥¥¥¥ If you just want to send an 'odoc' to an application that is */
    /* already running, you can stop here and do an AESend on theEvent */
    AECoerceDesc(&theEvent, typeAppParameters, &launchDesc);
    HLock((Handle)theEvent.dataHandle);
    /* and stuff it in the parameter block */
    /* This is a little weird, since we're actually moving the event out of the */
    /* AppParameters descriptor.  But it's necessary, the coercison to typeAppParameters */
    /* stuffs the whole appleevent into one AERecord (instead of a AEDesc) so  */
    /* the Finder gets the whole event as one handle.  It can then parse it itself */
    /* to do the sending */
    launchThis.launchAppParameters = (AppParametersPtr)*(launchDesc.dataHandle);
    /* launch the thing */
    launchThis.launchBlockID = extendedBlock;
    launchThis.launchEPBLength = extendedBlockLen;
    launchThis.launchFileFlags = nil;
    launchThis.launchControlFlags = launchContinue + launchNoFileFlags;
    LaunchApplication(&launchThis);
    /* and launch it */
    
    
}
