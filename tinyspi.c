/**
 * Copyright (c) 2017, Martin Roth (mhroth@gmail.com)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include <assert.h>
#include <fcntl.h>            // Needed for SPI port
#include <linux/spi/spidev.h> // Needed for SPI port
#include <string.h>
#include <sys/ioctl.h>        // Needed for SPI port
#include <unistd.h>           // Needed for SPI port

#include <errno.h>
#include <stdio.h>

// http://www.raspberry-projects.com/pi/programming-in-c/spi/using-the-spi-interface
#include "tinyspi.h"

int tspi_open(TinySpi *tspi, const char *path, uint32_t spi_speed) {
  assert(tspi != NULL);
  assert(path != NULL);

  int fd = open(path, O_RDWR);
  assert(fd >= 0);

  int ret = 0;

  //----- SET SPI MODE -----
  // SPI_MODE_0 (0,0) CPOL = 0, CPHA = 0, Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
  // SPI_MODE_1 (0,1) CPOL = 0, CPHA = 1, Clock idle low, data is clocked in on falling edge, output data (change) on rising edge
  // SPI_MODE_2 (1,0) CPOL = 1, CPHA = 0, Clock idle high, data is clocked in on falling edge, output data (change) on rising edge
  // SPI_MODE_3 (1,1) CPOL = 1, CPHA = 1, Clock idle high, data is clocked in on rising, edge output data (change) on falling edge
  uint8_t spi_mode = 0;
  ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode); // set write mode
  assert((ret == 0) && "Write mode could not be set.");
  ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode); // set read mode
  assert((ret == 0) && "Read mode could not be set.");

  uint8_t spi_bitsPerWord = 8;
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
  assert((ret == 0) && "Bits per word could not be set.");
  ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
  assert((ret == 0) && "Bits per word could not be set.");

  ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
  assert((ret == 0) && "Write speed could not be set.");
  ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
  assert((ret == 0) && "Write speed could not be set.");

  tspi->fd = fd;
  tspi->speed = spi_speed;
  return fd;
}

int tspi_close(TinySpi *tspi) {
  assert(tspi != NULL);
  assert(tspi->fd > 0);
  int ret = close(tspi->fd);
  assert((ret == 0) && "SPI interace was not properly closed.");
  return ret;
}

int tspi_write(TinySpi *tspi, int num_bytes, uint8_t *data) {
  assert(tspi != NULL);
  assert(num_bytes >= 0);
  assert(data != NULL);

  // https://raspberrypi.stackexchange.com/questions/65595/spi-transfer-fails-with-buffer-size-greater-than-4096
  assert((num_bytes < 4096) && "SPI transfer size is too large.");

  struct spi_ioc_transfer spi;
  memset(&spi, 0, sizeof(struct spi_ioc_transfer));
  spi.tx_buf = (uint64_t) data;
  spi.rx_buf = 0; // don't care about receiving
  spi.len = num_bytes;
  spi.delay_usecs = 0;
  spi.speed_hz = tspi->speed;
  spi.bits_per_word = 8;
  spi.cs_change = 0;

  int ret = ioctl(tspi->fd, SPI_IOC_MESSAGE(1), &spi);
  if (ret == -1) {
    printf("TinySPI ioctl write fail: %s\n", strerror(errno));
    assert(0 && "Error while sending SPI message.");
  }

  return ret;
}
