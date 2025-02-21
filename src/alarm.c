#include "include/alarm.h"
#include "include/ssd1306/interface.h"

// Variaveis globais
Alarmstate current_sate = STATE_IDLE;
Alarmconfig alarm = {0};

// inicializa o alarme
void alarm_init()
{
    alarm.hours = 0;
    alarm.minutes = 0;
    alarm.seconds = 0;
    alarm.total_seconds = 0;
    alarm.active = false;

    // iniciar no estado ocioso
    change_sate(STATE_IDLE);
}

// define o tempo do alarme
void alarm_set_time(uint16_t hours, uint16_t minutes, uint16_t seconds)
{
    alarm.hours = hours;
    alarm.minutes = minutes;
    alarm.seconds = seconds;
    alarm.total_seconds = (hours * 3600 ) + (minutes * 60) + seconds;
}

// inicia contagem regressiva
void alarm_start()
{
    if(alarm.total_seconds > 0)
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

// verifica se o alarm está ativo
bool alarm_is_active()
{
    return alarm.active;
}

// obtém o tempo restante em segundos
uint32_t alarm_get_tempo_restante()
{
    if(!alarm.active)
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
void alarm_active()
{
    alarm.active = false;
    change_sate(STATE_ALARM);
}
