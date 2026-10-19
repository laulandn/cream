/*	open document snippet
	Steven Falkenburg 10/2/91
	
	This sample sends an 'odoc' event to a specified running application
*/

#include <PPCToolBox.h>
#include <AppleEvents.h>
#include <Aliases.h>

void main(void);
void InitStuff(void);
OSErr GetTargetAddress(Str255 prompt,Str255 appStr,PortInfoRec *myPortInfo,
						AEAddressDesc *targetAddress,TargetID *toTargetID);
OSErr SendODOC(AEAddressDesc *targetAddress,AliasHandle targetFile);
Boolean GetFileToOpen(FSSpec *target);

void main(void)
{
	PortInfoRec portInfo;
	AEAddressDesc targetAddress;
	TargetID targetID;
	OSErr err;
	FSSpec fileSpec;
	AliasHandle aliasTarget;
	
	InitStuff();
	
	if (!GetFileToOpen(&fileSpec))
		ExitToShell();
	
	if (GetTargetAddress("\pSpecify target of ODOC:",nil,&portInfo,&targetAddress,&targetID)==noErr) {
		err = NewAlias(nil,&fileSpec,&aliasTarget);
		if (err==noErr) {
			err = SendODOC(&targetAddress,aliasTarget);
			err = AEDisposeDesc(&targetAddress);
			DisposHandle(aliasTarget);
		}
	}
}


void InitStuff(void)
{
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	InitCursor();
	FlushEvents(everyEvent,0);
}


Boolean GetFileToOpen(FSSpec *target)
{
	StandardFileReply sfReply;
	
	StandardGetFile(nil,-1,nil,&sfReply);
	BlockMove(&sfReply.sfFile,target,sizeof(FSSpec));
	
	return sfReply.sfGood;
}


OSErr GetTargetAddress(Str255 prompt,Str255 appStr,PortInfoRec *myPortInfo,
						AEAddressDesc *targetAddress,TargetID *toTargetID)
{
	OSErr err;
	
	err = PPCBrowser(prompt,appStr,false,&toTargetID->location,myPortInfo,nil,"\p");
	if (err==noErr) {
		BlockMove(&myPortInfo->name,&toTargetID->name,256);
		err = AECreateDesc(typeTargetID,(Ptr)toTargetID,sizeof(TargetID),targetAddress);
	}
	
	return err;
}


OSErr SendODOC(AEAddressDesc *targetAddress,AliasHandle targetFile)
{
	OSErr err;
	AppleEvent theAE,reply;
	AEDescList descList;
	
	reply.dataHandle = nil;
	
	err = AECreateAppleEvent(kCoreEventClass,kAEOpenDocuments,targetAddress,
				kAutoGenerateReturnID,kAnyTransactionID,&theAE);
	if (err!=noErr)
		return err;
	
	err = AECreateList(nil,0,false,&descList);
	if (err!=noErr)
		return err;
	
	HLock(targetFile);
	err = AEPutPtr(&descList,0,typeAlias,(Ptr)*targetFile,sizeof(AliasHandle)+(**targetFile).aliasSize);
	HUnlock(targetFile);
	if (err!=noErr)
		return err;
	
	err = AEPutParamDesc(&theAE,keyDirectObject,&descList);
	if (err!=noErr)
		return err;
	
	err = AESend(&theAE,&reply,kAENoReply,kAENormalPriority,kAEDefaultTimeout,nil,nil);
	
	err = AEDisposeDesc(&descList);
	err = AEDisposeDesc(&theAE);
	if (reply.dataHandle)
		err = AEDisposeDesc(&reply);
}
