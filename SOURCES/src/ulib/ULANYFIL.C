/*
;,fs
;******** $.
;,fe
=====================================================================
               (c) copyright 1992, the software link inc.
                       all rights reserved

 module name:        ulanyfil.c
 creation date:      04/01/92
 revision date:
 author:             mjs
 description:        ulib module

======================================================================

mjs 04/01/92	created this module

=======================================================================
*/

#include <stdlib.h>
#include <dos.h>
#ifdef __BORLANDC__
#include <dir.h>
#endif
#include <io.h>

#include "asmtypes.h"
#include "ulib.h"

/*======================================================================
;,fs
; byte ul_any_files(byte *filespec, byte search_attr)
;
; in:	filespec -> file specification string (e.g. "c:\\xyz\\abc*.*")
;	search_attr = attribute, using FA_NORMAL, FA_RDONLY, etc.
;
; out:	retval = 0 if matching files exist
;	retval = 1 if no matching files exist
;	retval = 2 if an error was returned from findfirst()
;
;,fe
========================================================================*/
byte ul_any_files(byte *filespec, byte search_attr) {

  word err_stat;			// holds error status

#ifdef __BORLANDC__
  struct ffblk ffblk;			// for findfirst/next
  err_stat = findfirst(filespec,&ffblk,search_attr);
#else
  struct find_t ffblk;
  err_stat = _dos_findfirst(filespec,search_attr,&ffblk);
#endif
  if(err_stat != 0) {
    if(_doserrno == 0x12) {
      return(1);
      }
    else {
      return(2);
      }
    }
  return(0);
  }

