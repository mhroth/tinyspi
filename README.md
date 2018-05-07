# TinySpi
Tiny SPI is a small C library implementing a simple interface to the [Serial Peripheral Interface](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface_Bus) bus (SPI), such as is available on a Raspberry Pi.

## Usage
```c
#include "tinyspi.h"

#define SPI_HZ 2000000 // bus frequency

// declare the TinySpi structure
TinySpi tspi;

// open the SPI interface
tspi_open(&tspi, "/dev/spidev0.0", SPI_HZ);

// send data (e.g. to LEDs)
tspi_write(&tspi, num_data_bytes, data_bytes);

// close the SPI interface
tspi_close(&tspi);
```

## License
This code is released under the [ISC License](https://opensource.org/licenses/ISC).
