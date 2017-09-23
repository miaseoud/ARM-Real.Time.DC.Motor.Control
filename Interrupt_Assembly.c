#include "REG.h"
#include "DIO.h"
#include "UART0.h"
#include  "Interrupt_Assembly.h"
void EnableInterrupts (void){
    __asm(" CPSIE I");
    __asm(" BX LR");
    //GPIO_PORTF_DATA_R=1;
   // UART0_SendChr(65);
}
