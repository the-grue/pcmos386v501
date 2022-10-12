/*
;,fs
;******** $.
;,fe
=====================================================================
               (c) copyright 1992, the software link inc.
                       all rights reserved

 module name:        ulremfil.c
 creation date:      04/01/92
 revision date:
 author:             mjs
 description:        ulib module

======================================================================

mjs 04/01/92	created this module
jts 06/30/18	added code to allow build under bcc or tcc

=======================================================================
*/

#include <stdlib.h>
#include <dos.h>
#ifdef __BORLANDC__
#include <dir.h>
#else
#include <stdio.h>
#include "ulwathlp.h"
#endif
#include <io.h>
#include <string.h>

#include "asmtypes.h"
#include "ulib.h"

#ifndef __BORLANDC__
#ifndef FA_NORMAL
#define FA_NORMAL 0x00			// normal file type
#endif
#endif
/*======================================================================
;,fs
; byte ul_remove_files(byte *filespec, byte search_attr)
;
; in:	filespec -> file specification string (e.g. "c:\\xyz\\abc*.*")
;	search_attr = attribute, using FA_NORMAL, FA_RDONLY, etc.
;
; out:	retval = 0 if successful
;	retval = 1 if an error occured
;
;,fe
========================================================================*/
byte ul_remove_files(byte *filespec, byte search_attr) {

  word first;				// controls findfirst/next calls
  word err_stat;			// holds error status
#ifdef __BORLANDC__
  struct ffblk ffblk;			// for findfirst/next
#else
  struct find_t ffblk;			// for findfirst/next
#endif
  byte drvstr[MAXDRIVE];		// for fnsplit
  byte pathstr[MAXDIR];			// for fnsplit
  byte fnamestr[MAXFILE];		// for fnsplit
  byte extstr[MAXEXT];			// for fnsplit
  byte wbuf[MAXPATH];			// holds d:\path filespec portion
  byte *trunc_ptr;			// used to maintain wbuf
  word attr;				// each file's attribute

#ifdef __BORLANDC__
  fnsplit(filespec,drvstr,pathstr,fnamestr,extstr);
#else
  _splitpath(filespec,drvstr,pathstr,fnamestr,extstr);
#endif
  strcpy(wbuf,drvstr);
  strcat(wbuf,pathstr);
  trunc_ptr = strchr(wbuf,0);
  first = 1;
  while(1) {
    if(first) {
#ifdef __BORLANDC__
      err_stat = findfirst(filespec,&ffblk,search_attr);
#else
      err_stat = _dos_findfirst(filespec,search_attr,&ffblk);
#endif
      first = 0;
      }
    else {
#ifdef __BORLANDC__
      err_stat = findnext(&ffblk);
#else
      err_stat = _dos_findnext(&ffblk);
#endif
      }
    if(err_stat != 0) {
      if(_doserrno == 0x12) {
        return(0);
        }
      else {
        return(1);
        }
      }
#ifdef __BORLANDC__
    strcat(wbuf,ffblk.ff_name);
#else
    strcat(wbuf,ffblk.name);
#endif
    if(search_attr != FA_NORMAL) {
#ifdef __BORLANDC__
      attr = _chmod(wbuf,0);
#else
      if(_dos_getfileattr(wbuf, &attr) != 0) {
      	return(1);
      }
#endif
      if(attr == 0xffff) {
        return(1);
        }
      if(attr & search_attr) {
#ifdef __BORLANDC__
        attr = _chmod(wbuf,1,FA_NORMAL);
        if(attr == 0xffff) {
          return(1);
          }
#else
	attr = _dos_setfileattr(wbuf, FA_NORMAL);
	if(attr != 0) {
	  return(1);
	  }
#endif
        }
      }
    if(remove(wbuf) != 0) {
      return(1);
      }
    *trunc_ptr = 0;
    }
  }

