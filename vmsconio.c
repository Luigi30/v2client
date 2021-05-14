
/*
 * This code defines an equivalent version of kbhit() and getch() for
 * use under VAX/VMS, together with an exit handler to reset terminal
 * characteristics.
 *
 * This code assumes that kbhit() has been invoked to test that there
 * are characters in the typeahead buffers before getch() is invoked to
 * get the answer.
 */

#include "vmsconio.h"

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

/*	  
 * **-ttynorm-Restore initial terminal characteristics
 *
 * Functional Description:
 *
 * This procedure is invoked to restore the initial terminal characteristics.
 */
void ttynorm()
/*
 * Arguments:
 *
 *  None.
 *
 * Implicit Inputs:
 *
 *  OldCharBuf	Initial terminal characteristics.
 *  _kbhitChan_	Channel assigned to the terminal.
 *
 * Implicit Outputs:
 *
 *  None.
 */	  
{
  int sts;

  if (_kbhitChan_ != 0) {
      CharBuf.Mode = OldCharBuf.Mode;
      CharBuf.ExtChar = OldCharBuf.ExtChar;
    /*
      CharBuf.Mode &= ~TT$M_NOECHO;
      CharBuf.ExtChar &= ~TT2$M_PASTHRU;
    */
      if ((sts = sys$qiow (
			       0,
			       _kbhitChan_,
			       IO$_SETMODE,
			       (struct _iosb *)&iosb,
			       0,
			       0,
			       &OldCharBuf,
			       12,
			       0,
			       0,
			       0,
			       0)) & 01) sts = iosb.sts;
      if (!(sts & 01)) {
	    fprintf(stderr,"\nFailed to reset terminal characteristics!");
	    (void) lib$signal(sts);
      }
   }
   return;
}
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
int kbhit()
/*
 * Arguments:
 *
 *  None.
 *
 * Returns:
 *
 *  TRUE  if there is a character in the typeahead buffer.
 *  FALSE if there is no character in the typeahead buffer.
 */


{
  int sts;

  struct {
	unsigned short TypAhdCnt;
	char FirstChar;
	char Reserved[5];
  } TypCharBuf;

    if (((sts = kbhit_Getchn()) & 01) || sts == 0) {

        /*
        **  Get typeahead count
        */
        if ((sts = sys$qiow (
                    0,
                    _kbhitChan_,
                    IO$_SENSEMODE | IO$M_TYPEAHDCNT,
                    (struct _iosb *)&iosb,
                    0,
                    0,
                    &TypCharBuf,
                    8,
                    0,
                    0,
                    0,
                    0)) & 01) sts = iosb.sts;
        if (sts & 01) return(TypCharBuf.TypAhdCnt>0);
        (void) lib$signal(sts);
    }
}

/*
 * **-getch-Get a character and return it
 *
 * Description:
 *
 * Get a character from the keyboard and return it. Unlike Unix, the character
 * will be explicitly echoed unless ttycbreak() has been called first. If the
 * character is in the typeahead, that will be read first.
 */
int vms_getch()
/*
 * Arguments:
 *
 *  None.
 *
 * Returns:
 *
 *  Character Read.
 */
{
  unsigned int sts;
  char CharBuf;

  if (((sts = kbhit_Getchn()) & 01) || sts == 0) {
      if ((sts = sys$qiow (
			      0,
			      _kbhitChan_,
			      IO$_READVBLK,
			      (struct _iosb *)&iosb,
			      0,
			      0,
			      &CharBuf,
			      1,
			      0,
			      NoTerm,
			      0,
			      0)) & 01) sts = iosb.sts;
  }
  if (sts & 01) return ((int) CharBuf);
  fprintf(stderr,"\nFailed to get character");
  (void) lib$signal(sts);
}
/*
 * **-putch-Put Character To 'Console' Device
 *
 * This procedure is a companion to getch, outputing a character to the
 * terminal with a minimum of fuss (no VAXCRTLK, no RMS!). This routine
 * simply gets a channel (if there isn't one already and uses QIO to
 * output.
 *
 */
int putch(int chr)
/*
 * Arguments:
 *  chr		Character to output.
 *
 * Returns:
 *
 *  Status return from Getchn and qio.
 *
 * Side Effects
 *
 * May assign a channel to the terminal.
 */
{
  unsigned int sts;

  if (((sts = kbhit_Getchn()) & 01) || sts == 0) {
      if ((sts = sys$qiow (
			      0,
			      _kbhitChan_,
			      IO$_WRITEVBLK,
			      (struct _iosb *)&iosb,
			      0,
			      0,
			      &chr,
			      1,
			      0,
			      0,
			      0,
			      0)) & 01) sts = iosb.sts;
  }
  if (sts & 01) return (sts);
  fprintf(stderr,"\nFailed to put character");
  (void) lib$signal(sts);
}
/*
 * **-ttycbreak-Set Unix-like Cbreak mode
 *
 * Functional Description:
 *
 * This code must be invoked to produce the Unix-like cbreak operation which
 * disables echo, allows control character input.
 */
void ttycbreak ()
/*
 * Arguments:
 *
 *  None.
 *
 * Returns:
 *
 *  None.
 *
 * Side Effects
 *
 * May assign a channel to the terminal.
 */
{
    struct {
	unsigned short TypAhdCnt;
	char FirstChar;
	char Reserved[5];
    } TypCharBuf;
    char buf[80];
    int sts;

    if (((sts = kbhit_Getchn()) & 01) || sts == 0) {
/*
 * Flush any typeahead before we change characteristics
 */
	if ((sts = sys$qiow (
			       0,
			       _kbhitChan_,
			       IO$_SENSEMODE | IO$M_TYPEAHDCNT,
			       (struct _iosb *)&iosb,
			       0,
			       0,
			       &TypCharBuf,
			       8,
			       0,
			       0,
			       0,
			       0)) & 01) sts = iosb.sts;
	if (sts) {
	    if (TypCharBuf.TypAhdCnt>0) {
		if ((sts = sys$qiow (
			    0,
			   _kbhitChan_,
			   IO$_READVBLK | IO$M_NOECHO | IO$M_TIMED,
			   (struct _iosb *)&iosb,
			   0,
			   0,
			   &buf,
			   (TypCharBuf.TypAhdCnt >= 80 ? 80 :
			    TypCharBuf.TypAhdCnt),
			   1,
			   NoTerm,
			   0,
			   0)) & 01) sts = iosb.sts;
			   
		if (sts)
		    TypCharBuf.TypAhdCnt -= iosb.byteCount;
	    }
	}
	if (!(sts & 01)) TypCharBuf.TypAhdCnt = 0;
/*
 * Modify characteristics
 */
	CharBuf = OldCharBuf;
	CharBuf.Mode = (OldCharBuf.Mode | TT$M_NOECHO) & ~TT$M_NOTYPEAHD;
	CharBuf.ExtChar = OldCharBuf.ExtChar | TT2$M_PASTHRU;
	if ((sts = sys$qiow (
		       0,
		       _kbhitChan_,
		       IO$_SETMODE,
		       (struct _iosb *)&iosb,
		       0,
		       0,
		       &CharBuf,
		       12,
	    	       0,
		       0,
		       0,
		       0)) & 01) sts = iosb.sts;
	if (!(sts & 01)) {
	  fprintf(stderr,
		  "\nttybreak()- Failed to set terminal characteristics!");
	  (void) lib$signal(sts);
	}
    }
}
