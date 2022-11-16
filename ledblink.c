/* 
 * File:   newmain.c
 * Author: vinnieg
 *
 * Created on October 10, 2022, 1:21 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

/*
 * 
 */
int main(int argc, char** argv)
{
    uint32_t *led_ptr;
    
    // disable watchdog timer
//    WDT_REGS->WDT_MR

//    led_ptr = (uint32_t *)0x400E1470;
////    0x400E1470
//    *led_ptr = 0x8;

    // Is this not necessary?
//    PIOA_REGS->PIO_WPMR &= ~0x1;    // clear bit 0 of write protection (allows write to pio en)

//    PIOA_REGS->PIO_PER |= PIN_PA3;  // enable desired pin
//    PIOA_REGS->PIO_OER |= PIN_PA3;  // output enable desired pin
//    PIOA_REGS->PIO_OWER |= PIN_PA3; // output write enable desired pin
    
//    PIOA_REGS->PIO_PER |= PIO_PA3;  // enable desired pin
//    PIOA_REGS->PIO_OER |= PIO_PA3;  // output enable desired pin
//    PIOA_REGS->PIO_OWER |= PIO_PA3; // output write enable desired pin
    
    
    PIOA_REGS->PIO_PER |= PIO_PA22;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA22;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA22; // output write enable desired pin
    
//    PIOA_REGS->PIO_OWDR |= ~0x0; // output write disable desired pin


    
    for (;;)
    {
//        PIOA_REGS->PIO_ODSR ^= PIN_PA3;
        PIOA_REGS->PIO_ODSR ^= PIO_PA22;


        for (volatile uint32_t i = 0; i < 200000; i++);
//        *led_ptr ^= 0x8;
    }
    
    return (EXIT_SUCCESS);
}
