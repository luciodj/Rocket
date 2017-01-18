# 1 - Hello World

First demo program: Blinking an LED of course!

* Device: PIC16F1619
* Board: Curiosity (DM164137)
* MPLAB X and MPLAB Code Configurator

## The Basic Idea
* Toggle RA5 (right most LED) on the curiosity development board.
* Add a 500ms (1/2s) delay using the builtin function *__delay_ms()*.
* That's all!

## Details
Remember to:
* Enable Low Voltage Programming (Needed by the Curiosity on board programmer)
* Select 4MHz INTOSC

## What Next?
* Use a Timer peripheral to define the blinking rate
* Use the Potentiometer (with the ADC) to change the blinking rate
* Use a PWM peripheral to make the LED pulse (smooth dimming)

## References
* Book: [This is (not) Rocket Science](http://blog.flyingpic24.com/rocket)





    

