#include "include/lib/bibliotecas.h"
#include "include/ssd1306/font.h"
#include "include/ssd1306/ssd1306.h"
#include "include/ssd1306/interface.h"

ssd1306_t ssd; // Estrutura do display

static bool cor = true;

// Inicializa o display SSD1306
void displayssd1306_init()
{
    // Inicializa o I2C a 400Khz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa e configura o display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);

    // Limpa o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Atualiza o display com as informações do joystick e desenha o formato (quadrado ou triângulo)
static inline void update_display()
{
    // Inicializa o display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);

    cor = !cor;

    // Desenha o formato de teste do display
        // Limpa o display com a cor inversa
    ssd1306_fill(&ssd, !cor);
    ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor);
    ssd1306_draw_string(&ssd, " TESTE I2C", 8, 10);
    ssd1306_draw_string(&ssd, " 123 teste", 20, 30);

    ssd1306_send_data(&ssd); // Envia os dados para o display
}