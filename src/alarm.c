#include "include/alarm.h"
#include "include/ssd1306/interface.h"

// variáveis globais
Alarmstate current_state = STATE_IDLE;
Alarmconfig alarm = {0};

// inicializa o alarme
void alarm_init()
{
    alarm.hours = 0;
    alarm.minutes = 0;
    alarm.seconds = 0;
    alarm.total_seconds = 0;
    alarm.active = false;

    // inicia no estado ocioso
    change_state(STATE_IDLE);
}

// define o tempo do alarme
void alarm_set_time(uint16_t hours, uint16_t minutes, uint16_t seconds)
{
    alarm.hours = hours;
    alarm.minutes = minutes;
    alarm.seconds = seconds;
    alarm.total_seconds = (hours * 3600) + (minutes * 60) + seconds;
}

// inicia a contagem regressiva
void alarm_start()
{
    if (alarm.total_seconds > 0)
    {
        alarm.start_time = time_us_32() / 1000000; // converte para segundos
        alarm.active = true;
        change_state(STATE_COUNTDOWN);
    }
}

// para a contagem regressiva
void alarm_stop()
{
    alarm.active = false;
}

// reinicia o alarme
void alarm_reset()
{
    alarm.active = false;
    change_state(STATE_IDLE);
}

// verifica se o alarme está ativo
bool alarm_is_active()
{
    return alarm.active;
}

// obtém o tempo restante em segundos
uint32_t alarm_get_tempo_restante()
{
    if (!alarm.active)
    {
        return alarm.total_seconds;
    }

    uint32_t elapsed = (time_us_32() / 1000000) - alarm.start_time;

    if (elapsed >= alarm.total_seconds)
    {
        return 0;
    }

    return alarm.total_seconds - elapsed;
}

// atualiza o alarme
void alarm_update()
{
    if (alarm.active)
    {
        uint32_t remaining = alarm_get_tempo_restante();

        if (remaining == 0)
        {
            alarm_activate();
        }
    }
}

// ativa o alarme
void alarm_activate()
{
    alarm.active = false;
    change_state(STATE_ALARM);
}

// função para transição de estado
void change_state(Alarmstate new_state)
{
    // executa função de saída do estado atual
    switch (current_state)
    {
    case STATE_IDLE:
        state_idle_exit();
        break;
    case STATE_PROGRAM:
        state_program_exit();
        break;
    case STATE_COUNTDOWN:
        state_countdown_exit();
        break;
    case STATE_ALARM:
        state_alarm_exit();
        break;
    }

    // atualiza o estado atual
    current_state = new_state;

    // executa função de entrada do novo estado
    switch (current_state)
    {
    case STATE_IDLE:
        state_idle_enter();
        break;
    case STATE_PROGRAM:
        state_program_enter();
        break;
    case STATE_COUNTDOWN:
        state_countdown_enter();
        break;
    case STATE_ALARM:
        state_alarm_enter();
        break;
    }
}

// implementação das funções de estado
void state_idle_enter()
{
    // limpa o display e mostra a tela inicial
    display_show_idle_screen();
}

void state_idle_update()
{
    // verifica se o botão a foi pressionado para entrar no modo de programação
    if (!gpio_get(BUTTON_A))
    {
        change_state(STATE_PROGRAM);
        sleep_ms(200); // debounce simples
    }
}

void state_idle_exit()
{
    // nada a fazer ao sair do estado ocioso
}

void state_program_enter()
{
    // inicializa os valores para programação
    alarm.hours = 0;
    alarm.minutes = 0;
    alarm.seconds = 0;
    display_show_program_screen();
}

