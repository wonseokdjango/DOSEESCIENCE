/**
  * @File   prac01.c
  * @Author won.seok.django@gmail.com
  * @Brief  LED blink
  */
#define GPFSEL_BASE 0x3f200000
#define GPSET_BASE 0x3f20001c
#define GPCLR_BASE 0x3f200028
#define GPLEV_BASE 0x3f200034

unsigned int* GPFSEL = (unsigned int*)GPFSEL_BASE;
unsigned int* GPLEV = (unsigned int*)GPLEV_BASE;
unsigned int* GPSET = (unsigned int*)GPSET_BASE;
unsigned int* GPCLR = (unsigned int*)GPCLR_BASE;

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
    GPCLR[_pin / 32] = (1 << (_pin % 32));
  else
    GPSET[_pin / 32] = (1 << (_pin % 32));
}

int main(void)
{
  int dly;

  set_GPIO_mode(5, 1);

  while(1)
  {
    write_GPIO(5, 1);

    for(dly = 0; dly < 1000000; ++dly);

    write_GPIO(5, 0);

    for(dly = 0; dly < 1000000; ++dly);
  }

  return 0;
}
