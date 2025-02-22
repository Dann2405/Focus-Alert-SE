#ifndef SSD1306_H
#define SSD1306_H

// Cabeçalho responsavel por definir as funções de controle do display OLED SSD1306

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define WIDTH 128
#define HEIGHT 64

// Comandos do display
typedef enum
{
  SET_CONTRAST = 0x81,
  SET_ENTIRE_ON = 0xA4,
  SET_NORM_INV = 0xA6,
  SET_DISP = 0xAE,
  SET_MEM_ADDR = 0x20,
  SET_COL_ADDR = 0x21,
  SET_PAGE_ADDR = 0x22,
  SET_DISP_START_LINE = 0x40,
  SET_SEG_REMAP = 0xA0,
  SET_MUX_RATIO = 0xA8,
  SET_COM_OUT_DIR = 0xC0,
  SET_DISP_OFFSET = 0xD3,
  SET_COM_PIN_CFG = 0xDA,
  SET_DISP_CLK_DIV = 0xD5,
  SET_PRECHARGE = 0xD9,
  SET_VCOM_DESEL = 0xDB,
  SET_CHARGE_PUMP = 0x8D
} ssd1306_command_t;

// Estrutura do display
typedef struct
{
  uint8_t width, height, pages, address;
  i2c_inst_t *i2c_port;
  bool external_vcc;
  uint8_t *ram_buffer;
  size_t bufsize;
  uint8_t port_buffer[2];
} ssd1306_t;

// Protótipos das funções
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void ssd1306_config(ssd1306_t *ssd);
void ssd1306_command(ssd1306_t *ssd, uint8_t command);
void ssd1306_send_data(ssd1306_t *ssd);

void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value);
void ssd1306_fill(ssd1306_t *ssd, bool value);
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill);
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value);
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value);
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value);
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y);
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y);

void displayssd1306_init();

// Inicializa o display
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c)
{
  ssd->width = width;
  ssd->height = height;
  ssd->pages = height / 8U;
  ssd->address = address;
  ssd->i2c_port = i2c;
  ssd->bufsize = ssd->pages * ssd->width + 1;
  ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
  ssd->ram_buffer[0] = 0x40;
  ssd->port_buffer[0] = 0x80;
}

// Configura o display
void ssd1306_config(ssd1306_t *ssd)
{
  ssd1306_command(ssd, SET_DISP | 0x00);
  ssd1306_command(ssd, SET_MEM_ADDR);
  ssd1306_command(ssd, 0x01);
  ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
  ssd1306_command(ssd, SET_SEG_REMAP | 0x01);
  ssd1306_command(ssd, SET_MUX_RATIO);
  ssd1306_command(ssd, HEIGHT - 1);
  ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
  ssd1306_command(ssd, SET_DISP_OFFSET);
  ssd1306_command(ssd, 0x00);
  ssd1306_command(ssd, SET_COM_PIN_CFG);
  ssd1306_command(ssd, 0x12);
  ssd1306_command(ssd, SET_DISP_CLK_DIV);
  ssd1306_command(ssd, 0x80);
  ssd1306_command(ssd, SET_PRECHARGE);
  ssd1306_command(ssd, 0xF1);
  ssd1306_command(ssd, SET_VCOM_DESEL);
  ssd1306_command(ssd, 0x30);
  ssd1306_command(ssd, SET_CONTRAST);
  ssd1306_command(ssd, 0xFF);
  ssd1306_command(ssd, SET_ENTIRE_ON);
  ssd1306_command(ssd, SET_NORM_INV);
  ssd1306_command(ssd, SET_CHARGE_PUMP);
  ssd1306_command(ssd, 0x14);
  ssd1306_command(ssd, SET_DISP | 0x01);
}

// Envia um comando para o display
void ssd1306_command(ssd1306_t *ssd, uint8_t command)
{
  ssd->port_buffer[1] = command;
  i2c_write_blocking(
      ssd->i2c_port,
      ssd->address,
      ssd->port_buffer,
      2,
      false);
}

// Envia os dados para o display
void ssd1306_send_data(ssd1306_t *ssd)
{
  ssd1306_command(ssd, SET_COL_ADDR);
  ssd1306_command(ssd, 0);
  ssd1306_command(ssd, ssd->width - 1);
  ssd1306_command(ssd, SET_PAGE_ADDR);
  ssd1306_command(ssd, 0);
  ssd1306_command(ssd, ssd->pages - 1);
  i2c_write_blocking(
      ssd->i2c_port,
      ssd->address,
      ssd->ram_buffer,
      ssd->bufsize,
      false);
}

