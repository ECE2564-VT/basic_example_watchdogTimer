/*
 * This application illustrates a simple use of watchdog timer.
 * It can be considered a simple extension to basic_example_GPIO project.
 * Whenever we press S1, we not only turn on LED1, but also reset the watchdog timer, which keeps the application alive.
 * If we don't press S1 for sometime (the interval of watchdog timer),
 * watchdog timer rolls over and does a soft reset on the processor.
 * We have written the code such that when the processor comes out of soft reset,
 * it turns on the left LED2 red and does nothing.
 *
 */

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// This function initializes GPIO
void initializeGPIO();
void initializeWatchdogTimer();

void TurnOn_Launchpad_LED1();
void TurnOff_Launchpad_LED1();
void Toggle_Launchpad_LED1();
void TurnOn_Launchpad_LED2RED();
void TurnOn_Launchpad_LED2GREEN();
void Toggle_Launchpad_LED2GREEN();
void TurnOff_Launchpad_LED2RED();
char S1isPressed();

#define PRESSED 0

int main(void)
{

    initializeWatchdogTimer();
    initializeGPIO();


    // If the system comes out of a soft reset (forced by the Watchdog timer),
    // it only turns on the LED2 red and does nothing.
    if ((ResetCtl_getSoftResetSource() & RESET_SRC_1) == RESET_SRC_1) {
        TurnOn_Launchpad_LED2RED();
        while(1);
    }

    // If the system did not come out of a soft reset (started regularly),
    // it turns on the LED2 green and performs the simple operation we had implemented before in the GPIO application.
    // pressing S1 turns LED1 on and not pressing keeps it off. The only difference is that pressing S1 also resets the
    // watchdog timer, which means it keeps the application alive. If we don't press S1 for sometime, the watchdog timer
    // rolls over and therefore, it resets the processor in a soft reset.
    // This means, the next timer the processor comes back alive, the application goes to the "if" branch of this if-then-else.
    else {
        TurnOn_Launchpad_LED2GREEN();

        while (1) {

            if (!S1isPressed())
                TurnOff_Launchpad_LED1();

            // otherwise, turn the LED on
            else {
                TurnOn_Launchpad_LED1();

                // reset the watchdog timer. This "services" the dog. Or keeps the watchdog timer happy.
                // This means the processor is not going to be reset for some time.
                WDT_A_clearTimer();
            }
        }
    }

}

// -----------------------------------------------------------------------------------------------------------------
// Watchdog Timer Inititalization
// -----------------------------------------------------------------------------------------------------------------
void initializeWatchdogTimer() {

    // We can specify resets as either soft-resets or hard-resets. A soft reset allows us to check a different flag to
    // see if the WDT reset the system, but a hard reset is indistinguishable from unplugging the board and plugging it
    // back in.
    SysCtl_setWDTTimeoutResetType(SYSCTL_SOFT_RESET);

    // Initialize the WDT to use a value of 8192000 in its count-up timer
    WDT_A_initWatchdogTimer(WDT_A_CLOCKSOURCE_SMCLK, WDT_A_CLOCKITERATIONS_8192K);

    // In this example, we do not change the clock source of the watchdog timer. By default this clock is the system clock.

    // Since the system clock is 3MHz in this example, the watchdog interval is 8,192,000/3,000,000 ~ 2.73 seconds.
    // So, if we don't reset the watchdog timer within 2.73 s from starting it, it will reset the processor.

    // Start the watchdog timer
    WDT_A_startTimer();
}

// Initialization part is always device dependent and therefore does not change much with HAL
void initializeGPIO()
{
    // Initializing LED1, which is on Pin 0 of Port P1 (from page 37 of the Launchpad User Guide)
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

    // Initializing LED2 red, which is on Pin 0 of Port P2 (from page 37 of the Launchpad User Guide)
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);

    // Initializing LED2 green, which is on Pin 1 of Port P2 (from page 37 of the Launchpad User Guide)
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);

    // Initializing S1 (switch 1 or button 1) on launchpad
    // which is on Pin1 of Port 1 (from page 37 of the Launchpad User Guide)
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN1);

    // Initializing S2 (switch 2 or button 2) on launchpad
    GPIO_setAsInputPinWithPullUpResistor (GPIO_PORT_P1, GPIO_PIN4);

}

void TurnOn_Launchpad_LED1()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
}
void TurnOff_Launchpad_LED1()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void Toggle_Launchpad_LED1()
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
}

void TurnOn_Launchpad_LED2RED()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
}

void TurnOn_Launchpad_LED2GREEN()
{
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);
}

void Toggle_Launchpad_LED2GREEN()
{
    GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
}
void TurnOff_Launchpad_LED2RED()
{
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
}
char S1isPressed()
{
    return ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == PRESSED));
}

