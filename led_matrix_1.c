/* 
 * File:   newmain.c
 * Author: vinnieg
 *
 * Created on October 10, 2022, 3:39 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#define BIT0    0x0001  // 0001
#define BIT1    0x0002  // 0010
#define BIT2    0x0004  // 0100
#define BIT3    0x0008  // 1000
#define BLANK       10
#define ARROW_UP    11
#define ARROW_DOWN  12
#define ARROW_RIGHT 13
#define ARROW_LEFT  14

// Function prototypes
void inputRowAddress(uint8_t addr);
void enableLedControllers();
void disableLedControllers();
void latch();
void toggleLeds();
void delay(int number_of_seconds);
void clockPulse();
void clearLeds();
void fillRowWithZeros();
void clearDisplay();
void drawNum(uint8_t num, uint32_t i);
void display(uint16_t num, uint16_t direction);

#include <time.h>

uint32_t fontRows[15][32] = {   // 0
                                {0x0, 0x0, 0x1f0, 0x3f8, 0x7fc, 0xf1e, 0xe0e, 0x1e0f, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0x1c07, 0xe0e, 0xe0e, 0x7bc, 0x7fc, 0x3f8, 0x1f0, 0x0, 0x0},
                                
                                // 1
                                {0x0, 0x0, 0x1c0, 0x1c0, 0x1e0, 0x1f0, 0x1f8, 0x1fc, 0x1fe, 0x1df, 0x1cf, 0x1c3, 0x1c1, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x1c0, 0x0, 0x0},
                                
                                // 2
                                {0x0, 0x0, 0x3f0, 0x7f8, 0xffc, 0x1f1e, 0x1e0e,
                                0x1c0f, 0x1c07, 0x1c07, 0x1c06, 0x1c00, 0x1c00,
                                0x1e00, 0xe00,0xf00, 0x780, 0x380, 0x3c0, 0x1e0,
                                0xf0, 0x78, 0x38, 0x3c, 0x1e, 0xe, 0x1fff,
                                0x1fff, 0x1fff, 0x1fff, 0x0,0x0},
                                
                                // 3
                                {0x0000, 0x0000, 0x01F8, 0x03FC, 0x07FC, 0x07FE,
                                0x0E0F, 0x0E07, 0x0E07, 0x0E07, 0x0E00, 0x0E00,
                                0x0700, 0x07E0, 0x03E0, 0x07E0, // Top half
                                0x0FE0, 0x1E00, 0x1C00, 0x1C00, 0x1C00, 0x1C00,
                                0x1C07, 0x1C07, 0x1C07, 0x1E0F, 0x0F0E, 0x07FC,
                                0x03F8, 0x01F8, 0x0000, 0x0000},
                                
                                // 4
                                {0x0, 0x0, 0x700, 0x700, 0x700, 0x780, 0x780, 0x7c0, 0x7c0, 0x7e0, 0x7e0, 0x7f0, 0x770, 0x778, 0x738, 0x73c, 0x71c, 0x71e, 0x70e, 0x70f, 0x1fff, 0x1fff, 0x1fff, 0x700, 0x700, 0x700, 0x700, 0x700, 0x700, 0x700},
                                
                                // 5
                                {},
                                
                                // 6
                                {},
                                
                                // 7
                                {},
                                
                                // 8
                                {},
                                
                                // 9
                                {},
                                
                                // Blank (index 10))
                                {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                                0x0, 0x0, 0x0, 0x0, 0x0},
                                
                                // Arrow Up (Index 11)
                                {0x0, 0x0, 0x40, 0xe0, 0x1f0, 0x3f8, 0x7fc, 0xffe, 0x1fff, 0x1fff, 0x1fff, 0x1df7, 0x19f3, 0x11f1, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x0, 0x0},
                                
                                // Arrow Down (index 12)
                                {0x0, 0x0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x1f0, 0x11f1, 0x19f3, 0x1df7, 0x1fff, 0x1fff, 0x1fff, 0xffe, 0x7fc, 0x3f8, 0x1f0, 0xe0, 0x40, 0x0, 0x0},
                                
                                // Arrow Right (index 13)
                                {0x0, 0x0, 0x3f, 0x7f, 0x7e, 0xfe, 0xfc, 0x1fc, 0x1f8, 0x3f8, 0x3f0, 0x7f0, 0x7e0, 0xfe0, 0xfe0, 0x1fc0, 0xfc0, 0xfe0, 0x7e0, 0x7f0, 0x3f0, 0x3f8, 0x1f8, 0x1fc, 0xfc, 0xfc, 0x7e, 0x7e, 0x3f, 0x3f, 0x0, 0x0},
                                
                                // Arrow Left (index 14)
                                {0x0, 0x0, 0x1f80, 0xfc0, 0x7e0, 0x7e0, 0x3f0, 0x3f0, 0x1f8, 0x1f8, 0xfc, 0xfc, 0x7c, 0x7e, 0x3e, 0x3f, 0x3e, 0x7e, 0x7e, 0xfc, 0xfc, 0x1f8, 0x1f8, 0x3f0, 0x3f0, 0x7e0, 0x7e0, 0xfc0, 0xfc0, 0x1f80, 0x0, 0x0}
                                };
                                
                                
/*
 * 
 */
