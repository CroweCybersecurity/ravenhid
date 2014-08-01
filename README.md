ravenhid
=

Hardware and software to run a RFID reader to harvest card information.  This is the PCB design and Arduino code that will run a RFID reader, allowing you to gather and harvest cards.  Typically, a larger reader, such as those in garages, will be more successful, allowing you to ready over a couple feet instead of inches.  The board itself is designed to be modular and support multiple methods to output harvested cards once they are read:
- Text file on a MicroSD card
- Print out to LCD
- Bluetooth Low Energy Arduino serial connection 

Each of these options are supported in code, but can be ignored on the PCB.  The PCB itself has been designed to use a pluggable module for each of these options, making it easy to ignore, install, or change out which ones you find useful.  

#### PCB Design

We used Fritzing, an open source solution, to design the hardware for the RavenHID.  If you open our file in Frizting, you'll see the design which can be changed to suit your needs.  

#### Installation and Build
So the most difficult part of this is getting the PCB created.  There are numerous methods, and you should do what works for you.  If you're interested in having the board fabbed, we've made an OSHPark project to make it easy for you!  They do great work and you'll get three copies of the board just in case you have issues!

https://www.oshpark.com/shared_projects/Kp0nErMB

Once you have your board, you'll need all of the hardware.  The following bill of materials includes everything you need.  All of the parts are labelled on the PCB and everything is through-hole components to make it easy to solder together.  Once everything is put together, you'll need to load the code on your Arduino, which will require the Arduino IDE which can be downloaded freely.  

#### Power

You'll need to get a power pack of some kind.  Personally we've been using a lithium battery pack that is used to charge external devices.  The important things to note before you buy one are: 

* It must output at least 12V, it can output up to about 24V
* It needs to use a standard DC barrel jack, make sure as many of these only output USB
* Make sure it has some sort of auto-off or power switch, as you don't want to be opening the case constantly

The RavenHID PCB doesn't consume much power, but the HID reader most certainly does.  Even with this, we bought a $50 battery pack on Amazon and have never run out of power even leaving the RavenHID on the entire day.  

#### HID Reader
We have been using the [HID MaxiProx 5375|http://www.hidglobal.com/products/readers/hid-proximity/5375] for our reader.  Its large enough to pull cards from a couple feet, it can speak the right protocol for our Arduino, and its large enough to hold all of our electronics on the inside.  Additionally, you can also turn off the beeper, and it fits nicely in several bags, allowing you to gather discreetly. 

The reader can be wired to the RavenHID directly, there is a header which will output power and data to and from the MaxiProx.  

We used the Wiegand protocol to gather data into the RavenHID, so make sure that whatever reader you get is able to support that format.  This format uses two lines, Data0 and Data1, and changes the voltage on these two lines to signal a 0 or 1 bit.  The Arduino reads these one at a time and then compiles it and identifies the card data it contains.  Arduino interrupts are used on these lines, which allows us to do some bitwise addition in the code to get all the bits.  
