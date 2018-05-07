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

#ifndef _TINY_SPI_H_
#define _TINY_SPI_H_

#if __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct {
  int32_t fd;
  uint32_t speed; // hz
} TinySpi;

int tspi_open(TinySpi *tspi, const char *path, uint32_t speed);

int tspi_close(TinySpi *tspi);

/**
 * @return  Returns the number of bytes written.
 */
int tspi_write(TinySpi *tspi, int num_bytes, uint8_t *data);

#if __cplusplus
}
#endif

#endif // _TINY_SPI_H_