#ifndef interface_h
#define interface_h

// Protótipos das funções
void displayssd1306_init();
void display_show_idle_screen();
void display_update_program_values(uint16_t hours, uint16_t minutes, uint16_t seconds, uint8_t selection);
void display_show_countdown_screen();
void display_update_countdown(uint16_t hours, uint16_t minutes, uint16_t seconds);
void display_show_alarm_screen();

#endif // interface_h