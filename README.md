# Unworthy Space Patrol

This is just a small project to get some good C practice in, additionally with
some learning regarding ncurses.

The actual goals for the game are pretty vague. Firstly, I want it to use as
little memory as possible, while still being playable and without sacrificing
huge amounts of processing power. I want there to be some kind of ship
controlled by the player that can be moved around via arrow keys. I want there
to be projectiles that move independently of whatever else is going on. I want
there to be some kind of collision logic in place to kill the player. Maybe
something for the player to shoot too or something.

Additionally, the choice of C89 was a very deliberate one, as this way I hope
to be better aware of the difficulties present when writing older systems, or
dealing with legacy code.

To build this using make, just call `make`. By default this builds in release
mode, however `make mode=debug` is also available. Debug mode is exactly the
same as release mode except it uses less optimisation so it may at some point
come in handy for tracking down weird bugs.

The makefile automatically detects the sources in the src directory, although it
only finds ones with .c or .h extentions. Therefore, if you find yourself
modifying it throught the addition of c++ portions or the like, be sure to
change the makefile to make it work.

Also, I know I have way more warnings set than may be considered optimal. Being
new to C, I'm using them as a sort of guide to make sure I don't miss anything
obvious, hence the pragmas when I want to explicitely ignore the warnings.
