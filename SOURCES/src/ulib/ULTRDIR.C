/*
;,fs
;******** $.
;,fe
=====================================================================
               (c) copyright 1992, the software link inc.
                       all rights reserved

 module name:
 creation date:      12/10/92
 revision date:
 author:             mjs
 description:        ulib module

======================================================================

mjs 12/10/92	created this module

=======================================================================
*/

#include <stdlib.h>
#include <dos.h>
#ifdef __BORLANDC__
#include <dir.h>
#endif
#include <string.h>

#include "asmtypes.h"
#include "ulib.h"

/*======================================================================
;,fs
; word ul_trace_dir(byte *dpbuf, fspc_type *fsptr)
;
; this function is the effectively the same as ul_trace_dirl() except
; that no list is built of the filenames found.  this function is for
; the case where the activity of the work function includes the building
; of a list -- such that using ul_trace_dirl() would involve a
; waste of time and memory.
;
; see ul_trace_dirl() for other usage notes.
;
;,fe
========================================================================*/
word ul_trace_dir(byte *dpbuf, fspc_type *fsptr) {

  byte *orig_end;			// ptr to original end of dpbuf
  byte *trunc_ptr;			// used to maintain wbuf
#ifdef __BORLANDC__
  struct ffblk ffblk;			// structure for findfirst/next
#else
  struct find_t ffblk;			// structure for findfirst/next
#endif
  word err_stat;			// holds error status


  // record the original ending point of the string in the
  // drive/path buffer.  then make sure it ends with a backslash
  // (unless it's a null string -- for the current directory).

  orig_end = (byte *)strchr(dpbuf,0);
  trunc_ptr = orig_end;
  if(orig_end != dpbuf) {
    if(*(trunc_ptr-1) != '\\') {
      *trunc_ptr = '\\';
      trunc_ptr++;
      *trunc_ptr = 0;
      }
    }
  strcat(dpbuf,(fsptr->search_spec));

  // find each target file and call the work function

  while(1) {
    if(trunc_ptr != NULL) {
#ifdef __BORLANDC__
      err_stat = findfirst(dpbuf,&ffblk,fsptr->search_attr);
#else
      err_stat = _dos_findfirst(dpbuf,fsptr->search_attr,&ffblk);
#endif
      *trunc_ptr = 0;
      trunc_ptr = NULL;
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
        break;
        }
      else {
        *orig_end = 0;
        return(2);
        }
      }

    // for each file found, call the work function with a
    // pointer to dpbuf, the found name and its attribute.

#ifdef __BORLANDC__
    if((*(fsptr->work_func))(dpbuf,ffblk.ff_name,ffblk.ff_attrib) != 0) {
#else
    if((*(fsptr->work_func))(dpbuf,ffblk.name,ffblk.attrib) != 0) {
#endif
      *orig_end = 0;
      return(4);
      }
    }
  *orig_end = 0;
  return(0);
  }
