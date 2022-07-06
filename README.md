# tsck-exolifter
This code monitors the high/low states of three pushbuttons and one lockout switch.  When one of the pushbuttons goes "low", a corrresponding LED turns "on", and the other LEDs turn "off".  The pushbuttons are interlocked, so that while one is being pushed, the other two are ignored.  If the lockout switch is set "low", all three pushbuttons are ignored, but the last LED that was turned on stays "on".

Updated code includes control for electromagnets as well as a timeout feature.

For more info, reference the production status notes here:

G:\Shared drives\TSCK RoboSapiens (Internal)\Production\Fabrication\410 Exoskeleton Arm Lift
