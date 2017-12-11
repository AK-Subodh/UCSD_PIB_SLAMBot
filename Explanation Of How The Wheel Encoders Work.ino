
/* 

READ THIS BEFORE RUNNING CODE TO UNDERSTAND HOW DISTANCE IS MEASURED USING HALL EFFECT WHEEL ENCODERS
 
 
This will explain how using Hall effect wheel encoders will measure the total distance traveled by the wheels.



Let's look at one single wheel encoder by itself to fully understand how it will measure distance traveled:


The DAGU wheel encoder from sparkfun has a neodymium 8-pole magnet with a rubber hub.

The magnets in this wheel encoder has 4 north poles and 4 south poles (hence a total of 8-poles). The magnet can detect a change in state (in rising or falling edge) 8 different times over a total revolution.

While the magnet of the hall effect wheel encoder is rotating, it will hit the different north and south poles. North is the positive pole and south is the negative pole. 

As the magnet rotates a full revolution, depending on what "pole" it currently resides in, it can detect whether it is in a north pole or south pole region.


Whenever the magnet rotates and reaches a "north pole", it will recognize it as a rising edge (i.e. digital value of "1"). Similarly, whenever the magnet rotates and reaches a "south pole', it will recognize it 
as a falling edge (i.e. digital value of "0").

Because this change in state between rising and falling edge occurs 8 different times for the 8 poles on the magnet, you will be able to know how much distance the car has travelled based on how many times the 
magnet rotating changes its state. 

For example:

0/8: Car not moving. Current state of magnet: 1 => On the north nole area of the magnet (Rising edge) 
Car's wheel moves a certain distance: 1/8: Current state of magnet : 0 => Reached the south pole area of the magnet (Falling edge). So digital input value of wheel encoder has changed from 1 --> 0.

Because you know that the magnet has only turned 1/8th of its full cycle due the state changing oncee, you know that the wheel itself has also turned only 1/8th of its full rotation. This is because 

the magnet itself is attached to the rotating axle of the wheel. So, without accounting for slip, if the magnet rotates 1/8th of its total cycle, so does the wheel. 

So, because you know that the wheel has only moved 1/8th of its total rotation (Think of this 1/8th of a total revolution of the wheel), you know that the total distance traveled by that wheel

is 1/8th of its circumference. 1/8th of the total circumference of the wheel is = 1/8 * (2 * pi * radius of the wheel) = 1/8 * (circumference of the wheel).

The idea is to add 1/8th of the total circumference to the total distance traveled whenever there is a change in the state of the wheel encoder.

So, => Having a change in the state of the wheel encoder => means 1/8th of revolution complete => 1/8 * (circumference of the wheel) distance traveled.


 */
