#ifndef VMSCONIO_H
#define VMSCONIO_H

/*
 * This code defines an equivalent version of kbhit() and getch() for
 * use under VAX/VMS, together with an exit handler to reset terminal
 * characteristics.
 *
 * This code assumes that kbhit() has been invoked to test that there
 * are characters in the typeahead buffer before getch() is invoked to
 * get the answer.
 */

#define __NEW_STARLET 1

#include <signal.h>
#include <string.h>
#include <file.h>
#include <ctype.h>
#include <stdio.h>

/*	  
**  VMS Private Macros
*/	  
#include <descrip.h>
#include <DEVDEF.H>
#include <INTS.H>
#include <iodef.h>
#include <IOSBDEF.H>
#include <ttdef.h>
#include <tt2def.h>
#include <dcdef.h>
#include <climsgdef.h>
#include <rms.h>
#include <hlpdef.h>
#include <LIB$ROUTINES.H>
#include <starlet.h>

#define MAX_CMDSIZ	256  /*  Maximum command size */
#define MAX_FILENM	255 /* Mamimum file name size */

#define FDL$M_FDL_STRING    2		/* Use string for fdl text */
#define FDLSIZE		    4096	/* Maximum possible file size */

#undef getch

int getch();
void ttynorm();

#ifdef _USEDCL_

/*
 * Declare some external procedure prototypes (saves me confusion!)
 */
extern int lib$get_input(
	    struct dsc$descriptor *resultant,
	    struct dsc$descriptor *prompt, 
	    unsigned short *resultant_length);
extern int lib$put_output(
	    struct dsc$descriptor *output);
extern int lib$sig_to_ret();
/*	  
**  The CLI routines are documented in the system routines manual.
*/	  
extern int cli$dcl_parse(
	    struct dsc$descriptor *command,
	    char cmd_table[],
	    int (*get_command)(
		struct dsc$descriptor *resultant,
		struct dsc$descriptor *prompt, 
		unsigned short *resultant_length),
	    int (*get_parameter)(
		struct dsc$descriptor *resultant,
		struct dsc$descriptor *prompt, 
		unsigned short *resultant_length),
	    struct dsc$descriptor *prompt);
extern int cli$present( struct dsc$descriptor *object);
extern int cli$_get_value(
	    struct dsc$descriptor *object,
	    struct dsc$decsriptor *value,
	    unsigned short *value_len);
/*
 * Static Data
 */
static $DESCRIPTOR (cmdprmt_d, "DROPSAFE> ");  /*  Prompt string */

#endif /* _USEDCL_ */

static unsigned short _kbhitChan_ = 0;

static struct IOSB {
	unsigned short sts;
	unsigned short byteCount;
	unsigned short terminator;
	unsigned short terminatorSize;
	} iosb;

static $DESCRIPTOR (kbdev_desc, "SYS$COMMAND:");

static struct {
	char Class;
	char Type;
	unsigned short BufferSize;
	unsigned int Mode;
	int ExtChar;
  } CharBuf, OldCharBuf;

static $DESCRIPTOR (out_file_descr, "SYS$DISK:[]"); /* Default Output
						       File Descr */

static int flags = FDL$M_FDL_STRING;

/*
 * **-kbhit_handler-This exit handler restores the terminal characteristics
 *
 * Description:
 *
 * This procedure is invoked to return the the terminal to normality (depends
 * on what you think is normal!). Anyway, it gets called to restore
 * characteristics either through ttynorm or via an exit handler.
 */
static void kbhit_handler(int *sts)
{
  ttynorm();
  (void) sys$dassgn (
	  _kbhitChan_);
  _kbhitChan_ = 0;
}

/*
 * Data Structures For Linking Up Exit Handler 
 */
static unsigned int exsts;

static struct {
	int link;
	void *rtn;
	int argcnt;
	unsigned int *stsaddr;
   } exhblk = { 0, &(kbhit_handler), 1, &(exsts)};
