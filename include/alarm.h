#ifndef ALARM_H
#define ALARM_H

#include "include/lib/bibliotecas.h"

// definição dos estados do sistema
typedef enum
{
    STATE_IDLE, // estado inicial do programa
    STATE_PROGRAM, // programação do alarm
    STATE_COUNTDOWN, // contagem regresiva
    STATE_ALARM // Alarme ativado
} Alarmstate;

// estrutura para armazenar a configuração do alarme:
typedef struct
{
    uint16_t hours;
    uint16_t minutes;
    uint16_t seconds;
    uint32_t total_seconds; // tempo total em segundos
    uint32_t start_time; // "carimbo" do inicio de contagem
    bool active; // indica se o alarme esta ativo
} Alarmconfig;

// Prototipos das funções
void alarm_init();
void alarm_set_time(uint16_t hours, uint16_t minutes, uint16_t seconds);
void alarm_start();
void alarm_stop();
void alarm_reset();
bool alarm_is_active();
uint32_t alarm_get_tempo_restante();
void alarm_update();
void alarm_activade();

// Variaveis globais:
extern Alarmstate current_state;
extern Alarmconfig alarm;

#endif