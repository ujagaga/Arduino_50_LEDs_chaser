# Arduino_50_LEDs_chaser
An LED chaser for 50 LEDs using Arduino Pro Mini and 3 PCF8575 port extender boards.

Each of the port extenders has 16 pins, so to connect 50 LED's we need to use two more from the Arduino board.
The extenders communicate via I2C (two wire interface) so they should be all configured for a different address.
Their SDA and SCL pins should be tied in parallel and those pins connected to A4 and A5 as they are SDA and SCL hw pins.

One analog pin should be used to read a potentiometer slider to change the timeout. I am using pin A7.

You can use the Arduino onboard voltage stabilizer, but it can not supply much current and 50 LEDs will draw a lot, so they should never be on at the same time.
