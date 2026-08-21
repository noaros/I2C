# I2C

I found this tutorial.

[stm32-bare-metal-programming-i2c](https://medium.com/@jchrysaphiades/stm32-bare-metal-programming-i2c-4b1f9ed66f53)

There were many options of course, but I liked that one because it was in similar 'bare-metal' style as the first 'blinky' project. Because I like that style, I'll use that tutorial and hack up the first project, CMSIS version, as a starting point. I'm tempted to to make space in the linker script for my ad hoc debug address, as I'm probably corrupting something (alluded to in first tutorial), but I'm so curious as to whether something will break I'll just wait and see.. 😄

I ordered the external real time clock as an excuse for I2C, though I admit it is a bit silly since the microcontroller (STM32429ZI again) already has one!

Since my board deviates from the one in tutorial, first order of business was checking and deciding on which puns to use for I2C. After consulting multiple AI's, and learning to verify the info in the datasheet / reference manual, and even checking out what pins STM32CubeMX would use, I opted for PB8 and PB9, which differ from the tutorial. The author chose I2C2 for reasons he doesn't say. I'll go with I2C1 since AI suggests that is the default and the pins might be easier to work with.

There are some interesting choices as to whether to include pin logic like the author or use all the CMSIS constants. I've experimented with both styles. CMSIS definitely helpful for the base registers addresses. 

I discovered a fascinating conflict. The author states it is reasonable to assume any board with designated I2C pins would have pullup resistors, needed for I2C to be reliable. Yet Claude says the opposite, that neither his board nor mine actually have those, but that they are commonly provided with the external I2C component board. It is possible he misunderstands why his system is working. I checked my RTC order and it is unclear if it has pullup resistors, though it seems likely. For now I'll just follow along and see.

The author sets high speed mode without saying why. His I2C reset code seems convoluted to me, and since it might not even be needed I'll try leaving it out completely. The author's math derivation for setting SCL frequency was very helpful however vs the obtuse reference manual, and might be the most valuable part of the tutorial for me. I found the DS3231 datasheet and confirmed the max rise time as shown by author. Those setting were also tricky, and I can get why some would want to let the IDE auto generate this. I suspect understanding these concepts could come in handy for debugging however.

One aspect not explained was the I2C address. I assumed mine would be different, but I was able to confirm it was not in the datasheet.
