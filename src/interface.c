#include "include/lib/bibliotecas.h"
#include "include/ssd1306/font.h"
#include "include/ssd1306/ssd1306.h"
#include "include/ssd1306/interface.h"
#include <string.h>

ssd1306_t ssd; // Estrutura do display

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
    ssd1306_draw_string(&ssd, "FOCUS ALERT", 20, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Mostra instruções
    ssd1306_draw_string(&ssd, "BEM VINDO!", 25, 20);
    ssd1306_draw_string(&ssd, "A: Programar", 5, 40);
    ssd1306_draw_string(&ssd, "Alarme", 36, 50);

    // Atualiza o display
    ssd1306_send_data(&ssd);
}

// Atualiza os valores na tela de programação
void display_update_program_values(uint16_t hours, uint16_t minutes, uint16_t seconds, uint8_t selection)
{
    // Limpa a área de exibição
    ssd1306_fill(&ssd, false);

    // Desenha a borda
    ssd1306_rect(&ssd, 0, 0, WIDTH, HEIGHT, true, false);

    // Mostra o título
    ssd1306_draw_string(&ssd, "PROGRAMANDO", 20, 5);

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
    ssd1306_draw_string(&ssd, "TEMPO RESTANTE", 8, 5);

    // Desenha uma linha horizontal
    ssd1306_hline(&ssd, 0, WIDTH - 1, 15, true);

    // Mostra instruções
    ssd1306_draw_string(&ssd, "A: Cancelar", 5, 46);
    ssd1306_draw_string(&ssd, "ALARM", 35, 55);
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
}