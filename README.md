# timebase
Accuarte time source for old clock

A friend of mine came to me with an interesting problem. He has an old clock which runs using mechanical means to provide an actuator coil at a period of approximately 1.33333s. In reality, the accuracy varies over time, with a falling battery voltage.
My challenge, therefore, was to come up with something better and that's what this project is all about.

What do I know about the clock? Well the actuator coil is about 20 ohms and the original battery was a nominal 1.5v cell.

I set myself some basic parameters:
 * Low current solution capable of being run from one or two AAA batteries over a period of at least 6 months
 * Easily interfacable with the clock, without requiring changes to the clock which can't easily be undone.
 * Cheap
 * Accurate to within 5mS a day
 