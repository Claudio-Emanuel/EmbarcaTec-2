#include <stdio.h>
#include "pico/stdio.h"
#include "pico/cyw43_arch.h"
#include "pico/async_context.h"
#include "lwip/altcp_tls.h"
#include "example_http_client_util.h"
#include "hardware/adc.h"
// ======================== CONFIG======================================= //
#define HOST "192.168.2.104" // IP DA REDE
#define PORT 5000
#define INTERVALO_MS 500 //INTERVALO PARA ENVIAR MENSAGEM
#define BUTTON_A 5 //DEFINICAO DO BOTAO A"
#define BUTTON_B 6 //DEFINICAO DO BOTAO B
#define LED_RED 13 //DEFINICAO DO LED VERMELHO
#define LED_BLUE 12 //DEFINICAO DO LED_AZUL
#define JOY_X 26 //DEFINICAO DO EIXO X DO JOYSTICK
#define JOY_Y 27 //DEFINICAO DO EIXO Y DO JOYSTICK
#define DEADZONE 300 //VALOR DA ZONA MORTA DO ADC
// ====================================================================== //
// ========================INICIO DO MAIN================================ //
int main()
{
    // Inicializando entradas padrão
    stdio_init_all();

    // Inicializacao do conversor analogico
    adc_init();

    // Pinagem do ADC sem pulldown
    adc_gpio_init(JOY_X); // GPIO 26 como entrada do ADC
    adc_gpio_init(JOY_Y); // GPIO 27 como entrada do ADC 

// ==Configuração do Pino do Botão== //
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    
    // Conifguração do Led
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    printf("\nIniciando cliente HTTP...\n");

    if (cyw43_arch_init())
    {
        printf("Erro ao inicializar Wi-Fi\n");
        return 1;
    }
    cyw43_arch_enable_sta_mode();

    printf("Conectando a %s...\n", WIFI_SSID);
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD,
                                           CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Falha na conexão Wi-Fi\n");
        return 1;
    }

    printf("Conectado! IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

    // Loop Principal
    int button_state_a = 1; // Variável para controlar o estado do botão (1 = solto, 0 = pressionado)
    int button_state_b = 1; // Variável para controlar o estado do botão (1 = solto, 0 = pressionado)

    while (1) {
        adc_select_input(1); // Seleciona o canal 1 do ADC (GPIO 26)
        uint adc_x_raw= adc_read(); // Lê o valor do ADC

        adc_select_input(0); // Seleciona o canal 2 do ADC (GPIO 27)
        uint adc_y_raw= adc_read(); // Lê o valor do ADC
        int delta_x =  -(2048 - adc_x_raw) ;
        int delta_y = adc_y_raw - 2048;
        // Imprime os valores lidos do ADC
        const char *direction = NULL;

        // Verifica a direção do joystick
        if (abs(delta_x) < DEADZONE && abs(delta_y) > DEADZONE) {
            direction = (delta_y < 0) ? "/SUL" : "/NORTE";
        }else if (abs(delta_x) == DEADZONE && abs(delta_y) == DEADZONE) {
            direction = "/CENTRO";
        }else if (abs(delta_y) < DEADZONE && abs(delta_x) > DEADZONE) {
            direction = (delta_x > 0) ? "/LESTE" : "/OESTE";
        } else if (delta_x > DEADZONE && delta_y > -DEADZONE) {
            direction = "/NORDESTE";
        } else if (delta_x > DEADZONE && delta_y < DEADZONE) {
            direction = "/SUDESTE";
        } else if (delta_x < -DEADZONE && delta_y > -DEADZONE) {
            direction = "/NOROESTE";
        } else if (delta_x < -DEADZONE && delta_y < DEADZONE) {
            direction = "/SUDOESTE";
        }
        // Se a direção foi detectada, envia o comando    
        if (direction != NULL)
        {
            EXAMPLE_HTTP_REQUEST_T req = {0};
            req.hostname = HOST;
            req.url = direction;
            req.port = PORT;
            req.headers_fn = http_client_header_print_fn;
            req.recv_fn = http_client_receive_print_fn;

            printf("Enviando direção: %s\n", direction);
            int result = http_client_request_sync(cyw43_arch_async_context(), &req);

            if (result == 0)
            {
                printf("Direção enviada com sucesso!\n");
            }
            else
            {
                printf("Erro ao enviar direção: %d\n", result);
            }

            sleep_ms(20);
        }

        // Verifica o estado dos botões
        // Inicializa a variável path como NULL
        const char *path = NULL;

        // Se o botão A for apertado
        if (gpio_get(BUTTON_A) == 0)
        {
            if (button_state_a == 1)
            {                    // Se o botão estava solto e agora foi pressionado
                path = "/CLICK_A"; // Envia a mensagem "CLICK"
                gpio_put(LED_RED, 1);
                button_state_a = 0; // Atualiza o estado para pressionado
            }
        }
        else
        {
            if (button_state_a == 0)
            {                    // Se o botão estava pressionado e agora foi solto
                path = "/SOLTO_A"; // Envia a mensagem "SOLTO"
                gpio_put(LED_RED, 0);
                button_state_a = 1; // Atualiza o estado para solto
            }
        }
        if (gpio_get(BUTTON_B) == 0)
        {
            if (button_state_b == 1)
            {                    // Se o botão estava solto e agora foi pressionado
                path = "/CLICK_B"; // Envia a mensagem "CLICK"
                gpio_put(LED_BLUE, 1);
                button_state_b = 0; // Atualiza o estado para pressionado
            }
        }
        else
        {
            if (button_state_b == 0)
            {                    // Se o botão estava pressionado e agora foi solto
                path = "/SOLTO_B"; // Envia a mensagem "SOLTO"
                gpio_put(LED_BLUE, 0);
                button_state_b = 1; // Atualiza o estado para solto
            }
        }

        if (path != NULL)
        {
            EXAMPLE_HTTP_REQUEST_T req = {0};
            req.hostname = HOST;
            req.url = path;
            req.port = PORT;
            req.headers_fn = http_client_header_print_fn;
            req.recv_fn = http_client_receive_print_fn;

            printf("Enviando comando: %s\n", path);
            int result = http_client_request_sync(cyw43_arch_async_context(), &req);

            if (result == 0)
            {
                printf("Comando enviado com sucesso!\n");
            }
            else
            {
                printf("Erro ao enviar comando: %d\n", result);
            }

            sleep_ms(20); // Evita múltiplos envios rápidos
        }

        sleep_ms(INTERVALO_MS);
    }
}