int main(int argc, char** argv) {

//    uint32_t *led_ptr;
    volatile uint32_t i = 0;
//    volatile uint32_t time_delay;
    volatile uint16_t j;

    
    // Disable watchdog timer
    WDT_REGS->WDT_MR = WDT_MR_WDDIS(1);
    
    // The LED matrix have the following Pins
    // CLK, STB (driver latch), OE (output enable)
    // DEMUX A, B, C, and ?D
    // R0, G0, B0: input for the upper 16 rows of the display
    // R1, G1, B1: input for the lower 16 rows of the display
    
    // MCU Pins assign to the inputs of the LED Matrix        
    // R0 -> PA3
    // G0 -> PA4
    // B0 -> PA5            
    // R1 -> PA7
    // G1 -> PA8            
    // B1 -> PA9
    // DEMUX A -> PA10
    // DEMUX B -> PA11
    // DEMUX C -> PA12
    // DEMUX D -> PA24 (or PA27 it seems)
    // CLK -> PA13
    // Latch -> PA14
    // OE -> PA21
    
    // Clock Pin
    PIOA_REGS->PIO_PER |= PIO_PA13;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA13;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA13; // output write enable desired pin
    PIOA_REGS->PIO_ODSR |= PIO_PA13; // start high before being toggled
    // PIOA_REGS->PIO_ODSR &= ~PIO_PA13; // ????

    // BLUE 0 Pin
    PIOA_REGS->PIO_PER |= PIO_PA3;      // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA3;      // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA3;     // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA3;    // drive R0 low
    // PIOA_REGS->PIO_ODSR |= PIO_PA3;  // drive R0 high
    
    // RED 0 Pin
    PIOA_REGS->PIO_PER |= PIO_PA4;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA4;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA4; // output write enable desired pin
    // PIOA_REGS->PIO_ODSR |= PIO_PA4; // drive G0 high
    PIOA_REGS->PIO_ODSR &= ~PIO_PA4; // drive G0 low
    
    // GREEN 0 Pin
    PIOA_REGS->PIO_PER |= PIO_PA5;      // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA5;      // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA5;     // output write enable desired pin
    // PIOA_REGS->PIO_ODSR |= PIO_PA5;  // drive B0 high
    PIOA_REGS->PIO_ODSR &= ~PIO_PA5;    // drive B0 low

    // BLUE 1 Pin
    PIOA_REGS->PIO_PER |= PIO_PA7;      // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA7;      // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA7;     // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA7;    // drive R1 low
    // PIOA_REGS->PIO_ODSR |= PIO_PA7;  // drive R1 high
    
    // RED 1 Pin
    PIOA_REGS->PIO_PER |= PIO_PA8;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA8;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA8; // output write enable desired pin
    // PIOA_REGS->PIO_ODSR |= PIO_PA8; // drive G1 high
    PIOA_REGS->PIO_ODSR &= ~PIO_PA8; // drive G1 low
    
    // GREEN 1 Pin
    PIOA_REGS->PIO_PER |= PIO_PA9;      // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA9;      // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA9;     // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA9;    // drive B1 low
    // PIOA_REGS->PIO_ODSR |= PIO_PA5;  // drive B1 high

    // LATCH Pin
    PIOA_REGS->PIO_PER |= PIO_PA14;     // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA14;     // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA14;    // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA14;   // start latch low
    
    // OE Pin (LED Controller's Output Enable. OE is active low)
    PIOA_REGS->PIO_PER |= PIO_PA21;     // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA21;     // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA21;    // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA21;   // OE
    // PIOA_REGS->PIO_ODSR |= PIO_PA21;

    
    // DEMUX Pin A
    PIOA_REGS->PIO_PER |= PIO_PA10;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA10;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA10; // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA10; // Pull DEMUX Pin A to low
    
    // DEMUX Pin B
    PIOA_REGS->PIO_PER |= PIO_PA11;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA11;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA11; // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA11;// Pull DEMUX Pin B to low
    
    // DEMUX Pin C
    PIOA_REGS->PIO_PER |= PIO_PA12;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA12;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA12; // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA12;// Pull DEMUX Pin C to low
    
    // DEMUX Pin D (D might actually be PA24)
    PIOA_REGS->PIO_PER |= PIO_PA27;  // enable desired pin
    PIOA_REGS->PIO_OER |= PIO_PA27;  // output enable desired pin
    PIOA_REGS->PIO_OWER |= PIO_PA27; // output write enable desired pin
    PIOA_REGS->PIO_ODSR &= ~PIO_PA27;// Pull DEMUX Pin D to low
    
    // Panels operate at 1:16 scan rate
    // LEDs to control: 32 x 64 x 3 = 6144
    // 24 led controllers, each has 16 output (each controller is 16 bit)
    // so 24 x 16 = 384
    // 6144 / 384 = 16 times less bits than we need
    // scan rate should handle that, but not sure what should happen for daisy chained displays
    // I should check that the processor is actually running at 300 MHz
    // pump in all the bits I want one time (384 bits per line)
    // 384 * 2 = 768 (clock enable every other cycle, bit push ever other cycle)
    // Probably need the clock to cycle between every individual bit push!
    // In which case:
    // 384 / 6 = 64
    
    /*
    For each row of pixels, we repeat the following cycle of steps:
        - Clock in the data for the current row one bit at a time
        - Pull the latch and output enable pins high. This enables the latch, allowing the
         * row data to reach the output driver but it also disables the output so that no
         * LEDs are lit while we're switching rows.
        - Switch rows by driving the appropriate row select lines.
        - Pull the latch and output enable pins low again, enabling the output and closing
         * the latch so we can clock in the next row of data.

     */
    
    disableLedControllers();
    PIOA_REGS->PIO_ODSR &= ~PIO_PA14;   // pull latch low

    clearDisplay();
//    PIOA_REGS->PIO_ODSR &= ~PIO_PA3; // set Blue0 low
//    PIOA_REGS->PIO_ODSR &= ~PIO_PA4; // set Red0 low
//    PIOA_REGS->PIO_ODSR &= ~PIO_PA5; // set Green low
//    PIOA_REGS->PIO_ODSR &= ~PIO_PA7; // set Blue1 low    
//    PIOA_REGS->PIO_ODSR &= ~PIO_PA8; // set Red1 low
//    PIOA_REGS->PIO_ODSR &= ~PIO_PA9; // set Green1 low
   


    // Number 3:
    //0x0000 // Row 1
    //0x0000 // Row 2
    //0x01F8 // Row 3
    //0x03FC // Row 4
    //0x07FC // Row 5
    //0x07FE // Row 6
    //0x0E0F // Row 7
                                                       

    
    
    while(1)
    {
          display(203, ARROW_UP);      
    } // End of While(1))
    
    //for(volatile uint32_t j=0; j <500000;j++);
    
    for(;;);
    return (EXIT_SUCCESS);
}

