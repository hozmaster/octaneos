/*
 * Berlin Brown
 *
 * bigbinc@hotmail.com
 *
 * $Id: demo.S,v 1.1 2004/03/14 03:23:59 bigbinc Exp $
 */


.code32
.globl begtext, begdata, begbss, endtext, enddata, endbss

.text
begtext:
.data
begdata:
.bss
begbss:
.text

hello:	jmp hello

.text
endtext:
.data
enddata:
.bss
endbss:
