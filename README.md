# I2C

I found this tutorial.

[stm32-bare-metal-programming-i2c](https://medium.com/@jchrysaphiades/stm32-bare-metal-programming-i2c-4b1f9ed66f53)

There were many options of course, but I liked that one because it was in similar 'bare-metal' style as the first 'blinky' project. Because I like that style, I'll use that tutorial and hack up the first project, CMSIS version, as a starting point. As you will see, I follow the tutorial fairly close for the I2C protocol but then deviate near the end.

I made 8 bytes of space in the linker script for ad hoc debugging, so I can vet my code is running with an incrementing value plus have space to set another int. In theory I was corrupting something otherwise, though everything seemed to work fine even without the space allocated.

I ordered the external real time clock as an excuse for I2C, though I admit it is a bit silly since the microcontroller (STM32429ZI again) already has one. Then again, the external one includes a battery, so perhaps not so silly..

Since my board deviates from the one in tutorial, first order of business was checking and deciding on which pins to use for I2C. After consulting multiple AI's, and learning to verify the info in the datasheet / reference manual, and even checking out what pins STM32CubeMX would use, I opted for PB8 and PB9, which differ from the tutorial. The author chose I2C2 for reasons he doesn't say. I'll go with I2C1 since AI suggests that is the default and the pins might be easier to work with.

There are some interesting choices as to whether to include pin logic register math like the author or use all the CMSIS constants. I've experimented with both styles. CMSIS is definitely helpful for the base registers addresses. 

I think the author makes an error in saying that 'master' sends the 'ack' for the write case. This 'ack' instead comes from the slave, except for when the master is reading from the slave. I discovered another discrepancy. The author states it is reasonable to assume any board with designated I2C pins would have pullup resistors, needed for I2C to be reliable. Yet Claude says the opposite, that neither his board nor mine actually have those, but that they are commonly provided with the external I2C component board. It is possible he misunderstands why his system is working. I checked my RTC order and it is unclear if it has pullup resistors, though it seems likely. For now I'll just follow along and see.

The author sets high speed mode without saying why. His I2C reset code seems convoluted to me, and since it might not even be needed I'll try leaving it out completely. I also removed the unnecessary variable set for ensuring a register read. 

The author's math derivation for setting SCL frequency was very helpful however vs the obtuse reference manual, and might be the most valuable part of the tutorial for me. I found the DS3231 datasheet and confirmed the max rise time as shown by author. Those setting were also tricky, and I can get why some would want to let the IDE auto generate this. I suspect understanding these concepts could come in handy for debugging however.

One aspect not explained was the I2C address. I assumed mine would be different, but I've since learned all such devices are set to the same and further confirmed with the datasheet.
