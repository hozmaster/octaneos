//
// Berlin Brown
//
// hexdump.c
//
// functions defined in system.h
//
// $Id: hexdump.c,v 1.2 2005/05/26 00:06:53 bigbinc Exp $
//

#include <system/system.h>
#include <system/alpha.h>
#include <system/beta.h>


// [ see... system.h - use 8 bytes per row ]
void public_hexdump(void *_addr, int _count) {

  // 16 bytes = 00
  char buf[80];
  
  int i = 0;
  int j = 0;
  int _row_ctr_max = 0;
  int _row_start = 0;
  int _row_ctr = 0;
  char _c = 0;
  unsigned char *__mem;
  
  __mem = _addr;
  
  __sprintf(buf, " ------- HexDump starting at: %x ----------------------\n", __mem);
  __puts(buf);

  _row_ctr_max = _count / 8;

  for (j = 0; j < _row_ctr_max; j++) {
    
    __sprintf(buf, "[%x+%d]::", __mem, _row_ctr);
    __puts(buf);
    
    for (i = 0; i < 8; i++) {
      
      __sprintf(buf, " 0x%x", __mem[(j * 8) + i]);
      __puts(buf);
      
    }
    
    __sprintf(buf, " ** ");	      
    __puts(buf);    
    
    for (i = 0; i < 8; i++) {

      _c = __mem[(j * 8) + i];
      
      //  -- convert to ascii 
      if (((char)(_c >= 32) && (char)(_c <= 126))) {
	
	__sprintf(buf, "%c", _c); 
	__puts(buf);    
	
      } else {
	
	__sprintf(buf, ".", _c);
	__puts(buf);    
	
      }
      
    }
    
    __sprintf(buf, "\n");	      
    __puts(buf);    
    
    _row_ctr += 8;
    
  }
  
  __sprintf(buf, " -------  [ End HexDump ] ----------------------------\n");
  __puts(buf);

}
