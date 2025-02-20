#ifndef BIBLIOTECAS_H
#define BIBLIOTECAS_H

// bibliotecas padrão do SDK
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/interp.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// cabeçalho config.h com as definições do projeto
#include "include/config.h"

#include "build/ws2818b.pio.h"

#endif // BIBLIOTECAS_H