void state_program_update()
{
    static uint8_t selection = 0; // 0=horas, 1=minutos, 2=segundos
    static absolute_time_t last_update = {0};
    static bool joystick_triggered = false; // indica se o ajuste já foi disparado

    // definição da zona morta com histerese
    const uint16_t DEADZONE_LOW = 1900;   // pode sofrer ajustes conforme necessário futuramente
    const uint16_t DEADZONE_HIGH = 2100;

    if (absolute_time_diff_us(last_update, get_absolute_time()) < 100000)
    {
        return;
    }
    last_update = get_absolute_time();

    // lê o valor do eixo vertical do joystick (vry)
    adc_select_input(0); // vry_pin está em adc canal 0
    uint16_t y_value = adc_read();

    // printf("y value: %d\n", y_value);

    if (!gpio_get(SW_PIN))
    {
        selection = (selection + 1) % 3;
        sleep_ms(200); // debounce
    }

    // se o joystick estiver centralizado, reseta o disparo
    if (y_value >= DEADZONE_LOW && y_value <= DEADZONE_HIGH) {
        joystick_triggered = false;
    } 
    else if (!joystick_triggered) {
        joystick_triggered = true;
        if (y_value < DEADZONE_LOW) {
            // movimento para cima: decrementa
            switch (selection)
            {
            case 0:
                alarm.hours = (alarm.hours + 23) % 24;
                break;
            case 1:
                alarm.minutes = (alarm.minutes + 59) % 60;
                break;
            case 2:
                alarm.seconds = (alarm.seconds + 59) % 60;
                break;
            }
        }
        else if (y_value > DEADZONE_HIGH) {
            // movimento para baixo: incrementa
            switch (selection)
            {
            case 0:
                alarm.hours = (alarm.hours + 1) % 24;
                break;
            case 1:
                alarm.minutes = (alarm.minutes + 1) % 60;
                break;
            case 2:
                alarm.seconds = (alarm.seconds + 1) % 60;
                break;
            }
        }
        sleep_ms(200); // debounce adicional
    }

    display_update_program_values(alarm.hours, alarm.minutes, alarm.seconds, selection);

    if (!gpio_get(BUTTON_B))
    {
        alarm.total_seconds = (alarm.hours * 3600) + (alarm.minutes * 60) + alarm.seconds;
        if (alarm.total_seconds > 0)
        {
            change_state(STATE_COUNTDOWN);
        }
        sleep_ms(200);
    }

    if (!gpio_get(BUTTON_A))
    {
        change_state(STATE_IDLE);
        sleep_ms(200);
    }
}

void state_program_exit()
{
    // nada a fazer ao sair do estado de programação
}

void state_countdown_enter()
{
    alarm.start_time = time_us_32() / 1000000; // converte para segundos
    alarm.active = true;
    display_show_countdown_screen();
}

void state_countdown_update()
{
    static absolute_time_t last_update = {0};

    // atualiza o display a cada 1 segundo
    if (absolute_time_diff_us(last_update, get_absolute_time()) >= 1000000)
    {
        last_update = get_absolute_time();

        // calcula o tempo restante
        uint32_t remaining = alarm_get_tempo_restante();

        // atualiza o display com o tempo restante
        uint16_t h = remaining / 3600;
        uint16_t m = (remaining % 3600) / 60;
        uint16_t s = remaining % 60;

        display_update_countdown(h, m, s);

        // verifica se o tempo acabou
        if (remaining == 0)
        {
            alarm_activate();
        }
    }

    // verifica se o botão a foi pressionado para cancelar a contagem
    if (!gpio_get(BUTTON_A))
    {
        alarm_reset();
        sleep_ms(200); // debounce
    }
}

void state_countdown_exit()
{
    // nada a fazer ao sair do estado de contagem regressiva
}

void state_alarm_enter()
{
    // configura pwm para controlar o buzzer
    gpio_set_function(Buzzer_A, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(Buzzer_A);
    pwm_set_wrap(slice_num, 1000);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, 500);
    pwm_set_enabled(slice_num, true);

    display_show_alarm_screen();
}

void state_alarm_update()
{
    static absolute_time_t last_led_update = {0};
    static bool led_state = false;

    // pisca o led vermelho a cada 500ms
    if (absolute_time_diff_us(last_led_update, get_absolute_time()) >= 500000)
    {
        last_led_update = get_absolute_time();
        led_state = !led_state;
        gpio_put(LED_RED, led_state);
    }

    // verifica se o botão a foi pressionado para confirmar o alarme
    if (!gpio_get(BUTTON_A))
    {
        // desliga o buzzer
        gpio_set_function(Buzzer_A, GPIO_FUNC_SIO);
        gpio_put(Buzzer_A, 0);

        // desliga o led vermelho
        gpio_put(LED_RED, 0);

        // volta para o estado ocioso
        alarm_reset();
        sleep_ms(200); // debounce
    }
}

void state_alarm_exit()
{
    // desliga o buzzer
    gpio_set_function(Buzzer_A, GPIO_FUNC_SIO);
    gpio_put(Buzzer_A, 0);

    // desliga o led vermelho
    gpio_put(LED_RED, 0);
}