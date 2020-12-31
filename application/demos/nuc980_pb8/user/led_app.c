
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define LED_MAGIC    'k'
#define IOCTL_LED_ON   _IOW(LED_MAGIC,1,int)
#define IOCTL_LED_OFF  _IOW(LED_MAGIC,2,int)
#define IOCTL_LED_RUN  _IOW(LED_MAGIC,3,int)
#define IOCTL_LED_SHINE  _IOW(LED_MAGIC,4,int)
#define IOCTL_LED_ALLON  _IOW(LED_MAGIC,5,int)
#define IOCTL_LED_ALLOFF _IOW(LED_MAGIC,6,int)
#define LED_PIN       NUC980_PB8

void usage(char *exename)
{
    printf("Usage:\n");
    printf("  %s <led_no> <on/off>\n",exename);
    printf("  led_no = 1,2,3or4\n");

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

    fd = open("/dev/gpio_led_gpio",0);
    if(fd < 0)
    {
        printf("Can not open /dev/gpio_led_gpio\n");
        return  -1;
    }

    printf ("argc = %d \n",argc);

    if(argc == 2)
    {
        if(!strcmp(argv[1],"on"))
        {
            printf("argv[1]=on\n");
            ioctl(fd,IOCTL_LED_ON,count);
        }
        else if(!strcmp(argv[1],"off"))
        {
            printf("argv[1]=off\n");
            ioctl(fd,IOCTL_LED_OFF,count);
        }
        else if(!strcmp(argv[1],"run"))
        {
            printf("argv[1]=run\n");
            ioctl(fd,IOCTL_LED_RUN,count);
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