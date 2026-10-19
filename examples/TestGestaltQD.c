#ifndef __CTYPE__
#include <CType.h>
#endif

#ifndef __OSUTILS__
#include <OSUtils.h>
#endif

#ifndef __STDIO__
#include <StdIO.h>
#endif

#ifndef __GESTALTEQU__
#include <Gestalt.h>
#endif

#ifndef __TYPES__
#include <Types.h>
#endif

#define	TRUE			0xFF
#define	FALSE			0

#define Gestalttest			0xA1AD
#define NoTrap			0xA89F

int main(int argc,char **argv)
{
OSErr		err;
long		feature;

if ((GetTrapAddress(Gestalttest) != GetTrapAddress(NoTrap))) {
	err = Gestalt(gestaltQuickdrawVersion, &feature);
	if (!err) {
		if ((feature & 0x0f00) == 0x0000)
			printf ("We have Original QuickDraw version 0.%x\n", (feature & 0x00ff));
		else if ((feature & 0x0f00) == 0x0100)
			printf ("We have 8 Bit QuickDraw version 1.%x\n", (feature & 0x00ff));
		else if ((feature & 0x0f00) == 0x0200)
			printf ("We have 32 Bit QuickDraw version 2.%x\n", (feature & 0x00ff));
		else
			printf ("We don't have QD\n");
		}
	else 
		printf ("Gestalt err = %i\n",err);
	}
else
	printf ("No Gestalt\n");
}