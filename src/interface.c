#include "include/lib/bibliotecas.h"
#include "include/ssd1306/font.h"
#include "include/ssd1306/ssd1306.h"
#include "include/ssd1306/interface.h"
#include <string.h>

ssd1306_t ssd; // Estrutura do display

// Padrões para a matriz de LEDs 5x5
const uint8_t led_pattern_idle[5][5] = {
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}};

const uint8_t led_pattern_clock[5][5] = {
    {0, 1, 1, 1, 0},
    {1, 0, 0, 0, 1},
    {1, 0, 1, 0, 1},
    {1, 0, 0, 0, 1},
    {0, 1, 1, 1, 0}};

const uint8_t led_pattern_alarm[5][5] = {
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 1}};

// Variáveis para animação
static uint8_t animation_frame = 0;
static absolute_time_t last_animation_update = {0};

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

    // Limpa o display
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Mostra a tela inicial (modo ocioso)
void display_show_idle_screen()
{
    ssd1306_fill(&ssd, false);

    // Desenha uma borda
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);

    // Mostra o título
    ssd1306_draw_string(&ssd, "ALARME IDLE", 10, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Mostra instruções
    ssd1306_draw_string(&ssd, "BEM VINDO!", 25, 20);
    ssd1306_draw_string(&ssd, "Adicione um", 15, 33);
    ssd1306_draw_string(&ssd, "Alarme", 35, 43);
    ssd1306_draw_string(&ssd, "Press: A", 10, 55);

    // Atualiza o display
    ssd1306_send_data(&ssd);

    // Atualiza a matriz de LEDs
    display_update_led_matrix(PATTERN_IDLE);
}

// Mostra a tela de programação
void display_show_program_screen()
{
    ssd1306_fill(&ssd, false);

    // Desenha uma borda
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);

    // Mostra o título
    ssd1306_draw_string(&ssd, "A PROGRAMAR", 10, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Mostra instruções
    ssd1306_draw_string(&ssd, "B: Iniciar", 5, 55);

    // Inicializa com valores zerados
    display_update_program_values(0, 0, 0, 0);

    // Atualiza a matriz de LEDs
    display_update_led_matrix(PATTERN_IDLE);
}

// Atualiza os valores na tela de programação
void display_update_program_values(uint16_t hours, uint16_t minutes, uint16_t seconds, uint8_t selection)
{
    // Limpa a área de exibição
    ssd1306_fill(&ssd, false);

    // Desenha a borda
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);

    // Mostra o título
    ssd1306_draw_string(&ssd, "A PROGRAMAR", 10, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Coordenadas base ajustadas
    uint8_t base_x = 28;  // Posição inicial dos elementos
    uint8_t label_y = 25; // Posição Y dos labels
    uint8_t time_y = 40;  // Posição Y dos números

    // Desenha os labels com espaçamento ajustado
    ssd1306_draw_string(&ssd, "H", base_x + 4, label_y);
    ssd1306_draw_string(&ssd, ":", base_x + 16, label_y);
    ssd1306_draw_string(&ssd, "MIN", base_x + 24, label_y);
    ssd1306_draw_string(&ssd, ":", base_x + 48, label_y);
    ssd1306_draw_string(&ssd, "S", base_x + 56, label_y);

    // Formata o tempo em partes separadas
    char h_buffer[3], m_buffer[3], s_buffer[3];
    sprintf(h_buffer, "%02d", hours);
    sprintf(m_buffer, "%02d", minutes);
    sprintf(s_buffer, "%02d", seconds);

    // Desenha os números com espaçamento ajustado
    ssd1306_draw_string(&ssd, h_buffer, base_x, time_y);
    ssd1306_draw_string(&ssd, ":", base_x + 16, time_y);
    ssd1306_draw_string(&ssd, m_buffer, base_x + 28, time_y);
    ssd1306_draw_string(&ssd, ":", base_x + 48, time_y);
    ssd1306_draw_string(&ssd, s_buffer, base_x + 56, time_y);

    // Mostra instrução no rodapé
    ssd1306_draw_string(&ssd, "B:", 5, 55);
    ssd1306_draw_string(&ssd, "INICIAR", 25, 55);

    // Dimensões para o retângulo de seleção
    uint8_t field_width = 20;  // Largura para dois dígitos
    uint8_t field_height = 14; // Altura do campo
    uint8_t margin = 3;        // Margem para o retângulo de seleção

    // Posições X para cada campo numérico (ajustadas para o novo espaçamento)
    uint8_t field_positions[] = {base_x, base_x + 28, base_x + 56};

    // Desenha o retângulo de seleção apenas se houver um campo selecionado
    if (selection < 3)
    {
        uint8_t select_x = field_positions[selection] - margin;
        uint8_t select_y = time_y - margin;

        // Desenha o retângulo de seleção
        ssd1306_rect(&ssd, select_y, select_x, field_width, field_height, true, false);

        // Redesenha os números para garantir visibilidade
        if (selection == 0)
            ssd1306_draw_string(&ssd, h_buffer, base_x, time_y);
        if (selection == 1)
            ssd1306_draw_string(&ssd, m_buffer, base_x + 28, time_y);
        if (selection == 2)
            ssd1306_draw_string(&ssd, s_buffer, base_x + 56, time_y);
    }

    // Atualiza o display
    ssd1306_send_data(&ssd);
}

