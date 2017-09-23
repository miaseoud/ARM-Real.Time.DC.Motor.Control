/******************************************************************************************************/
/*------------------- DC Motor Real-Time Speed Control using UART and Exnternal Signals --------------*/
/****************************************************************************************************/
/* Code developed for Tiva C series TM4C123G Launchpad/Developpment Board on Code Composer Studio
 * Pin1 in Port F (PF1) is connected to transistor base or H-Bridge (such ULN293) motor driver input.
 * The input speed is fed through UART0 or Increased/Decreased using switches connected to PF0 & PF4
 * The speed varies by changing the duty cycle of a periodic square wave at PF1 with period of 1ms
 * Each time the Systick timer overflows, its ISR handler is called to change wave polarity and load associated value.
 * When the switches PF0 or PF4 are pressed, the PortF ISR is executed to increase or decrease speed
 */
/*-------------------------------------------------  HEADERS ---------------------------------------*/
#include "PLL.h"
#include "UART0.h"
#include "REG.H"
#include "DIO.h"
#include "Interrupt_Assembly.h"
/*------------------------------------------ GLobal Variables & Functions Declarations ----------------*/
unsigned long High_Time,Low_Time;
char speed_index=4;
/****************************************************************************************************/
/*-------------------------------- Interrupt Service Routines Functions Declarations -----------------*/
/****************************************************************************************************/
extern void SysTick_Handler (void);
extern void GPIOPortF_Handler (void);
/*---------------------------------------- Initialization Functions Definition s------------------------*/
void SwitchesInterrupt_Init(void);
void SysTick_Init (void);
/****************************************************************************************************/
/*---------------------------------------------Systick Timer ISR------------------------------------*/
/****************************************************************************************************/
void SysTick_Handler (void){
    unsigned long High_Time,Low_Time;
    Low_Time=8000+speed_index*8000; //Update High & Low time based on speed_index from UART or Switches PF0 & PF4
    High_Time = 80000-Low_Time; // constant period of 1ms, variable duty cycle
    /*SysTick interrupt handler is not required to clear the SysTick*/
    // Toggle PF1
    if(GPIO_PORTF_DATA_R&0x02){
      DIO_PortWrite(PortF,Pin1,DIO_LOW);//  PF1 low
      NVIC_ST_RELOAD_R = Low_Time-1;     // reload value for low phase
    } else{
     DIO_PortWrite(PortF,Pin1,DIO_HIGH);//  PF1 high
      NVIC_ST_RELOAD_R = High_Time-1;     // reload value for high phase
    }
}
/****************************************************************************************************/
/*--------------------------------------------PORTF ISR for Switches--------------------------------*/
/****************************************************************************************************/
void GPIOPortF_Handler(void){ // called on portF interrupts

  if(GPIO_PORTF_RIS_R&0x01){  // PortF Raw Interrupt Status check for  SW2
    GPIO_PORTF_ICR_R = 0x01;  // GPIO Interrupt Clear: to clear the corresponding bit in the GPIORIS for For edge-triigered...
   if (speed_index<8)         //check if within limits
       speed_index++;         // slow down
  }
  if(GPIO_PORTF_RIS_R&0x10){   // PortF Raw Interrupt Status check for  SW1
    GPIO_PORTF_ICR_R = 0x10;   // // GPIO Interrupt Clear: to clear the corresponding bit in the GPIORIS for For edge-triigered...
    if (speed_index>0)         //check if within limits
    speed_index--;             // speed up
  }
  UART0_SendChr(speed_index+'0');
  UART0_Println();
}
/******************************************************************************************************/
/*-----------------------------------------------Main-------------------------------------------------*/
/*******************************************************************************************************/
int main(void){
    PLL_Init();
    UART0_Init(9600,80000000); //UART baud-rate 9600 for running frequency 80MHz
    //Port Initialization,clock enabling, alternative and analog functions disable,...
    DIO_PortInit      (PortF , Pin0|Pin1|Pin2|Pin3|Pin4 , Pin0|Pin4);
    //Set Pins on Port F directions
    DIO_PortDirection (PortF , Pin1|Pin2|Pin3 , DIO_OUTPUT);
    DIO_PortDirection (PortF , Pin0|Pin4 , DIO_INPUT);
    SysTick_Init ();
    SwitchesInterrupt_Init();
    EnableInterrupts();
/******************************************************************************************************/
/*--------------------------------------------Super Loop----------------------------------------------*/
/****************************************************************************************************/
    while (1){
        //Blocking function waiting for UART receive flag, the push buttons are extenral interrupts.
        speed_index=UART0_ReadChr()-'0';//Blocking function waiting for UART receive flag.
        UART0_SendChr(speed_index); //Confirm receiving character by sending it back.
        UART0_Println();
    }
}
/******************************************************************************************************/
/*--------------------------------------------Systick Timer Initialization---------------------------*/
/****************************************************************************************************/
void SysTick_Init (void){
     /*SysTick Current Value Register (NVIC_ST_CTRL_R)
     1) Disable Systick
     2)Clock Source=System clock(80MHz)
     3)Interrupt Enable*/
     NVIC_ST_CTRL_R =0x0006;
     NVIC_ST_CURRENT_R = 0;        // any write to clear "current" in SysTick Current Value Register
     NVIC_ST_RELOAD_R = Low_Time-1;       //Initial Reload Value to SysTick Reload Value Register
     NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // Set priority 2
     NVIC_ST_CTRL_R =0x0007;      //Enable SystickTimer
}

void SwitchesInterrupt_Init(void){
  GPIO_PORTF_IS_R &= ~0x11;     //  PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 one edge-only sensitive
  GPIO_PORTF_IEV_R &= ~0x11;    //     PF4,PF0 falling edge event
  GPIO_PORTF_ICR_R = 0x11;      //  clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // enable interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF00FFFF)|0x00400000; // (g) priority 2
  NVIC_EN0_R = 0x40000000;      //  enable interrupt 30 in NVIC
}
/********************************************************************************************************************/
