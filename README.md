# cmkstatus

cmkstatus implements a LED display for the checkmk service statistics.
The top row is updated once per minute with the status in following preference:
ok (green), critical (red), warning (yellow), unknown (orange), downtime (blue).
The lower 7 rows are showing the history (each row is 15 minutes), the first is
green or red depending on status, additional LED can be lit for warning, unknown
and downtime.

# Requirements

## Hardware

* Adafruit Feather M0 Basic Proto
* Adafruit Neopixel FeatherWing

## IDE

* Arduino IDE

* Board Manager:
    * Arduino SAMD Boards
    * Adafruit SAMD Boards

* Libraries:
    * Adafruit DMA neopixel library
    * Adafruit Neopixel
    * SerialCommands

# Installation

Assemble the Feather M0 and Neopixel. The default PIN6 is used for the LED data.
Compile and install the arduino sketch `display/display.ino`.

Copy the python script (e.g. to `/usr/sbin`). The script assumes is located at /omd (a link to /opt/omd). To avoid changing ttyACM* devices use the `/dev/serial/by-id/` path.
```shell
/usr/sbin/checkmk-status example /dev/serial/by-id/usb-Adafruit_Feather_M0_xyz-if00
```
This will display the status of site example. The script has direction option (default top) for setting the installation direction (top is USB port is up).

There is an example sytemd unit template in `examples/checkmkstatus.service`.