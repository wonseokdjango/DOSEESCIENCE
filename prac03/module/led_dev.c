/**
  * @File   prac03/led_dev.c
  * @Author won.seok.django@gmail.com
  * @Brief  LED device driver module
  */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/io.h>

MODULE_AUTHOR("Wonseok Lee");
MODULE_LICENSE("GPL");

#define GPFSEL_BASE 0x3f200000
#define GPSET_BASE 0x3f20001c
#define GPCLR_BASE 0x3f200028
#define GPLEV_BASE 0x3f200034

volatile unsigned int* GPFSEL;
volatile unsigned int* GPLEV;
volatile unsigned int* GPSET;
volatile unsigned int* GPCLR;

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

#define DEV_NUM 60
#define DEV_NAME "led_dev"

int led_open(struct inode* _inode, struct file* _filp);
int led_close(struct inode *inode, struct file *filp);
long led_ioctl(struct file *flip, unsigned int cmd, unsigned long arg);

/**
  *@Brief file operation table
  */
struct file_operations led_fops = {
  open : led_open,
  release : led_close,
  unlocked_ioctl : led_ioctl
};

/**
  *@Brief       module init function
  *@Param[out]  always 0
  */
int led_init(void)
{
  int ret = register_chrdev(DEV_NUM, DEV_NAME, &led_fops);
  if(ret < 0)
  {
    printk("WONSEOK > cannot obtain major number %d\n", DEV_NUM);
    return ret;
  }

  GPFSEL = (volatile unsigned int*)ioremap(GPFSEL_BASE, 4);
  GPLEV = (volatile unsigned int*)ioremap(GPLEV_BASE, 4);
  GPSET = (volatile unsigned int*)ioremap(GPSET_BASE, 4);
  GPCLR = (volatile unsigned int*)ioremap(GPCLR_BASE, 4);
  
  printk("WONSEOK > Hello DOSEESCIENCE\n");

  return 0;
}

/**
  *@Brief module exit function
  */
void led_exit(void)
{
  unregister_chrdev(DEV_NUM, DEV_NAME);
 	printk("WONSEOK > Bye DOSEESCIENCE\n");
}

/**
  *@Brief       set GPIO 05 as output pin for LED
  *@Param[in]   _inode  inode pointer
  *@Param[in]   _flip   file descriptor
  *@Param[out]  always 0
  */
int led_open(struct inode* _inode, struct file* _filp)
{
  set_GPIO_mode(5, 1);

  return 0;
}

/**
  *@Brief       do nothing
  *@Param[in]   _inode  inode pointer
  *@Param[in]   _flip   file descriptor
  *@Param[out]  always 0
  */
int led_close(struct inode *inode, struct file *filp)
{
  return 0;
}

/**
  *@Brief       turn-on or turn-off LED according to command
  *@Param[in]   _flip file descriptor
  *@Param[in]   _cmd  command
  *@Param[in]   _arg  argument, we do not use it
  *@Param[out]  always 0
  */
long led_ioctl(struct file *flip, unsigned int cmd, unsigned long arg)
{
  if (cmd)
    write_GPIO(5, 1);
  else
    write_GPIO(5, 0);

  return 0;
}

module_init(led_init);
module_exit(led_exit);

