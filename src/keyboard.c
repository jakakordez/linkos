#include <stddef.h>
#include <stdint.h>
#include "io.h"
#include "keyboard.h"
#include "string.h"
#include "cli.h"

char keyboard[] = " ";
char oldkey;
char key = 0;
const char KeyMAPL[] = "  1234567890\'+  qwertzuiopss\n asdfghjklss s yxcvbnm,.-     ";
const char KeyMAPU[] = "  !\"#$%&/()=?*  QWERTZUIOPSS\n ASDFGHJKLSS S YXCVBNM;:_     ";
int ALT_DOWN = 0;
int SHIFT_DOWN = 0;

char Keyboard_Scan(){
  while(1){
    oldkey=key;
    key=inb(0x60);
    if(!(oldkey==key)){
      Key k = key;
      int r = RELEASED(k);
      k &= 0x7F;
      if(k == LShift || k == RShift) SHIFT_DOWN = !r;
      else if(k == Alt) ALT_DOWN = !r;
		  //if(key&0x80) cli_print(printf("r%d ", key&0x7F));
		  //else cli_print(printf("p%d ", key&0x7F));
      return key;
    }
  }
}
