#include <bluetooth.h>
#include <msp430.h>
#include <MSP430FR2xx_4xx/driverlib.h>

volatile unsigned char ReceivedValue = '\0';
volatile bool loggedIn = false;

void exitFunction(void);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
	//WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PMM_unlockLPM5();

//	/* Use Calibration values for 1MHz Clock DCO*/
//	DCOCTL = 0;
//	BCSCTL1 = CALBC1_1MHZ;
//	DCOCTL = CALDCO_1MHZ;
	//ConfigureClockModule();

	//initClockTo16MHz();
	InitializeUART(); // initializes blue tooth connection with msp

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	_enable_interrupts();

	while(1){
           // read user input
	    while (!loggedIn);

        UARTSendString("Please select what size you would like: (s for Small, m for Medium, l for Large, and x to Exit)\r\n");

        while (ReceivedValue == '\0');

        switch (ReceivedValue){
            case 's':
                UARTSendString("Dispensing a Small serving\r\n");
                ReceivedValue = '\0';
                exitFunction();
                break;
            case 'm':
                UARTSendString("Dispensing a Medium serving\n\r");
                ReceivedValue = '\0';
                exitFunction();
                break;
            case 'l':
                UARTSendString("Dispensing a Large serving\n\r");
                ReceivedValue = '\0';
                exitFunction();
                break;
            case 'x':
                exitFunction();
                break;
            default:
                UARTSendString("Please select a different choice\n\r");
                ReceivedValue = '\0';
                break;
        }

    }

}

void exitFunction() {
    UARTSendString("Disconnecting your device...\r\n");
    UARTSendString("AT");
    loggedIn = false;
    return;
}

#pragma vector = USCI_A0_VECTOR
__interrupt
void USCIAB0RX_ISR(void)
{
    ReceivedValue = UARTReceiveByte();   // read user input
    if ((ReceivedValue == 'N') && (!loggedIn)) {
        loggedIn = true;
        ReceivedValue = '\0';
    }
    UCA0IFG &= ~UCRXIFG;
}