/*
 * **-kbhit_Getchn-Get Channel
 *
 * Functional Description:
 *
 * Private routine to get a terminal channel and save the terminal
 * characteristics.
 *
 * Arguments:
 *
 *  None.
 *
 * Returns:
 *
 *  If 0, channel already assigned. If odd, then assign was successful
 * otherwise returns VMS error status.
 *
 * Implicit Inputs:
 *
 * _kbhitChan_	Channel assigned to the terminal (if any).
 *
 * Implicit Outputs:
 *
 *  OldCharBuf	Initial terminal characteristics.
 *  _kbhitChan_	Channel assigned to the terminal.
 *
 * Side Effects:
 *
 *  Establishes an exit handler to restore characteristics and deassign
 * terminal channel.
 */
/*
 * **-kbhit_Getchn-Get Channel
 *
 * Functional Description:
 *
 * Private routine to get a terminal channel and save the terminal
 * characteristics.
 *
 * Arguments:
 *
 *  None.
 *
 * Returns:
 *
 *  If 0, channel already assigned. If odd, then assign was successful
 * otherwise returns VMS error status.
 *
 * Implicit Inputs:
 *
 * _kbhitChan_	Channel assigned to the terminal (if any).
 *
 * Implicit Outputs:
 *
 *  OldCharBuf	Initial terminal characteristics.
 *  _kbhitChan_	Channel assigned to the terminal.
 *
 * Side Effects:
 *
 *  Establishes an exit handler to restore characteristics and deassign
 * terminal channel.
 */
static int kbhit_Getchn()
{
    int sts = 0;

    if (_kbhitChan_ == 0) {
        if ((sts = sys$assign (
                &kbdev_desc,
                &_kbhitChan_,
                0,
                0)) & 1) {
            if ((sts = sys$qiow (
                    0,
                    _kbhitChan_,
                    IO$_SENSEMODE,
                    (struct _iosb *)&iosb,
                    0,
                    0,
                    &OldCharBuf,
                    12,
                    0,
                    0,
                    0,
                    0)) & 01) sts = iosb.sts;
            if (sts & 01) {
                if (!(OldCharBuf.Class & DC$_TERM)) {
                    fprintf(stderr,"\nNot running on a terminal");
                }
                (void)sys$dclexh(&exhblk);
            }
        }
        else 
        {
            printf("failed sys$assign: %d\n", sts);
        }
    }
    return(sts);
}
/*	  
 * **-ttynorm-Restore initial terminal characteristics
 *
 * Functional Description:
 *
 * This procedure is invoked to restore the initial terminal characteristics.
 */
void ttynorm();
/*
 * **-kbhit-Find out if a key has been pressed
 *
 * Description:
 *
 * Make the terminal noecho and sense the characters coming in by looking at
 * the typeahead count. Note that the character remains in the typeahead buffer
 * untill either read, or that the user types a Control-X when not in 'passall'
 * mode.
 */
int kbhit();

static int64 NoTerm = 0;  /*  TT Terminator Mask (Nothing) */

/*
 * **-getch-Get a character and return it
 *
 * Description:
 *
 * Get a character from the keyboard and return it. Unlike Unix, the character
 * will be explicitly echoed unless ttycbreak() has been called first. If the
 * character is in the typeahead, that will be read first.
 */
int vms_getch();
/*
 * **-putch-Put Character To 'Console' Device
 *
 * This procedure is a companion to getch, outputing a character to the
 * terminal with a minimum of fuss (no VAXCRTLK, no RMS!). This routine
 * simply gets a channel (if there isn't one already and uses QIO to
 * output.
 *
 */
int putch(int chr);
/*
 * **-ttycbreak-Set Unix-like Cbreak mode
 *
 * Functional Description:
 *
 * This code must be invoked to produce the Unix-like cbreak operation which
 * disables echo, allows control character input.
 */
void ttycbreak ();

#endif