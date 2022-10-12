/*
;,fs
;******** $.
;,fe
=====================================================================
               (c) copyright 1992, the software link inc.
                       all rights reserved

 module name:        ulrdlbl.c
 creation date:      12/15/92
 revision date:
 author:             mjs
 description:        ulib module

======================================================================

mjs 12/15/92	created this module

=======================================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#ifdef __BORLANDC__
#include <dir.h>
#endif
#include <string.h>

#include "asmtypes.h"
#include "ulib.h"

/*======================================================================
;,fs
; void ul_read_dsklbl(byte drvnum, byte *lbuf)
;
; in:	drvnum = drive number (1 for 'A', 2 for 'B', etc.)
;	lbuf -> buffer that is at least 12 chars in size
;
; out:
;
;,fe
========================================================================*/
void ul_read_dsklbl(byte drvnum, byte *lbuf) {

#ifdef __BORLANDC__
  struct ffblk ffblk;			// structure for findfirst/next
#else
  struct find_t ffblk;
#endif
  word err_stat;			// holds error status
  byte first;
  byte search_spec[8];

  sprintf(search_spec,"%c:\\*.*",drvnum+'A'-1);
  *lbuf = 0;
  first = 1;
  while(1) {
    if(first) {
#ifdef __BORLANDC__
      err_stat = findfirst(search_spec,&ffblk,8);
#else
      err_stat = _dos_findfirst(search_spec,8,&ffblk);
#endif
      first = 0;
      }     else {
#ifdef __BORLANDC__
      err_stat = findnext(&ffblk);
#else
      err_stat = _dos_findnext(&ffblk);
#endif
      }
    if(err_stat) {
      return;
      }
#ifdef __BORLANDC__
    if(ffblk.ff_attrib & 8) {
      if(strlen(ffblk.ff_name) > 8) {
        strncpy(lbuf,ffblk.ff_name,8);
        strcpy(&lbuf[8],&ffblk.ff_name[9]);
        }       else {
        strcpy(lbuf,ffblk.ff_name);
#else
    if(ffblk.attrib & 8) {
      if(strlen(ffblk.name) > 8) {
        strncpy(lbuf,ffblk.name,8);
        strcpy(&lbuf[8],&ffblk.name[9]);
        }       else {
        strcpy(lbuf,ffblk.name);
#endif
        }
      return;
      }
    }
  }

