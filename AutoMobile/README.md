
LAST UPDATE: 11-4-15 RUCKUS

"Kit" AutoMobile Project

Authors:
Eric Petersen
Shawn Wilson

Project Beginning Date: May 2015

Currently Implemented Hardware:
-- TODO
-- list is currently in AutoMobile.ino Notes

Currently Implemented Functionality:
-- manual drive mode
-- auto-mobile mode
-- full wifi comms
-- telemetry readout

Planed Inprovements:
-- Directional Buttons for Direct drive... with auto off when not press
-- Auto Drive function based on incoming ping sensor
-- a 3d space with a Gameobject that will move based on the Cars movement

PROBLEMS NOT YET OVERCOME:

******************************************

[NODE]No SD card protocols... we should just hollow out all the example codes and make that the basic skeleton for a new library
of SD card commands... make files... save files... read files... get card info... make a SD card webserver... would be a cool
protocol to just open up the running car to code changes from a web brower locally would be a WHOLE NOTHER way to communitcate...
***More thought on the SDWEBserver... super necessary... the phone app needs to be able to put the car in the mode... then cruise
over to its website... download the whatever files that the car has generated... and then put the car back into whatever else mode.
*** *** there could be a file on Ip address it knows... way points it knows... statistical stuff file... total operation time... 
amount of times any sensor has been used... how many hours of runtime do I get on a neopixel strip? or every piece broken down...
can i have protocols that sensor sweep all the radio bands... blue... wifi... 2.4 ... and tries to remeber other people, laptops, 
cell phones, IoTs that it has met. and then corliate just on its own... where it was when it met that other phone, how often it 
meets other phones, stuff like that. With GPSM and GPS... soon... it should be ... hyper aware of where it is... and who nearby...

[MEGA]thing 2. LCD still hammered

[MEGA]THING 3. UNITY WORKS AGAIN AND SENDS BACK AND FORTH YEAH!!!...
but not in the new code implementation... when you call for the sensor read... it only come back to the serial that called it...
need to go to unity.

[MEGA]thing 4. NEO PIXEL is hooked up on PIN 9 and is ready to go but has ZERO code attacked to it.

[MEGA]thing 5. THe compass is in... but not hooked up... im gonna chain it to the MPU but that means it needs extra code in our already 
INSANE mpu library... that oh yeah... achievement +=1...

thing 6. Is the old Ledtree gonna stay there? its not hurting anyone is it? 4 dang on digital dang on pins its taking up... but we 
dont need them for other stuff yet...

thing 7. is it possible a nother nother arduino should be in the mix to handle the screen output the same way the node handles wifi, like a boss?


******************************************


ACHIEVEMENTS:
-- Com functionality to Unity Works well.
-- BUilt NEW ... brand new ... only we use it ... library for the MPU... works better... faster... with temp... totally...
-- Audio... if primitive... still a horn to get outta the way...

UPDATE LOG:

9-19-15 -- Somewhat working avoidance detention over wifi... beginning to read accel data. COMMS COMPLETE.
9-17-15 -- There is enough functionality out of the machine to begin proper documentation
11-3-15 -- The equipment repackaging is now complete. For notes sake, Shawn and I presented the physical car
and our ideas before some folks in billings on 10-21 and wrecked the car off of the table where we were attempting to drive it.
Upon it crashing to the floor and breaking it. Everything on the car was broken down to parts and reassembled cleanly with a 
better layout, new parts and new functionality. ALL in all, its way more badass now. Not only have we add a speaker, with its own
quasi library shawn made from scratch that works awesome and plays mario and james bond themes, but I put the NeoPixel 1x8 led
strip for night rider effects on the front bumper. The MPU-6050 has had extra pinouts for expansion moduals that i have now added.
that is the magnetometer digial compass. ALSO, least of all, is the SD card reader that is hooked up to the Node, which runs 
wickedly faster at 80mhz than the mega does... even the due at 82mhz wouldnt be as fast as the node with all the libraries weighing it 
down. So now with the SD card reader and a little more fancy code, we might get wave files to play off the sd card... and it works with
my 8GB card so thats a lot of *.wavs. At this point we have converted most all of the code to High Grade C++ indoor hydro and that seems
to be more efficient if way more complicated and difficult to use. it is more professional and will make it more usable to other people.
