# I2C

I found this tutorial.

[stm32-bare-metal-programming-i2c](https://medium.com/@jchrysaphiades/stm32-bare-metal-programming-i2c-4b1f9ed66f53)

There were many options of course, but I liked that one because it was in similar 'bare-metal' style as the first 'blinky' project. Because I like that style, I'll use that tutorial and hack up the first project, CMSIS version, as a starting point. I ordered the external real time clock as an excuse for I2C, though I admit it is a bit silly since the microcontroller (STM32429ZI again) already has one!

Since my board deviates from the one in tutorial, first order of business was checking and deciding on which puns to use for I2C. After consulting multiple AI's, and learning to verify the info in the datasheet (and realizing I didn't even have the proper datasheet!), and even checking out what pins STM32CubeMX would use, I opted for PB8 and PB9.

I'm tempted to to make space in the linker script for my ad hoc debug address, as I'm probably corrupting something (alluded to in first tutorial), but I'm so curious as to whether something will break I'll just wait and see..

