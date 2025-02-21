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