// Mostra a tela de contagem regressiva
void display_show_countdown_screen()
{
    ssd1306_fill(&ssd, false);

    // Desenha uma borda
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);

    // Mostra o título
    ssd1306_draw_string(&ssd, "    TIMER    ", 8, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Mostra instruções
    ssd1306_draw_string(&ssd, "A: Cancelar", 5, 55);

    // Atualiza a matriz de LEDs
    display_update_led_matrix(PATTERN_CLOCK);
}

// Atualiza o tempo na tela de contagem regressiva
void display_update_countdown(uint16_t hours, uint16_t minutes, uint16_t seconds)
{
    char buffer[20];

    // Limpa a área do tempo
    ssd1306_rect(&ssd, 20, 25, 88, 20, false, true);

    // Formata e mostra o tempo
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
    ssd1306_draw_string(&ssd, buffer, 30, 30);

    // Atualiza o display
    ssd1306_send_data(&ssd);

    // Atualiza a animação do relógio na matriz de LEDs
    if (absolute_time_diff_us(last_animation_update, get_absolute_time()) >= 1000000)
    {
        last_animation_update = get_absolute_time();
        animation_frame = (animation_frame + 1) % 4;
        display_update_led_matrix(PATTERN_CLOCK);
    }
}

// Mostra a tela de alarme
void display_show_alarm_screen()
{
    ssd1306_fill(&ssd, false);

    // Desenha uma borda
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);

    // Mostra o título com destaque
    ssd1306_draw_string(&ssd, "!!! ALARME !!!", 10, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Mostra a mensagem
    ssd1306_draw_string(&ssd, "Botao A: Parar", 10, 35);

    // Atualiza o display
    ssd1306_send_data(&ssd);

    // Atualiza a matriz de LEDs
    display_update_led_matrix(PATTERN_ALARM);
}

// Atualiza a matriz de LEDs 5x5
void display_update_led_matrix(uint8_t pattern)
{
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2818b_program);
    ws2818b_program_init(pio, sm, offset, MATRIX_LED, 800000);

    // Buffer para os LEDs (5x5 = 25 LEDs)
    uint32_t pixels[25];
    memset(pixels, 0, sizeof(pixels));

    // Preenche o buffer de acordo com o padrão selecionado
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            int idx = y * 5 + x;

            switch (pattern)
            {
            case PATTERN_IDLE:
                pixels[idx] = led_pattern_idle[y][x] ? 0x00001F00 : 0; // Verde se ativo
                break;

            case PATTERN_CLOCK:
                // Animação simples do relógio
                if (led_pattern_clock[y][x])
                {
                    // Varia com base no frame de animação
                    if ((x + y) % 4 == animation_frame)
                    {
                        pixels[idx] = 0x00101080; // Azul claro
                    }
                    else
                    {
                        pixels[idx] = 0x00000080; // Azul escuro
                    }
                }
                break;

            case PATTERN_ALARM:
                // Padrão piscante de alarme
                if (led_pattern_alarm[y][x])
                {
                    pixels[idx] = 0x001F0000; // Vermelho
                }
                break;
            }
        }
    }

    // Envia os dados para a matriz de LEDs
    for (int i = 0; i < 25; i++)
    {
        pio_sm_put_blocking(pio, sm, pixels[i]);
    }
}