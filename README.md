# ESP8266 ARDUINO EXAMPLE - AES256
This is a pretty simple sketch that performs some basic AES256 encryption on a pre-padded string. It uses some (perhaps slightly under-documented) functions included in the bearSsl library to do everything so no major external dependencies are needed.

This code was developed / tested on an adafruit feather huzzah.

# Setup / Building
You should refer to the setup instructions for the [makeEspArduino](https://github.com/plerup/makeEspArduino) project for details on how to set this stuff up. Once i had the arduino IDE working for my esp8266 everything pretty much just worked. You need to configure the tty and board configuration of your esp8266 inside the Makefile if it is any different from my the setup i had.

Once everything is configured you compile with make and you can upload using make upload.

# References

* [This code was put together based on an example inside the bearSsl unit test suite.](https://github.com/earlephilhower/bearssl-esp8266/blob/master/test/test_crypto.c#L3699)
* [This code uses my own base64 encoder to print some of the binary data to the serial port](https://github.com/hannasm/base64c)
* [All the really complicated makefile stuff was from a separate github project](https://github.com/plerup/makeEspArduino)
