/*
** James "im" Beninghaus
*/


#include	<Errors.h>
#include	<OSUtils.h>
#include	<Processes.h>
#include	<stdio.h>

main () {
	
	auto	OSErr				osErr			= noErr;
	auto	ProcessSerialNumber	process;
	auto	ProcessInfoRec		procInfo;
	auto	Str255				fileName,
								procName;
	auto	DateTimeRec			launchDateTime;			

	InitGraf((Ptr) &qd.thePort);
	
	
	printf(
		"         Process Name                 Number        Type  Signature      Mode   Location   Size   FreeMem      Launcher         LaunchDate        ActiveTime  FileDirID VRefNum  FileName                        "                        
		"\n"
		"-------------------------------- -----------------  ----    ----       -------- -------- -------- -------- ----------------- -------------------   --------   --------  ----     --------------------------------"
		"\n"
	);
	
	process.HighLongOfPSN = kNoProcess;
	process.LowLongOfPSN  = kNoProcess;
	
	procInfo.processInfoLength	= sizeof(procInfo);
	procInfo.processName		= procName;
	procInfo.processFileName	= fileName;

	while (procNotFound != (osErr = GetNextProcess(&process))) {
		if (noErr == (osErr = GetProcessInformation(&process, &procInfo))) {
			if ('\0' == procName[1])
				procName[1] = '0';
			Secs2Date(procInfo.processLaunchDate, &launchDateTime);
			printf(
				"%-32P %08lx.%08lx '%c%c%c%c'  '%c%c%c%c'      %08lx %08lx %08lx %08lx %08lx.%08lx %2d/%2d/%2d %2d:%02d:%02d   %08lx   %08lx  %04x     %P\n",
				procInfo.processName,
				procInfo.processNumber.HighLongOfPSN,
				procInfo.processNumber.LowLongOfPSN,
				((char *) &procInfo.processType)[0],
				((char *) &procInfo.processType)[1],
				((char *) &procInfo.processType)[2],
				((char *) &procInfo.processType)[3],
				((char *) &procInfo.processSignature)[0],
				((char *) &procInfo.processSignature)[1],
				((char *) &procInfo.processSignature)[2],
				((char *) &procInfo.processSignature)[3],
				procInfo.processMode,
				procInfo.processLocation,
				procInfo.processSize,
				procInfo.processFreeMem,
				procInfo.processLauncher.HighLongOfPSN,
				procInfo.processLauncher.LowLongOfPSN,
				launchDateTime.month,
				launchDateTime.day,
				launchDateTime.year,
				launchDateTime.hour,
				launchDateTime.minute,
				launchDateTime.second,
				procInfo.processActiveTime,
				procInfo.processFileDirID,
				procInfo.processFileVRefNum,
				procInfo.processFileName
			);
		}
	}
}