# I2C

I found this tutorial.

[stm32-bare-metal-programming-i2c](https://medium.com/@jchrysaphiades/stm32-bare-metal-programming-i2c-4b1f9ed66f53)

There were many options of course, but I liked that one because it was in similar 'bare-metal' style as the first 'blinky' project. Because I like that style, I'll use that tutorial and hack up the first project, CMSIS version, as a starting point. I'm tempted to to make space in the linker script for my ad hoc debug address, as I'm probably corrupting something (alluded to in first tutorial), but I'm so curious as to whether something will break I'll just wait and see.. 😄

I ordered the external real time clock as an excuse for I2C, though I admit it is a bit silly since the microcontroller (STM32429ZI again) already has one!

Since my board deviates from the one in tutorial, first order of business was checking and deciding on which puns to use for I2C. After consulting multiple AI's, and learning to verify the info in the datasheet / reference manual, and even checking out what pins STM32CubeMX would use, I opted for PB8 and PB9, which differ from the tutorial. The author chose I2C2 for reasons he doesn't say. I'll go with I2C1 since AI suggests that is the default and the pins might be easier to work with.

I compared both approaches, but actually like the way the author exposes some of the pin math as registers are set as opposed to full use of CMSIS constants, which seems to hide too much. Therefore I shall adopt similar. I still like CMSIS for the base registers addresses. 
