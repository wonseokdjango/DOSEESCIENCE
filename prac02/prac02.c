/**
  * @File   prac02.c
  * @Author won.seok.django@gmail.com
  * @Brief  LED1 blinks, LED2 is controlled by tactile switch
  */

extern void enable_irq(void);

#define GPFSEL_BASE 0x3f200000
#define GPSET_BASE 0x3f20001c
#define GPCLR_BASE 0x3f200028
#define GPLEV_BASE 0x3f200034
#define IRQ_ENABLE_BASE 0x3f20b200
#define ARM_TIMER_BASE 0x3f20b400

unsigned int* GPFSEL = (unsigned int*)GPFSEL_BASE;
unsigned int* GPLEV = (unsigned int*)GPLEV_BASE;
unsigned int* GPSET = (unsigned int*)GPSET_BASE;
unsigned int* GPCLR = (unsigned int*)GPCLR_BASE;
unsigned int* IRQ_ENABLE = (unsigned int*)IRQ_ENABLE_BASE;
unsigned int* ARM_TIMER = (unsigned int*)ARM_TIMER_BASE;

/**
  * @Brief      This function set GPIO[_pin] as mode [_mode]
  * @Param[in]  _pin  pin number
  * @Param[in]  _mode 0 for input mode, others for output mode
  */
static inline void set_GPIO_mode(unsigned int _pin, unsigned int _mode)
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
static inline unsigned int read_GPIO(unsigned int _pin)
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
static inline void write_GPIO(unsigned int _pin, unsigned int _value)
{
  if (_value == 0)
    GPCLR[_pin / 32] |= (1 << (_pin % 32));
  else
    GPSET[_pin / 32] |= (1 << (_pin % 32));
}

void set_TIMER(unsigned int tick)
{
  IRQ_ENABLE[0] = 1;
  ARM_TIMER[0] = tick;
  ARM_TIMER[2] = 0x003E00A2;
  ARM_TIMER[3] = 0;

  enable_irq();
}

static volatile unsigned int cnt = 0;

void c_irq_handler(void)
{
  ++cnt;

  if (cnt % 2)
    write_GPIO(5, 0);
  else
    write_GPIO(5, 1);

  ARM_TIMER[3] = 0;
}


int my_main(void)
{
  int i;
  //set_TIMER(1000000);
  set_GPIO_mode(5, 1);
  
  write_GPIO(5, 0);
  for(i=0;i<100000;++i);
  write_GPIO(5, 1);
  for(i=0;i<100000;++i);

  write_GPIO(5, 0);
  for(i=0;i<100000;++i);
  write_GPIO(5, 1);
  for(i=0;i<100000;++i);

  write_GPIO(5, 0);
  for(i=0;i<100000;++i);
  write_GPIO(5, 1);
  for(i=0;i<100000;++i);

  while(1)
    continue;

  return 0;
}
