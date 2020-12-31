
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#include <linux/platform_device.h>


#include <linux/miscdevice.h>
#include <linux/device.h>

#include <linux/fs.h>

#include <linux/gpio.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>
#include <mach/gpio.h>

#define LED_MAGIC    'k'
#define IOCTL_LED_ON   _IOW(LED_MAGIC,1,int)
#define IOCTL_LED_OFF  _IOW(LED_MAGIC,2,int)
#define IOCTL_LED_RUN  _IOW(LED_MAGIC,3,int)
#define IOCTL_LED_SHINE  _IOW(LED_MAGIC,4,int)
#define IOCTL_LED_ALLON  _IOW(LED_MAGIC,5,int)
#define IOCTL_LED_ALLOFF _IOW(LED_MAGIC,6,int)
#define LED_PIN       NUC980_PB8


#define TIMER_SET    (200*HZ)/1000


static int major;
static struct class *jt_gpio_class;
static struct device *jt_gpio_device;
static struct timer_list test_timer;

unsigned char ledflag = 1;

static void sec_timer_handler(unsigned long arg)
{
    mod_timer(&test_timer,jiffies+TIMER_SET);
    if(ledflag != 0)
    {
        gpio_set_value(LED_PIN,0);
        ledflag = 0;
    }
    else
    {
        gpio_set_value(LED_PIN,1);
        ledflag = 1;
    } 
}

static int led_flash(void)
{
    printk(KERN_INFO "Timer init \n");
    test_timer.function = sec_timer_handler;
    test_timer.expires = jiffies + TIMER_SET;
    printk(KERN_INFO "TIMER_SET is %d\n",TIMER_SET);

    init_timer(&test_timer);
    add_timer(&test_timer);

    return 0;
}


static long gpio_led_ioctrl(struct file *files,unsigned int cmd,unsigned long arg)
{
    printk("cmd is %d,arg is %d\n",cmd,(unsigned int)(arg));

    switch(cmd)
    {
        case IOCTL_LED_ON:
        del_timer(&test_timer);
        gpio_set_value(LED_PIN,0);
        break;
        case IOCTL_LED_OFF:
        del_timer(&test_timer);
        gpio_set_value(LED_PIN,1);
        break;
        case IOCTL_LED_RUN:
        led_flash();
        break;

        default:
        break;
    }
    return 0;
}

static int gpio_led_release(struct inode *inode,struct file *file)
{
    printk(KERN_EMERG "GPIO LED RELEASE\n");
    return 0;
}

static int gpio_led_open(struct inode *inode,struct file *file)
{
    printk(KERN_EMERG "GPIO LED OPEN\n");
    return 0;
}

static struct file_operations gpio_led_ops ={
    .owner       =   THIS_MODULE,
    .open        =   gpio_led_open,
    .release     =   gpio_led_release,
    .unlocked_ioctl =  gpio_led_ioctrl,
};

static int gpio_led_init(void)
{
    int ret;
    printk(KERN_EMERG "MODULE 20200921\n");
    ret = gpio_request(LED_PIN,"LED_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST NUC980 PB8 FAILED!\n");
        return ret;
    }

    gpio_direction_output(LED_PIN,1);
    gpio_set_value(LED_PIN,0);
    major = register_chrdev(0,"gpio_led_gpio",&gpio_led_ops);

    jt_gpio_class = class_create(THIS_MODULE,"jt_gpio_class");

    if(!jt_gpio_class)
    {
        printk(KERN_EMERG "jt gpio class create failed!\n");
        return -1;
    }

    jt_gpio_device = device_create(jt_gpio_class,NULL,MKDEV(major,0),NULL,"gpio_led_gpio");
    if(!jt_gpio_device)
    {
        printk(KERN_EMERG "jt gpio device create failed!\n");
        return -1;
    }

    return 0;
}

static void gpio_led_exit(void)
{
    printk(KERN_EMERG "module 20200921 exit!\n");
    
    unregister_chrdev(major,"gpio_led_gpio");
    gpio_free(LED_PIN);

    device_unregister(jt_gpio_device);
    class_destroy(jt_gpio_class);

    del_timer(&test_timer);

}



module_init(gpio_led_init);
module_exit(gpio_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WANG");







