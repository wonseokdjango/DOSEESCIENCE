/**
  * @File   prac02.c
  * @Author won.seok.django@gmail.com
  * @Brief  LED blink using timer
  */
#define GPFSEL_BASE 0x3f200000
#define GPSET_BASE 0x3f20001c
#define GPCLR_BASE 0x3f200028
#define GPLEV_BASE 0x3f200034

#define ARM_TIMER_LOD_BASE 0x3F00B400
#define ARM_TIMER_VAL_BASE 0x3F00B404
#define ARM_TIMER_CTL_BASE 0x3F00B408
#define ARM_TIMER_CLI_BASE 0x3F00B40C
#define ARM_TIMER_RIS_BASE 0x3F00B410
#define ARM_TIMER_MIS_BASE 0x3F00B414
#define ARM_TIMER_RLD_BASE 0x3F00B418
#define ARM_TIMER_DIV_BASE 0x3F00B41C
#define ARM_TIMER_CNT_BASE 0x3F00B420

#define IRQ_ENABLE_BASIC_BASE 0x3F00B218
#define IRQ_DISABLE_BASIC_BASE 0x3F00B224

unsigned int* GPFSEL = (unsigned int*)GPFSEL_BASE;
unsigned int* GPLEV = (unsigned int*)GPLEV_BASE;
unsigned int* GPSET = (unsigned int*)GPSET_BASE;
unsigned int* GPCLR = (unsigned int*)GPCLR_BASE;

unsigned int* ARM_TIMER_LOD = (unsigned int*)ARM_TIMER_LOD_BASE;
unsigned int* ARM_TIMER_VAL = (unsigned int*)ARM_TIMER_VAL_BASE;
unsigned int* ARM_TIMER_CTL = (unsigned int*)ARM_TIMER_CTL_BASE;
unsigned int* ARM_TIMER_CLI = (unsigned int*)ARM_TIMER_CLI_BASE;
unsigned int* ARM_TIMER_RIS = (unsigned int*)ARM_TIMER_RIS_BASE;
unsigned int* ARM_TIMER_MIS = (unsigned int*)ARM_TIMER_MIS_BASE;
unsigned int* ARM_TIMER_RLD = (unsigned int*)ARM_TIMER_RLD_BASE;
unsigned int* ARM_TIMER_DIV = (unsigned int*)ARM_TIMER_DIV_BASE;
unsigned int* ARM_TIMER_CNT = (unsigned int*)ARM_TIMER_CNT_BASE;

unsigned int* IRQ_ENABLE_BASIC = (unsigned int*)IRQ_ENABLE_BASIC_BASE;
unsigned int* IRQ_DISABLE_BASIC = (unsigned int*)IRQ_DISABLE_BASIC_BASE;

extern void enable_irq (void);
volatile unsigned int cnt = 0;

/**
  * @Brief      This function set GPIO[_pin] as mode [_mode]
  * @Param[in]  _pin  pin number
  * @Param[in]  _mode 0 for input mode, others for output mode
  */
inline void set_GPIO_mode(unsigned int _pin, unsigned int _mode)
{
  GPFSEL[_pin / 10] &= ~(7 << (3 * (_pin % 10)));

  if (_mode != 0)
    GPFSEL[_pin / 10] |= (1 << (3 * (_pin % 10)));
}


/**
  * @Brief      This function read value from GPIO[_pin]
  * @Param[in]  _pin  pin number
  * @Param[out] 0 or 1
  */
inline unsigned int read_GPIO(unsigned int _pin)
{
  if (GPLEV[_pin / 32] & (1 << (_pin % 32)))
    return 1;
  else
    return 0;
}

/**
  * @Brief      This function write [_value] at GPIO[_pin]
  * @Param[in]  _pin    pin number
  * @Param[in]  _value  0 for 0, others for 1
  */
inline void write_GPIO(unsigned int _pin, unsigned int _value)
{
  if (_value == 0)
    GPCLR[_pin / 32] = (1 << (_pin % 32));
  else
    GPSET[_pin / 32] = (1 << (_pin % 32));
}

/**
  * @Brief  This function is timer interrupt handler which toggles LED
  */
void c_irq_handler ( void )
{
  ++cnt;
  if (cnt % 2)
    write_GPIO(5, 1);
  else
    write_GPIO(5, 0);

  /*
  if (cnt % 3)
    write_GPIO(6, 1);
  else
    write_GPIO(6, 0);
    */
  
  ARM_TIMER_CLI[0] = 0;
}

int main (void)
{
  IRQ_ENABLE_BASIC[0] = 0;
  IRQ_DISABLE_BASIC[0] = 1;
  
  ARM_TIMER_LOD[0] = 2000000;
  ARM_TIMER_RLD[0] = 2000000;
  ARM_TIMER_CLI[0] = 0;
  ARM_TIMER_CTL[0] = 0x003E00A2;

  set_GPIO_mode(5, 1);
  //set_GPIO_mode(6, 1);

  IRQ_ENABLE_BASIC[0] = 1;
  
  enable_irq();

  while(1);

  return 0;
}