void display(uint16_t num, uint16_t direction)
{
    uint32_t n;
    uint8_t digit1 = 0;
    uint8_t digit2 = 0;
    uint8_t digit3 = 0;
    
//    uint8_t num = 1;
    volatile uint32_t i = 0;
        
    for(i = 0; i <= 15; i++) // Loop to draw 16 rows (draws row 1 & 16, 2 & 17, etc.)
        {        
            // Print Row 3
            inputRowAddress(i + 1);
            disableLedControllers();

            if (num <= 9)
            {
                drawNum(BLANK, i);
                drawNum(num, i);
                drawNum(BLANK, i);
                drawNum(direction, i);
            } else if (num >= 10 && num <= 99) {
                
                digit1 = (num / 10) % 10;   // e.g., If num=43, then digit1 = 4
                digit2 = num % 10;          // e.g., if num=43, then digit2 = 3
                
                drawNum(digit1, i);
                drawNum(digit2, i);
                drawNum(BLANK, i);
                drawNum(direction, i);
            } else {
                digit1 = (num / 100) % 10;   // e.g., If num=43, then digit1 = 4
                digit2 = (num / 10) % 10;   // e.g., If num=43, then digit1 = 4
                digit3 = num % 10;          // e.g., if num=43, then digit2 = 3
                
                drawNum(digit1, i);
                drawNum(digit2, i);
                drawNum(digit3, i);
                drawNum(direction, i);                
            }
            
            enableLedControllers();           

            for(volatile uint32_t j=0; j <400;j++);
            
//            n++;
//            if (n == 1000)
//            {
//                n = 0;
//                num++;
//                if (num == 5)
//                {
//                    num = 1;
//                }
//            }
            clearDisplay();
        }
}

void clearDisplay()
{
    volatile uint32_t i = 0;
    
    clearLeds();
    disableLedControllers();
    
    for (i = 0; i < 64; i++)
    {
        clockPulse();
        latch();
    }
    
    enableLedControllers();
}

void fillRowWithZeros()
{
    clearLeds();
}

