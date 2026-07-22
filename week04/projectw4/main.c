/*********Task 3*********************
#include "stm32f1xx_hal.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void SysTick_Handler(void)
{
    HAL_IncTick();
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
        HAL_Delay(500); // 500ms Sáng, 500ms Tắt
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Bật Clock cho Port C
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Cấu hình LED PC13 làm Output Push-Pull
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void SystemClock_Config(void) {}
void Error_Handler(void) { while(1); }

/***************Task 4***************
//Cách mắc: chân nút nối GND với chân PA0(tức là A0 trên blue pill)

#include "stm32f1xx_hal.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void SysTick_Handler(void)
{
    HAL_IncTick();
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    uint8_t last_button_state = GPIO_PIN_SET; 
    uint32_t last_debounce_time = 0;
    uint8_t stable_button_state = GPIO_PIN_SET;

    while (1)
    {
        uint32_t current_time = HAL_GetTick();
        uint8_t raw_button_reading = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        
        // Nếu trạng thái thay đổi (do nhiễu hoặc nhấn thật), reset bộ đếm thời gian
        if (raw_button_reading != last_button_state)
        {
            last_debounce_time = current_time;
        }
        last_button_state = raw_button_reading;

        // Nếu trạng thái giữ ổn định lâu hơn 50ms
        if ((current_time - last_debounce_time) > 50)
        {
            // Bắt cạnh lên (Thả nút nhấn ra: Chuyển từ LOW -> HIGH)
            if (stable_button_state == GPIO_PIN_RESET && raw_button_reading == GPIO_PIN_SET)
            {
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13); // Đảo trạng thái LED PC13
            }
            stable_button_state = raw_button_reading;
        }
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Bật Clock cho Port A và Port C
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Cấu hình Output: LED PC13
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Cấu hình Input Pull-up: Nút nhấn PA0
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SystemClock_Config(void) {}
void Error_Handler(void) { while(1); }
//**************Task 5 Cách 2****************
// //Cách mắc: 4 chân dương led dô A8-A11, chân âm dô gnd
// //Ma trận led: A1,A2 là output, A3, A4 là input.
// #include "stm32f1xx_hal.h"

// void SystemClock_Config(void);
// static void MX_GPIO_Init(void);

// void SysTick_Handler(void)
// {
//     HAL_IncTick();
// }

// int main(void)
// {
//     HAL_Init();
//     SystemClock_Config();
//     MX_GPIO_Init();

//     while (1)
//     {
//         uint8_t btn1 = 0, btn2 = 0, btn3 = 0, btn4 = 0;

//         // Quét Hàng 1 (Kéo PA1 xuống 0, PA2 lên 1) 
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
        
//         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) { btn1 = 1; } // Phím (1,1) -> LED 1
//         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) { btn2 = 1; } // Phím (1,2) -> LED 2

//         //Quét Hàng 2 (Kéo PA1 lên 1, PA2 xuống 0) 
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
        
//         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == GPIO_PIN_RESET) { btn3 = 1; } // Phím (2,1) -> LED 3
//         if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) { btn4 = 1; } // Phím (2,2) -> LED 4

//         //Điều khiển 4 LED (PA8 - PA11) 
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  btn1 ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 1
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9,  btn2 ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 2
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, btn3 ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 3
//         HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, btn4 ? GPIO_PIN_SET : GPIO_PIN_RESET); // LED 4

//         HAL_Delay(20); // Anti-bounce phím ma trận
//     }
// }

// static void MX_GPIO_Init(void)
// {
//     GPIO_InitTypeDef GPIO_InitStruct = {0};

//     // 1. Bật Clock Port A
//     __HAL_RCC_GPIOA_CLK_ENABLE();

    
//     // 2. Cấu hình OUTPUT: 
//     // - PA1, PA2: Mặc định SET (1) cho quét ma trận
//     // - PA8, PA9, PA10, PA11: Mặc định RESET (0) để TẮT 4 LED
//     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1 | GPIO_PIN_2, GPIO_PIN_SET);
//     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11, GPIO_PIN_RESET);

//     GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
//     GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//     GPIO_InitStruct.Pull = GPIO_NOPULL;
//     GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//     // 3. Cấu hình INPUT:
//     // - PA3, PA4: Cột 1 và Cột 2 (Input Pull-up)
//     GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
//     GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//     GPIO_InitStruct.Pull = GPIO_PULLUP;
//     HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
// }

// void SystemClock_Config(void) {}
// void Error_Handler(void) { while(1); }