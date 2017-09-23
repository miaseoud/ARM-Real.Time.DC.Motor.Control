/*************************************************************************************************************************/
/*------------------------------------------------ Library for driving ARM GPIOs ----------------------------------------*/
/*************************************************************************************************************************/
/* - Library is used for:
*  - Determining Port/Pin direction.
*  - Setting/Clearing Port/Pin.
*  - Reading Port/Pin
*  Ex: DIO_PortWrite(PortB, Pin0 | Pin2 | Pin3, DIO_LOW);
*/
#ifndef DIO_H
#define DIO_H
typedef unsigned long uint32;
typedef unsigned long* puint32;

/*************************************************************************************************************************/
/*-------------------------------------------------- Macros for functions ------------------------------------------------*/
/*************************************************************************************************************************/
#define SET_BIT(r,b) ((r) |=  (1   << (b))) //set bit
#define CLEAR_BIT(r,b) ((r) &= (~(1) << (b))) //clear bit
#define TOGGLE_BIT(r,b) ((r) ^=  (1   << (b))) //toggle bit

#define IS_TRUE(r,b)        (( (r) & (1<<(b))) != 0 ) //is bit true (1)

#define SET_REG(r,v,b) r &= ~(b); \
    r |= (v & b)  /*clear bits to change*/
//#define SET_REG(r,v,b) (((v))?  (SET_BIT(r,b)) : (CLEAR_BIT(r,b)))

#define GET_REG(r,b) r & b
/*************************************************************************************************************************/
#define DIO_INPUT 0
#define DIO_OUTPUT 0xFF
#define DIO_LOW 0
#define DIO_HIGH 0xFF

#define PortA	0x00
#define PortB	0x01
#define PortC	0x02
#define PortD	0x03
#define PortE	0x04
#define PortF	0x05

#define Pin0    0x01
#define Pin1    0x02
#define Pin2    0x04
#define Pin3    0x08
#define Pin4    0x10
#define Pin5    0x20
#define Pin6    0x40
#define Pin7    0x80

#define RED     0x02
#define BLUE    0x04
#define GREEN   0x08
#define YELLOW  0x0A
#define CYAN    0x0C
#define WHITE   0x0E
#define PINK    0x06

#define DIO_TRUE 1
#define DIO_FALSE 0
/*************************************************************************************************************************/
/*------------------------------------------------ Functions declarations -----------------------------------------------*/
/*************************************************************************************************************************/
void DIO_PortInit(uint32 , uint32 , uint32 );
void DIO_PortRead(uint32 , uint32,puint32 );
void DIO_PortWrite(uint32, uint32, uint32);
void DIO_PortDirection(uint32 , uint32, uint32);
#endif


