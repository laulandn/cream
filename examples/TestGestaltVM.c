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
	err = Gestalt(gestaltVMAttr, &feature);
	if (!err) {
		if (feature & 0x0001)
			printf ("We have VM\n");
		else
			printf ("We don't have VM\n");
		}
	else 
		printf ("Gestalt err = %i\n",err);
	}
else
	printf ("No Gestalt\n");
}