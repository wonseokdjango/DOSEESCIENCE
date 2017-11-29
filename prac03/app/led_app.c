/**
  * @File   prac03/led_app.c
  * @Author won.seok.django@gmail.com
  * @Brief  application program for LED device driver test
  */
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
  int blink = 0;

  int fd = open("/dev/led_dev", O_RDWR);
  if (fd < 0)
  {
    printf("WONSEOK > device open failed.\n");
    exit(-1);
  }

  while (blink < 5)
  {
    ioctl(fd, 1, 0);
    sleep(1);

    ioctl(fd, 0, 0);
    sleep(1);

    ++blink;
  }

  close(fd);

  return 0;
}
