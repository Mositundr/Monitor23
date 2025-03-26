/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "gpio.h"
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RST_PIN GPIO_PIN_2
#define DC_PIN GPIO_PIN_3
#define CS_PIN GPIO_PIN_4
#define RST_PORT GPIOA
#define DC_PORT GPIOA
#define CS_PORT GPIOA

// Команды для SSD1309
#define SSD1309_SETCONTRAST 0x81
#define SSD1309_DISPLAYALLON_RESUME 0xA4
#define SSD1309_DISPLAYON 0xAF
#define SSD1309_DISPLAYOFF 0xAE
#define SSD1309_NORMALDISPLAY 0xA6
#define SSD1309_SETSTARTLINE 0x40
#define SSD1309_MEMORYMODE 0x20
#define SSD1309_COLUMNADDR 0x21
#define SSD1309_PAGEADDR 0x22
#define SSD1309_SEGREMAP 0xA0  // Нормальная ориентация 
#define SSD1309_COMSCANINC 0xC0  // Нормальная ориентация 
#define SSD1309_SETCOMPINS 0xDA
#define SSD1309_SETVCOMDETECT 0xDB
#define SSD1309_SETMULTIPLEX 0xA8
#define SSD1309_SETDISPLAYOFFSET 0xD3
#define SSD1309_SETDISPLAYCLOCKDIV 0xD5
#define SSD1309_SETPRECHARGE 0xD9
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
// Массив шрифта
const unsigned char font_regular[94][6] = 
{
    {0x00, 0x60, 0x60, 0x60, 0x00, 0x00},   // sp
    {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00},   // !
    {0x00, 0x00, 0x07, 0x00, 0x07, 0x00},   // "
    {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
    {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
    {0x00, 0x62, 0x64, 0x08, 0x13, 0x23},   // %
    {0x00, 0x36, 0x49, 0x55, 0x22, 0x50},   // &
    {0x00, 0x00, 0x05, 0x03, 0x00, 0x00},   // '
    {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00},   // (
    {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00},   // )
    {0x00, 0x14, 0x08, 0x3E, 0x08, 0x14},   // *
    {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08},   // +
    {0x00, 0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
    {0x00, 0x08, 0x08, 0x08, 0x08, 0x08},   // -
    {0x00, 0x00, 0x60, 0x60, 0x00, 0x00},   // .
    {0x00, 0x20, 0x10, 0x08, 0x04, 0x02},   // /
    {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
    {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
    {0x00, 0x42, 0x61, 0x51, 0x49, 0x46},   // 2
    {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
    {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
    {0x00, 0x27, 0x45, 0x45, 0x45, 0x39},   // 5
    {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
    {0x00, 0x01, 0x71, 0x09, 0x05, 0x03},   // 7
    {0x00, 0x36, 0x49, 0x49, 0x49, 0x36},   // 8
    {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
    {0x00, 0x00, 0x36, 0x36, 0x00, 0x00},   // :
    {0x00, 0x00, 0x56, 0x36, 0x00, 0x00},   // ;
    {0x00, 0x08, 0x14, 0x22, 0x41, 0x00},   // <
    {0x00, 0x14, 0x14, 0x14, 0x14, 0x14},   // =
    {0x00, 0x00, 0x41, 0x22, 0x14, 0x08},   // >
    {0x00, 0x02, 0x01, 0x51, 0x09, 0x06},   // ?
    {0x00, 0x32, 0x49, 0x59, 0x51, 0x3E},   // @
    {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
    {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36},   // B
    {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22},   // C
    {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
    {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41},   // E
    {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01},   // F
    {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
    {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
    {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00},   // I
    {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01},   // J
    {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41},   // K
    {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40},   // L
    {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
    {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
    {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
    {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06},   // P
    {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
    {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46},   // R
    {0x00, 0x46, 0x49, 0x49, 0x49, 0x31},   // S
    {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01},   // T
    {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
    {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
    {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
    {0x00, 0x63, 0x14, 0x08, 0x14, 0x63},   // X
    {0x00, 0x07, 0x08, 0x70, 0x08, 0x07},   // Y
    {0x00, 0x61, 0x51, 0x49, 0x45, 0x43},   // Z
    {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00},   // [
    {0x00, 0x02, 0x04, 0x08, 0x10, 0x20},   // \
    {0x00, 0x00, 0x06, 0x09, 0x09, 0x06},   // ] 
    {0x00, 0x00, 0x06, 0x09, 0x09, 0x06},   // ^ Градус )
    {0x00, 0x40, 0x40, 0x40, 0x40, 0x40},   // _
    {0x00, 0x00, 0x01, 0x02, 0x04, 0x00},   // '
    {0x00, 0x20, 0x54, 0x54, 0x54, 0x78},   // a
    {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38},   // b
    {0x00, 0x38, 0x44, 0x44, 0x44, 0x20},   // c
    {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F},   // d
    {0x00, 0x38, 0x54, 0x54, 0x54, 0x18},   // e
    {0x00, 0x08, 0x7E, 0x09, 0x01, 0x02},   // f
    {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g
    {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78},   // h
    {0x00, 0x00, 0x44, 0x7D, 0x40, 0x00},   // i
    {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00},   // j
    {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00},   // k
    {0x00, 0x00, 0x41, 0x7F, 0x40, 0x00},   // l
    {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78},   // m
    {0x00, 0x7C, 0x08, 0x04, 0x04, 0x78},   // n
    {0x00, 0x38, 0x44, 0x44, 0x44, 0x38},   // o
    {0x00, 0xFC, 0x24, 0x24, 0x24, 0x18},   // p
    {0x00, 0x18, 0x24, 0x24, 0x18, 0xFC},   // q
    {0x00, 0x7C, 0x08, 0x04, 0x04, 0x08},   // r
    {0x00, 0x48, 0x54, 0x54, 0x54, 0x20},   // s
    {0x00, 0x04, 0x3F, 0x44, 0x40, 0x20},   // t
    {0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
    {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C},   // v
    {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C},   // w
    {0x00, 0x44, 0x28, 0x10, 0x28, 0x44},   // x
    {0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y
    {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44},   // z
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void SSD1309_Reset(void);                     // Сброс дисплея
void SSD1309_WriteCommand(uint8_t command);   // Отправка команды
void SSD1309_WriteData(uint8_t data);         // Отправка данных
void SSD1309_Init(void);                      // Инициализация дисплея
void SSD1309_SetPosition(uint8_t x, uint8_t y); // Установка позиции (столбец, страница)
void SSD1309_Clear(void);                     // Очистка экрана
void SSD1309_DrawChar(uint8_t x, uint8_t y, char character); // Отрисовка символа
void SSD1309_DrawString(uint8_t x, uint8_t y, const char *str); // Отрисовка строки
void SSD1309_DisplayTemperature(int temp); // Отображение температуры
void SSD1309_DrawBigChar(uint8_t x, uint8_t y, uint8_t index); // Большие символы
void SSD1309_DrawBigString(uint8_t x, uint8_t y, const char *str); // Строка Больших симовлов
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  // Инициализация дисплея
  SSD1309_Init();
  
  // Очистка экрана
  SSD1309_Clear();
  
  int temperature = 23; // Чтение температуры
  SSD1309_DisplayTemperature(temperature); // Отображение температуры

  SSD1309_DrawBigString(40, 2, "23C" ); //вывод больших цифр
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI, RCC_MCODIV_1);
}

/* USER CODE BEGIN 4 */

// Сброс дисплея
void SSD1309_Reset(void) {
  HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
  HAL_Delay(100); // Задержка 100 мс для надёжного сброса
  HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

// Отправка команды на дисплей
void SSD1309_WriteCommand(uint8_t command) {
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET); // DC = 0 для команд
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); // CS = 0 для выбора чипа
  HAL_SPI_Transmit(&hspi1, &command, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);   // CS = 1 после передачи
}

// Отправка данных на дисплей
void SSD1309_WriteData(uint8_t data) {
  HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);   // DC = 1 для данных
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); // CS = 0 для выбора чипа
  HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);   // CS = 1 после передачи
}

// Инициализация дисплея SSD1309
void SSD1309_Init(void) {
  SSD1309_Reset(); // Сброс дисплея

  SSD1309_WriteCommand(SSD1309_DISPLAYOFF);           // Выключить дисплей
  SSD1309_WriteCommand(SSD1309_SETDISPLAYCLOCKDIV);   // Частота обновления
  SSD1309_WriteCommand(0x80);                         // Значение по умолчанию
  SSD1309_WriteCommand(SSD1309_SETMULTIPLEX);         // Установить мультиплексор
  SSD1309_WriteCommand(0x3F);                         // 64 строки (0-63)
  SSD1309_WriteCommand(SSD1309_SETDISPLAYOFFSET);     // Смещение дисплея
  SSD1309_WriteCommand(0x00);                         // Без смещения
  SSD1309_WriteCommand(SSD1309_SETSTARTLINE | 0x00);  // Начальная строка
  SSD1309_WriteCommand(0x8D);                         // Включение charge pump
  SSD1309_WriteCommand(0x14);                         // Включить
  SSD1309_WriteCommand(SSD1309_MEMORYMODE);           // Режим памяти
  SSD1309_WriteCommand(0x00);                         // Горизонтальная адресация
  SSD1309_WriteCommand(SSD1309_SEGREMAP);             // Сегменты отображаются в обратном порядке
  SSD1309_WriteCommand(SSD1309_COMSCANINC);           // Сканирование сверху вниз
  SSD1309_WriteCommand(SSD1309_SETCOMPINS);           // Настройка COM пинов
  SSD1309_WriteCommand(0x12);                         // Значение по умолчанию
  SSD1309_WriteCommand(SSD1309_SETCONTRAST);          // Контрастность
  SSD1309_WriteCommand(0xCF);                         // Установить контраст
  SSD1309_WriteCommand(SSD1309_SETPRECHARGE);         // Предзарядка
  SSD1309_WriteCommand(0xF1);                         // Значение по умолчанию
  SSD1309_WriteCommand(SSD1309_SETVCOMDETECT);        // Уровень VCOM
  SSD1309_WriteCommand(0x40);                         // Значение по умолчанию
  SSD1309_WriteCommand(SSD1309_DISPLAYALLON_RESUME);  // Нормальный режим
  SSD1309_WriteCommand(SSD1309_NORMALDISPLAY);        // Нормальное отображение
  SSD1309_WriteCommand(SSD1309_DISPLAYON);            // Включить дисплей
}

// Установка позиции для записи (x — столбец, y — страница)
void SSD1309_SetPosition(uint8_t x, uint8_t y) {
  SSD1309_WriteCommand(SSD1309_COLUMNADDR); // Установить диапазон столбцов
  SSD1309_WriteCommand(x);                  // Начальный столбец
  SSD1309_WriteCommand(127);                // Конечный столбец (127 — максимум)
  SSD1309_WriteCommand(SSD1309_PAGEADDR);   // Установить диапазон страниц
  SSD1309_WriteCommand(y);                  // Начальная страница
  SSD1309_WriteCommand(7);                  // Конечная страница (7 — максимум)
}

// Очистка экрана (заполнение нулями)
void SSD1309_Clear(void) {
  SSD1309_SetPosition(0, 0); // Начало с (0, 0)
  for (uint16_t i = 0; i < 128 * 8; i++) { // 128 столбцов * 8 страниц
      SSD1309_WriteData(0x00); // Заполнить нули
  }
}

// Отрисовка одного символа
void SSD1309_DrawChar(uint8_t x, uint8_t y, char character) {
  uint8_t index;
  if (character >= ' ' && character <= '~') {
      index = character - ' ';
  } else {
      return; // Недопустимый символ
  }
  for (uint8_t col = 0; col < 6; col++) {
      uint8_t data = font_regular[index][col];
      SSD1309_SetPosition(x + col, y);
      SSD1309_WriteData(data);
  }
}

// Отрисовка строки
void SSD1309_DrawString(uint8_t x, uint8_t y, const char *str) {
  while (*str) {
      SSD1309_DrawChar(x, y, *str);
      x += 6; // Сдвигаемся на следующий символ (ширина символа = 6 пикселей)
      str++;
  }
}

void SSD1309_DisplayTemperature(int temp) {
  char buffer[10];
  if (temp < 0) {
      SSD1309_DrawChar(0, 2, '-'); // Отрисовка минуса
      temp = -temp; // Преобразуем температуру в положительное число
  }
  sprintf(buffer, "%d", temp); // Преобразуем температуру в строку

  // Отображение температуры обычным шрифтом
  SSD1309_DrawString(0, 0, "ZNP"); // Выводим слово "TEMP"
  SSD1309_DrawString(0, 2, buffer); // Выводим значение температуры
  SSD1309_DrawChar(6 * strlen(buffer), 2, 93); // Символ градуса
  SSD1309_DrawChar(6 * strlen(buffer) + 6, 2, 'C'); // Символ 'C'
}

void SSD1309_DrawBigChar(uint8_t x, uint8_t y, uint8_t index) {
    const uint8_t segments[11][7] = {
        {1,1,1,1,1,1,0}, // 0
        {0,1,1,0,0,0,0}, // 1
        {1,1,0,1,1,0,1}, // 2
        {1,1,1,1,0,0,1}, // 3
        {0,1,1,0,0,1,1}, // 4
        {1,0,1,1,0,1,1}, // 5
        {1,0,1,1,1,1,1}, // 6
        {1,1,1,0,0,0,0}, // 7
        {1,1,1,1,1,1,1}, // 8
        {1,1,1,1,0,1,1}, // 9
        {1,0,0,1,1,1,0}  // C
    };

    // Очищаем область перед отрисовкой
    for(uint8_t page = y; page < y + 6; page++) {
        SSD1309_SetPosition(x, page);
        for(uint8_t i = 0; i < 20; i++) {
            SSD1309_WriteData(0x00);
        }
    }
    HAL_Delay(100); // Задержка после очистки

    // Рисуем горизонтальные сегменты (a, g, d)
    if(segments[index][0]) { // Сегмент a (верх)
        SSD1309_SetPosition(x + 3, y);
        for(uint8_t j = 0; j < 14; j++) {
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки верхнего сегмента
    }

    if(segments[index][6]) { // Сегмент g (середина)
        SSD1309_SetPosition(x + 3, y + 2);
        for(uint8_t j = 0; j < 14; j++) {
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки среднего сегмента
    }

    if(segments[index][3]) { // Сегмент d (низ)
        SSD1309_SetPosition(x + 3, y + 4);
        for(uint8_t j = 0; j < 14; j++) {
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки нижнего сегмента
    }

    // Рисуем вертикальные сегменты (f, b, e, c)
    if(segments[index][5]) { // Сегмент f (лево верх)
        for(uint8_t i = 0; i < 3; i++) {
            SSD1309_SetPosition(x, y + i);
            SSD1309_WriteData(0xFF);
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки левого верхнего сегмента
    }

    if(segments[index][1]) { // Сегмент b (право верх)
        for(uint8_t i = 0; i < 3; i++) {
            SSD1309_SetPosition(x + 17, y + i);
            SSD1309_WriteData(0xFF);
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки правого верхнего сегмента
    }

    if(segments[index][4]) { // Сегмент e (лево низ)
        for(uint8_t i = 2; i < 5; i++) {
            SSD1309_SetPosition(x, y + i);
            SSD1309_WriteData(0xFF);
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки левого нижнего сегмента
    }

    if(segments[index][2]) { // Сегмент c (право низ)
        for(uint8_t i = 2; i < 5; i++) {
            SSD1309_SetPosition(x + 17, y + i);
            SSD1309_WriteData(0xFF);
            SSD1309_WriteData(0xFF);
        }
        HAL_Delay(50); // Задержка после отрисовки правого нижнего сегмента
    }
}

void SSD1309_DrawBigString(uint8_t x, uint8_t y, const char *str) {
    while (*str) {
        uint8_t index;
        if (*str >= '0' && *str <= '9') {
            index = *str - '0';
        } else if (*str == 'C') {
            index = 10; // Индекс для символа 'C' в массиве Proba20x34
        } else {
            str++;
            continue;
        }
        SSD1309_DrawBigChar(x, y, index);
        x += 20; // Ширина символа 20 пикселей
        str++;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
