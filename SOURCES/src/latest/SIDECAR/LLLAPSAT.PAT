;
; This patch is to be applied to the LANSAT.SYS program in
; LANLink Laptop Release 2.2 for compatibility with PC-MOS Release 5.01
;
; To apply this patch, follow these steps:
;
; 1. Change into the directory that contains the file LANSAT.SYS.
;
; 2. Copy the file LLLAPSAT.PAT into that subdirectory.
;
; 3. Enter the following command at the system prompt:
;
;         DEBUG LANSAT.SYS < LLLAPSAT.PAT
;
;    (Make sure that the location of DEBUG.COM is in your
;     path statement, or the DEBUG program won't run.)
;
; This command will automatically load DEBUG, apply the following
; patch for you, and exit DEBUG.
;
;

E CS:01DC E9 BC 12 90 90
E CS:149B 81 3E 84 1F 01 05
E CS:14A1 72 03
E CS:14A3 E9 4b ED
E CS:14A6 80 3E 7F 1F 59
E CS:14AB E9 33 ED

W
Q
                                                                                                     