// Define um pixel no buffer de memória
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value)
{
  uint16_t index = (y >> 3) + (x << 3) + 1;
  uint8_t pixel = (y & 0b111);
  if (value)
    ssd->ram_buffer[index] |= (1 << pixel);
  else
    ssd->ram_buffer[index] &= ~(1 << pixel);
}

/*
void ssd1306_fill(ssd1306_t *ssd, bool value) {
  uint8_t byte = value ? 0xFF : 0x00;
  for (uint8_t i = 1; i < ssd->bufsize; ++i)
    ssd->ram_buffer[i] = byte;
}*/

// Preenche o display com um valor
void ssd1306_fill(ssd1306_t *ssd, bool value)
{
  // Itera por todas as posições do display
  for (uint8_t y = 0; y < ssd->height; ++y)
  {
    for (uint8_t x = 0; x < ssd->width; ++x)
    {
      ssd1306_pixel(ssd, x, y, value);
    }
  }
}

// Desenha um retângulo no display
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value, bool fill)
{
  for (uint8_t x = left; x < left + width; ++x)
  {
    ssd1306_pixel(ssd, x, top, value);
    ssd1306_pixel(ssd, x, top + height - 1, value);
  }
  for (uint8_t y = top; y < top + height; ++y)
  {
    ssd1306_pixel(ssd, left, y, value);
    ssd1306_pixel(ssd, left + width - 1, y, value);
  }

  if (fill)
  {
    for (uint8_t x = left + 1; x < left + width - 1; ++x)
    {
      for (uint8_t y = top + 1; y < top + height - 1; ++y)
      {
        ssd1306_pixel(ssd, x, y, value);
      }
    }
  }
}

// Desenha um círculo no display
void ssd1306_circle(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t radius, bool value)
{
  int x = radius;
  int y = 0;
  int err = 1 - x;

  while (x >= y)
  {
    ssd1306_pixel(ssd, x0 + x, y0 + y, value);
    ssd1306_pixel(ssd, x0 + y, y0 + x, value);
    ssd1306_pixel(ssd, x0 - y, y0 + x, value);
    ssd1306_pixel(ssd, x0 - x, y0 + y, value);
    ssd1306_pixel(ssd, x0 - x, y0 - y, value);
    ssd1306_pixel(ssd, x0 - y, y0 - x, value);
    ssd1306_pixel(ssd, x0 + y, y0 - x, value);
    ssd1306_pixel(ssd, x0 + x, y0 - y, value);

    y++;

    if (err < 0)
    {
      err += 2 * y + 1;
    }
    else
    {
      x--;
      err += 2 * (y - x + 1);
    }
  }
}

// Desenha uma linha no display
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value)
{
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);

  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;

  int err = dx - dy;

  while (true)
  {
    ssd1306_pixel(ssd, x0, y0, value); // Desenha o pixel atual

    if (x0 == x1 && y0 == y1)
      break; // Termina quando alcança o ponto final

    int e2 = err * 2;

    if (e2 > -dy)
    {
      err -= dy;
      x0 += sx;
    }

    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

// Desenha uma linha horizontal no display
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value)
{
  for (uint8_t x = x0; x <= x1; ++x)
    ssd1306_pixel(ssd, x, y, value);
}

// Desenha uma linha vertical no display
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value)
{
  for (uint8_t y = y0; y <= y1; ++y)
    ssd1306_pixel(ssd, x, y, value);
}

// Desenha um caractere no display
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y)
{
  uint16_t index = 0;
  if (c >= 'A' && c <= 'Z')
  {
    index = (c - 'A' + 11) * 8; // Letras maiúsculas: de A a Z, começando em 88
  }
  else if (c >= 'a' && c <= 'z')
  {
    index = (c - 'a' + 37) * 8; // Letras minúsculas: de a a z, começando em 296
  }
  else if (c >= '0' && c <= '9')
  {
    index = (c - '0' + 1) * 8; // Dígitos: '0' começa em índice 8, '9' termina em 87
  }
  else if (c == ':')
  {
    index = 504; // caractere ':'
  }
  else if (c == '!')
  {
    index = 512; // caractere '!'
  }

  // Desenha os 8 bytes do caractere
  for (uint8_t i = 0; i < 8; ++i)
  {
    uint8_t line = font[index + i];
    for (uint8_t j = 0; j < 8; ++j)
    {
      ssd1306_pixel(ssd, x + i, y + j, line & (1 << j));
    }
  }
}

// Função para desenhar uma string
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y)
{
  while (*str)
  {
    ssd1306_draw_char(ssd, *str++, x, y);
    x += 8;
    if (x + 8 >= ssd->width)
    {
      x = 0;
      y += 8;
    }
    if (y + 8 >= ssd->height)
    {
      break;
    }
  }
}

#endif