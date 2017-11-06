# DC Motor Real-Time Speed Control using UART and Exnternal Signals
Code developed for Tiva C series TM4C123G Launchpad/Developpment Board on Code Composer Studio
 * Pin1 in Port F (PF1) is connected to motor driver such as H-Bridge (i.e. ULN293) input.
 * The input speed is fed through UART0 or Increased/Decreased using switches connected to PF0/PF4 respectively.
 * The speed varies by changing the duty cycle of a periodic square wave at PF1 with period of 1ms.
 * Each time the Systick timer overflows, its ISR handler is called to change wave polarity and load associated value.
 * When the switches PF0 or PF4 are pressed, the PortF ISR is executed to increase or decrease speed
 
