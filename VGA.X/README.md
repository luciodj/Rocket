# VGA Display  


*  Device:    [PIC16F1619](http://microchip.com/pic16f161x)
*  Board:     [PICDEM Curiosity (DM164137)](http://microchip.com/curiosity)
*  Tools:     [MPLAB X , XC8 and MPLAB Code Configurator](htto://microchip.com/mplabx)

## The Basic Idea
* Driving a VGA display directly from the Curiosity board. 
* You will need:
    * A  [VGA connector (female)](https://en.wikipedia.org/wiki/VGA_connector)
    * Four wires, connected as follows:
    

     PIC      |    VGA 
 :---------:  | :---------:
 Pin 11 - RB6 | Pin 2 - Green
 Pin 12 - RB5 | Pin 13 - HSYNC
 Pin 13 - RB4 | Pin 14 - VSYNC
 Pin 20 - GND | Pin 5,7,10  - GND
 
## Software Implementation 
* Because of the small amount of RAM available (on a PIC16F1619) only a 25 x 20 Text page can be supported. 
* Since the `putch()` function is (re)defined, all the `stdio.h` functions ( puts(), printf()) are re-directed to the VGA display. 
* A simple scroll capability is provided for continuous text output.
* Use globals `cx` and `cy` to position the cursor. 

              
## More Ideas
* Experiment connecting other color inputs ( Red, Blue)
* Use PPS to choose the color to output the image to (UART TX) 
* Experiment with ["block graphics"](https://en.wikipedia.org/wiki/ZX_Spectrum_character_set)
* ...  

## References
* Buy the book at: [This is (not) Rocket Science](http://www.lulu.com/shop/lucio-di-jasio/this-is-not-rocket-science/paperback/product-22096090.html)
* Support: [This is (not) Rocket Science](http://flyingpic24.com/rocket)
* Blog: [The Pilot Logbook](http://blog.flyingpic24.com)
* More: [Simplicity](http://github.com/luciodj/Simplicity)

    
     

    