void clearLeds()
{
    PIOA_REGS->PIO_ODSR &= ~PIO_PA3; // set Blue0 low
    PIOA_REGS->PIO_ODSR &= ~PIO_PA4; // set Red0 low
    PIOA_REGS->PIO_ODSR &= ~PIO_PA5; // set Green low
    PIOA_REGS->PIO_ODSR &= ~PIO_PA7; // set Blue1 low    
    PIOA_REGS->PIO_ODSR &= ~PIO_PA8; // set Red1 low
    PIOA_REGS->PIO_ODSR &= ~PIO_PA9; // set Green1 low
}

void delay(volatile int number_of_seconds)
{

    // Converting time into milli_seconds
    volatile int milli_seconds = 1000 * number_of_seconds;
 
    // Storing start time
    clock_t start_time = clock();
 
    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void toggleLeds()
{
    PIOA_REGS->PIO_ODSR ^= PIO_PA4; // toggle G0
    PIOA_REGS->PIO_ODSR ^= PIO_PA8; // toggle G1
    PIOA_REGS->PIO_ODSR ^= PIO_PA5; // toggle B0
    PIOA_REGS->PIO_ODSR ^= PIO_PA9; // toggle B1
    PIOA_REGS->PIO_ODSR ^= PIO_PA3; // toggle R0
    PIOA_REGS->PIO_ODSR ^= PIO_PA7; // toggle R1
}

void latch()
{
    PIOA_REGS->PIO_ODSR |= PIO_PA14;   // pull latch high  (Latch!)
    PIOA_REGS->PIO_ODSR &= ~PIO_PA14;   // pull latch low (Latch off))
}

void enableLedControllers()
{
    // OE is active low, so pull OE low to ENABLE LED Controller's Output
    PIOA_REGS->PIO_ODSR &= ~PIO_PA21; 
}

void disableLedControllers()
{
    // OE is active low, so pull OE high to DISABLE LED Controller's Output
    PIOA_REGS->PIO_ODSR |= PIO_PA21;
}

// function to address A, B, C, D with one integer
void inputRowAddress(uint8_t addr)   // only need lower 4 bits of int
{
    if (addr < 9)
    {
        addr += 7;
    } else {
        addr -= 9;
    }
    
    // ABCD   User          DCBA     
    // 0001 = Row 1     8   1000     
    // 1001 = Row 2     9   1001    
    // 0101 = Row 3     10  1010    
    // 1101 = Row 4     11  1011    
    // 0011 = Row 5     12  1100    
    // 1011 = Row 6     13  1101    
    // 0111 = Row 7     14  1110    
    // 1111 = ROW 8     15  1111        
    // 0000 = Row 9     0   0000        
    // 1000 = Row 10    1   0001    
    // 0100 = Row 11    2   0010    
    // 1100 = Row 12    3   0011    
    // 0010 = Row 13    4   0100    
    // 1010 = Row 14    5   0101    
    // 0110 = Row 15    6   0110    
    // 1110 = Row 16    7   0111 
    
    if ((addr & BIT0) == 0)
        PIOA_REGS->PIO_ODSR &= ~PIO_PA10;
    else
        PIOA_REGS->PIO_ODSR |= PIO_PA10;
    
    if ((addr & BIT1) == 0)
        PIOA_REGS->PIO_ODSR &= ~PIO_PA11;
    else
        PIOA_REGS->PIO_ODSR |= PIO_PA11;

    if ((addr & BIT2) == 0)
        PIOA_REGS->PIO_ODSR &= ~PIO_PA12;
    else
        PIOA_REGS->PIO_ODSR |= PIO_PA12;
    
    if ((addr & BIT3) == 0)
        PIOA_REGS->PIO_ODSR &= ~PIO_PA27;
    else
        PIOA_REGS->PIO_ODSR |= PIO_PA27;
    
    return;
}

void clockPulse()
{
    PIOA_REGS->PIO_ODSR ^= PIO_PA13; // clk (Set high)
    PIOA_REGS->PIO_ODSR ^= PIO_PA13; // clk (Set low) So, one pulse
}

void drawNum(uint8_t num, uint32_t i) 
{
    uint16_t n;
    
    for(n = 0; n < 15; n++)  // Fill the Rows. Pushes a bit at every loop
    {
        // Write the top half
        if ((fontRows[num][i] & (1 << n)) > 0)
        {
            PIOA_REGS->PIO_ODSR |= PIO_PA5; // Set G0 High
        } else {
            PIOA_REGS->PIO_ODSR &= ~PIO_PA5; // Set G0 low
        }

        // Write the bottom half
        if ((fontRows[num][i + 16] & (1 << n)) > 0)
        {
            PIOA_REGS->PIO_ODSR |= PIO_PA9; // Set G0 High
        } else {
            PIOA_REGS->PIO_ODSR &= ~PIO_PA9; // Set G0 low
        }                             

        clockPulse();
        latch();
    }
}