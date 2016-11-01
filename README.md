# Transitnotifier_RedGuppy

This is the transit notifier project called Buzz Clock.
You will need the following hardware and software to build the Buzz Clock: 

Hardware:
<ul>
<li>ESP8266 x 1 (adafruit Huzzah breakout for the PCB we used)</li> 
<li>NeoPixel strip x 1 (60 neoPixels.  You can use less but you'll need to modify the code heavily)</li>
<li>Battery x 1 (or a USB 5 volt power supply with power and ground connections</li>
<li>Resistor x 2 (10k and 470R)</li>
<li>Capacitor x 1 (1000U 16v or 24v) </li>
<li>Push button x 1</li>
</ul>

Software:
<ul>
<li>Adafruit NeoPixel library</li>
<li>WiFi Client Secure library</li>
<li>TinyXML library</li>
</ul>

The working code resides in the NextBusGT folder. After connecting the hardwares together and connecting ESP8266 to your laptop, you need to modify the code with your wifi SSID and Password then upload the code to the ESP8266 and it will start retrieve the data from the xml provided by gtbuses.herokuapp.com.

If you wish to make your own PCB instead of using a breadboard for the setup you can start with our CAD files in the WiFiNeoClock director.  We used KiCad to create the board.  Additional information on working with kiCad can be found at..
http://kicad-pcb.org/
