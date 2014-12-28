#include <mios32.h>
#include "app.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#define PRIORITY_TASK_MATRIX_SCAN	( tskIDLE_PRIORITY + 2 )

#define KEYBOARD_MIDI_PORT USB0
#define KEYBOARD_MIDI_CHN Chn1

static void TASK_MatrixScan(void *pvParameters);

#define NUM_COLS 2
#define NUM_ROWS 4
#define NUM_LEDS 8
#define LED_PIN (NUM_ROWS + NUM_COLS)
u8 button_state[NUM_COLS]; // if more than 8 rows u8 will need to become u16
u8 led_state[NUM_LEDS][3]; // GRB

void set_led_din(u8 a) {
    MIOS32_BOARD_J10_PinSet(LED_PIN, a);
}

/* J10A
  { GPIOE, GPIO_Pin_8 },
  { GPIOE, GPIO_Pin_9 },
  { GPIOE, GPIO_Pin_10 },
  { GPIOE, GPIO_Pin_11 },
  { GPIOE, GPIO_Pin_12 },
  { GPIOE, GPIO_Pin_13 },
  { GPIOE, GPIO_Pin_14 },
  { GPIOE, GPIO_Pin_15 },

  // J10B
  { GPIOC, GPIO_Pin_13 },
  { GPIOC, GPIO_Pin_14 },
  { GPIOC, GPIO_Pin_15 },
  { GPIOE, GPIO_Pin_2 }, // PE3 is set to 1 by the MIOS32_SPI driver to ensure that the MEMs chip of the DISCOVERY board doesn't drive the SPI lines
  { GPIOE, GPIO_Pin_4 },
  { GPIOE, GPIO_Pin_5 },
  { GPIOE, GPIO_Pin_6 },
  { GPIOE, GPIO_Pin_7 },
*/
void APP_Init(void)
{
  MIOS32_BOARD_LED_Init(0xffffffff);

  int pin;
  for (pin = 0; pin < NUM_COLS; pin++) {
    MIOS32_BOARD_J10_PinInit(pin, MIOS32_BOARD_PIN_MODE_OUTPUT_PP);
  }
  for (pin = NUM_COLS; pin < (NUM_COLS + NUM_ROWS); pin++) {
    MIOS32_BOARD_J10_PinInit(pin, MIOS32_BOARD_PIN_MODE_INPUT_PD);
  }
  MIOS32_BOARD_J10_PinInit(LED_PIN, MIOS32_BOARD_PIN_MODE_OUTPUT_PP);

  int column;
  for (column = 0; column < NUM_COLS; column++) {
    button_state[column] = 0;
  }

  int i;
  for (i = 0; i < NUM_LEDS; i++) {
    led_state[i][0] = 0; 
    led_state[i][1] = 0;
    led_state[i][2] = 0;
  }

  xTaskCreate(TASK_MatrixScan, (signed portCHAR *)"MatrixScan", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK_MATRIX_SCAN, NULL);
}

void APP_Background(void)
{
  #if 0
  // endless loop
  while( 1 ) {
    // toggle the state of all LEDs (allows to measure the execution speed with a scope)
    MIOS32_BOARD_LED_Set(0xffffffff, ~MIOS32_BOARD_LED_Get());

  }
  #endif
}

void APP_MIDI_NotifyPackage(mios32_midi_port_t port, mios32_midi_package_t midi_package)
{
  //MIOS32_MIDI_SendDebugMessage("hello %d\n",42);
}

inline void ws2812_low() {
  set_led_din(1);
asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"); // numNops = 41

  set_led_din(0);
asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"); // numNops = 98
}

inline void ws2812_high() {
  set_led_din(1);
asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"); // numNops = 105

  set_led_din(0);
asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\t"); // numNops = 30
}

inline void ws2812_shiftOut(u8 a) {
  int i;
  for (i = 7; i >= 0; i--) {
    if (a & (1 << i)) {
      ws2812_high();
    } else {
      ws2812_low();
    }
  }
}

void ledTask() {
  int i;
  for (i = 0; i < NUM_LEDS; i++) {
    ws2812_shiftOut(led_state[i][0]);
    ws2812_shiftOut(led_state[i][1]);
    ws2812_shiftOut(led_state[i][2]);
  }
  set_led_din(0);
  //MIOS32_BOARD_LED_Set(0xffffffff, ~MIOS32_BOARD_LED_Get());
}

void BUTTON_NotifyToggle(u8 row, u8 column, u8 pin_value)
{
  int note_number = 0x15 + (4 * column) + row;

  if( pin_value ) {
    // determine velocity depending on delay
    int velocity = 127;

    MIOS32_MIDI_SendNoteOn(KEYBOARD_MIDI_PORT, KEYBOARD_MIDI_CHN, note_number, velocity);
  } else {
    // send Note On with velocity 0
    MIOS32_MIDI_SendNoteOn(KEYBOARD_MIDI_PORT, KEYBOARD_MIDI_CHN, note_number, 0x00);
  }
}

void setColumns(u8 value) {
  int i;
  for (i = 0; i < NUM_COLS; i++) {
    MIOS32_BOARD_J10_PinSet(i, (value & (1 << i)) ? 1 : 0);
  }
}

u8 getRows() {
  return (u8)((MIOS32_BOARD_J10_Get() & ((1 << (NUM_ROWS + NUM_COLS)) - 1)) >> NUM_COLS);
}

void TASK_MatrixScan(void *pvParameters)
{
  while( 1 ) {
    // wait for next timesplice (1 mS)
    vTaskDelay(1 / portTICK_RATE_MS);
    ledTask();

    u8 row, column, value;
	for(column = 0; column < NUM_COLS; column++) {
      setColumns(1 << column);
	  MIOS32_DELAY_Wait_uS(100); //TODO fine tune to save time
      u8 row_value = getRows();
      u8 changed = row_value ^ button_state[column];
      if (changed) {
        for (row = 0; row < NUM_ROWS; row++) {
          if (changed & (1 << row)) {
            value = (row_value & (1 << row)) ? 1 : 0;
            BUTTON_NotifyToggle(row, column, value);
          }
        }
      }
      button_state[column] = row_value;
    }
  }
}

void APP_SRIO_ServicePrepare(void)
{
}
void APP_SRIO_ServiceFinish(void)
{
}
void APP_DIN_NotifyToggle(u32 pin, u32 pin_value)
{
}
void APP_ENC_NotifyChange(u32 encoder, s32 incrementer)
{
}
void APP_AIN_NotifyChange(u32 pin, u32 pin_value)
{
}
