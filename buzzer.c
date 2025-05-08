#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

//Definição do pino do Buzzer
#define buzz1 21

// Configuração da função Buzzer para alerta
void buzzer_alert_tone() {
    gpio_set_function(buzz1, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(buzz1);

    pwm_set_wrap(slice, 6249); // Para gerar 2 kHz com clock padrão
    pwm_set_chan_level(slice, PWM_CHAN_A, 3125); // 50% de duty cycle
    pwm_set_enabled(slice, true);

    sleep_ms(200); // Dura 200 ms

    pwm_set_enabled(slice, false);
    sleep_ms(100); // Pausa
}
