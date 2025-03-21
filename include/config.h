#ifndef CONFIG_H
#define CONFIG_H

// Configurações e definições do projeto (pinos, tempos, limites, etc.)

// definição dos pinos do LED RGB
static const uint LED_RED = 13;
static const uint LED_GREEN = 11;
static const uint LED_BLUE = 12;

// definição dos pinos dos botões
static const uint BUTTON_A = 5;
static const uint BUTTON_B = 6;

// definição dos pinos dos buzzer
static const uint Buzzer_A = 21;
static const uint Buzzer_B = 10;

// definição dos pinos do joystick
static const uint VRX_PIN = 27;
static const uint VRY_PIN = 26;

// inicialização dos pinos leds
static inline void leds_init()
{
    // inicializa os pinos dos LEDs
    gpio_init(LED_RED);
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);

    // define os pinos como saida
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
}

// inicialização dos pinos dos botões
static inline void buttons_init()
{
    // inicializa os pinos dos botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);

    // define os pinos como entrada
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);

    // define os pinos como pull-up
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
}

// inicialização do joystick
static inline void joystick_init()
{
    // inicializa os pinos do joystick como adc
    adc_gpio_init(VRX_PIN);
    adc_gpio_init(VRY_PIN);
}

#endif // CONFIG_H