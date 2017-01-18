# 2 - Servo

Second Demo program: Control a small Servo Motor position to follow a potentiometer!

* Device: PIC16F1619
* Board: Curiosity (DM164137)
* MPLAB X , XC8 and MPLAB Code Configurator
* Servo motor: Model Craft ES-05 or similar

## The Basic Idea
* Use TMR2 to generate an 8ms time base.
* Use CCP1 (PWM) to produce a PWM signal (output to pin RB4).
* Use ADC (Auto-triggered by TMR2) to read the potentiometer (RC0).
* Update the PWM Duty Cycle between 0.4 and 1.4 ms after each ADC conversion.
* That's all!


## Details
* Remember to enable Low Voltage Programming (Need by Curiosity on board programmer), it can be found in CONFIG2 (System page).

## What Next?
* Add filtering to avoid the Servo to oscillate un-necessesarily between two positions.
* Divert/copy the PWM output to an LED to dim it.
* Control independently more Servos using all the PWMs available

## References
* Book: [This is (not) Rocket Science](http://blog.flyingpic24.com/rocket)







