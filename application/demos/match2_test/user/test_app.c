
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define GPIO_MAGIC    'k'
#define IOCTL_INPUT   _IOW(GPIO_MAGIC,1,int)
#define IOCTL_OUTPUT  _IOW(GPIO_MAGIC,2,int)
#define IOCTL_ETHERNET  _IOW(GPIO_MAGIC,3,int)
#define IOCTL_UART4  _IOW(GPIO_MAGIC,4,int)
#define IOCTL_UART5  _IOW(GPIO_MAGIC,5,int)
#define IOCTL_LED_ALLOFF _IOW(GPIO_MAGIC,6,int)
#define LED1_PIN       NUC980_PB8

void usage(char *exename)
{
    printf("Usage:\n");
    printf("  %s <input/output>\n",exename);

}


int main(int argc,char **argv)
{
    unsigned int led_no;
    int fd = -1;
    unsigned int count = 10;

    if((argc > 3)||(argc == 1))
    {
        return -1;
    }

    fd = open("/dev/test_drv",0);
    if(fd < 0)
    {
        printf("Can not open /dev/test_dev\n");
        return  -1;
    }

    printf ("argc = %d \n",argc);

    if(argc == 2)
    {
        if(!strcmp(argv[1],"input"))
        {
            printf("argv[1]=input\n");
            ioctl(fd,IOCTL_INPUT,count);
        }
        else if(!strcmp(argv[1],"output"))
        {
            printf("argv[1]=output\n");
            ioctl(fd,IOCTL_OUTPUT,count);
        }
        else if(!strcmp(argv[1],"ethernet"))
        {
            printf("argv[1]=ethernet\n");
            system("udhcpc -i eth0");
            ioctl(fd,IOCTL_ETHERNET,count);
        }
        else if(!strcmp(argv[1],"uart4"))
        {
            printf("argv[1]=uart4\n");
            system("cd ~");
            system("./usr/bin/uart_test uart4");
            ioctl(fd,IOCTL_UART4,count);
        }
        else if(!strcmp(argv[1],"uart5"))
        {
            printf("argv[1]=uart5\n");
            system("cd ~");
            system("./usr/bin/uart_test uart5");
            ioctl(fd,IOCTL_UART5,count);
        }
    }
    else
    {
        goto err;
    }
    close(fd);
    return 0;

err:
    if(fd > 0)
    {
        close(fd);
    }
    usage(argv[0]);
    return -1;
}