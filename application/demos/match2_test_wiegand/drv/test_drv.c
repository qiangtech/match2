
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

#define GPIO_MAGIC    'k'
#define IOCTL_INPUT   _IOW(GPIO_MAGIC,1,int)
#define IOCTL_OUTPUT  _IOW(GPIO_MAGIC,2,int)
#define IOCTL_ETHERNET  _IOW(GPIO_MAGIC,3,int)
#define IOCTL_UART4  _IOW(GPIO_MAGIC,4,int)
#define IOCTL_UART5  _IOW(GPIO_MAGIC,5,int)
#define IOCTL_WIEGAND  _IOW(GPIO_MAGIC,6,int)


#define LED1_PIN       NUC980_PB8   //running led
#define RD1_D1_PIN     NUC980_PA0   //reader1 d1 input
#define RD1_D0_PIN     NUC980_PA1   //reader1 d0 input
#define RD1_GLED_PIN   NUC980_PA2   //reader1 gled output
#define RD1_RLED_PIN   NUC980_PA3   //reader1 rled output

#define RD2_D1_PIN     NUC980_PA8   //reader2 d1 input
#define RD2_D0_PIN     NUC980_PA9   //reader2 d0 input
#define RD2_GLED_PIN   NUC980_PA10  //reader2 gled output
#define RD2_RLED_PIN   NUC980_PA11  //reader2 rled output

#define SENSER_PIN     NUC980_PB4   //senser input
#define TAMPER_PIN     NUC980_PB13  //tamp input
#define ARM_PIN        NUC980_PC0   //alarm input
#define RELAY3_PIN     NUC980_PC1   //alarm relay output
#define RELAY2_PIN     NUC980_PC2   //relay2 output
#define SOUNDER_PIN    NUC980_PC3   //sounder output
#define RELAY1_PIN     NUC980_PC4   //relay1 output

#define BUZZER_PIN     NUC980_PC15  //buzzer output

#define BUT_LED_PIN    NUC980_PF7   //button led output
#define PSU_PIN        NUC980_PF8   //psu input
#define CAT_PIN        NUC980_PF9   //contact input
#define BUTTON_PIN     NUC980_PF10  //button input

#define OUT12V_PIN     NUC980_PE10  //output 12v control output
#define DIR485_PIN     NUC980_PE12  //rs485 dir input


#define TIMER_SET    (200*HZ)/1000
#define timer1_period    (100*HZ)/1000    //100mS
#define timer2_period    (500*HZ)/1000   //500mS
#define timer3_period    (1*HZ)/1000   //1mS

static int major;
static struct class *jt_gpio_class;
static struct device *jt_gpio_device;
static struct timer_list test_timer;
static struct timer_list test1_timer;
static struct timer_list test2_timer;
static struct timer_list test3_timer;

unsigned char testmode = 0;


static void timer1_handler(unsigned long arg)
{
    static int value = 0;
    int res;

    mod_timer(&test1_timer,jiffies+timer1_period);

    res = gpio_get_value( TAMPER_PIN );
    if(res & ((value & 0x01)==0))
    {
        printk("Tamper pin change to HIGH! \n");
        value |= (0x01<<0);
    }
    else if ((res==0) & ((value & 0x01)!=0))
    {
        printk("Tamper pin change to LOW! \n");
        value &= ~(0x01<<0);
    }

    res = gpio_get_value( PSU_PIN );
    if(res & ((value & 0x02)==0))
    {
        printk("PSU pin change to HIGH! \n");
        value |= (0x01<<1);
    }
    else if ((res==0) & ((value & 0x02)!=0))
    {
        printk("PSU pin change to LOW! \n");
        value &= ~(0x01<<1);
    }

    res = gpio_get_value( CAT_PIN );
    if(res & ((value & 0x04)==0))
    {
        printk("CAT pin change to HIGH! \n");
        value |= (0x01<<2);
    }
    else if ((res==0) & ((value & 0x04)!=0))
    {
        printk("CAT pin change to LOW! \n");
        value &= ~(0x01<<2);
    }    

    res = gpio_get_value( BUTTON_PIN );
    if(res & ((value & 0x08)==0))
    {
        printk("BUTTON pin change to HIGH! \n");
        value |= (0x01<<3);
    }
    else if ((res==0) & ((value & 0x08)!=0))
    {
        printk("BUTTON pin change to LOW! \n");
        value &= ~(0x01<<3);
    } 

/*    res = gpio_get_value( RD1_D1_PIN );
    if(res & ((value & 0x10)==0))
    {
        printk("RD1_D1 pin change to HIGH! \n");
        value |= (0x01<<4);
    }
    else if ((res==0) & ((value & 0x10)!=0))
    {
        printk("RD1_D1 pin change to LOW! \n");
        value &= ~(0x01<<4);
    }

    res = gpio_get_value( RD1_D0_PIN );
    if(res & ((value & 0x20)==0))
    {
        printk("RD1_D0 pin change to HIGH! \n");
        value |= (0x01<<5);
    }
    else if ((res==0) & ((value & 0x20)!=0))
    {
        printk("RD1_D0 pin change to LOW! \n");
        value &= ~(0x01<<5);
    }

    res = gpio_get_value( RD2_D1_PIN );
    if(res & ((value & 0x40)==0))
    {
        printk("RD2_D1 pin change to HIGH! \n");
        value |= (0x01<<6);
    }
    else if ((res==0) & ((value & 0x40)!=0))
    {
        printk("RD2_D1 pin change to LOW! \n");
        value &= ~(0x01<<6);
    }

    res = gpio_get_value( RD2_D0_PIN );
    if(res & ((value & 0x80)==0))
    {
        printk("RD2_D0 pin change to HIGH! \n");
        value |= (0x01<<7);
    }
    else if ((res==0) & ((value & 0x80)!=0))
    {
        printk("RD2_D0 pin change to LOW! \n");
        value &= ~(0x01<<7);
    } 
*/
    res = gpio_get_value( ARM_PIN );
    if(res & ((value & 0x100)==0))
    {
        printk("ARM pin change to HIGH! \n");
        value |= (0x01<<8);
    }
    else if ((res==0) & ((value & 0x100)!=0))
    {
        printk("ARM pin change to LOW! \n");
        value &= ~(0x01<<8);
    }

    res = gpio_get_value( SENSER_PIN );
    if(res & ((value & 0x200)==0))
    {
        printk("SENSER pin change to HIGH! \n");
        value |= (0x01<<9);
    }
    else if ((res==0) & ((value & 0x200)!=0))
    {
        printk("SENSER pin change to LOW! \n");
        value &= ~(0x01<<9);
    } 

    res = gpio_get_value( DIR485_PIN );
    if(res & ((value & 0x400)==0))
    {
        printk("RS458 DIR pin change to HIGH! \n");
        value |= (0x01<<10);
    }
    else if ((res==0) & ((value & 0x400)!=0))
    {
        printk("RS485 DIR pin change to LOW! \n");
        value &= ~(0x01<<10);
    }   
}

static void reset_output(void)
{
    gpio_set_value( LED1_PIN ,1);
    gpio_set_value( BUT_LED_PIN ,0);
    gpio_set_value( RD1_GLED_PIN ,0);
    gpio_set_value( RD1_RLED_PIN ,0);
    gpio_set_value( RD2_GLED_PIN ,0);
    gpio_set_value( RD2_RLED_PIN ,0);
    gpio_set_value( RELAY1_PIN ,0);
    gpio_set_value( RELAY2_PIN ,0);
    gpio_set_value( RELAY3_PIN ,0);
    gpio_set_value( SOUNDER_PIN ,0);
    gpio_set_value( BUZZER_PIN ,0);
    gpio_set_value( OUT12V_PIN ,0);
}

static void timer2_handler(unsigned long arg)
{
    static int times = 0;

    mod_timer(&test2_timer,jiffies+timer2_period);

    reset_output();
    switch(times)
    {
        case 0:
            gpio_set_value( LED1_PIN ,0);
            break;
        case 1:
            gpio_set_value( BUT_LED_PIN ,1);
            break;
            case 2:
            gpio_set_value( RD1_GLED_PIN ,1);
            break;
            case 3:
            gpio_set_value( RD1_RLED_PIN ,1);
            break;
            case 4:
            gpio_set_value( RD2_GLED_PIN ,1);
            break;
            case 5:
            gpio_set_value( RD2_RLED_PIN ,1);
            break;
            case 6:
            gpio_set_value( RELAY1_PIN ,1);
            break;
            case 7:
            gpio_set_value( RELAY2_PIN ,1);
            break;
            case 8:
            gpio_set_value( RELAY3_PIN ,1);
            break;
            case 9:
            gpio_set_value( SOUNDER_PIN ,1);
            break;
            case 10:
            gpio_set_value( BUZZER_PIN ,1);
            break;
            case 11:
            gpio_set_value( OUT12V_PIN ,1);
            break;

            default:
            break;
    }
    times++;
    if(times>=12)
    {
        times = 0;
    }
}

static int reader1_receiving=0;  
static int reader1_received_bits=0;
static unsigned long reader1_receiv_data=0;
static int reader1_receiv_timecnt = 0;

static int reader2_receiving=0;  
static int reader2_received_bits=0;
static unsigned long reader2_receiv_data=0;
static int reader2_receiv_timecnt = 0;


static int check_patity(int num)
{
    int bits;
    unsigned long data;
    int part_bit;
    int one_number;
    int i;

    if(num==1)
    {
        bits= reader1_received_bits;
        data = reader1_receiv_data;
    }
    if(num==2)
    {
        bits= reader2_received_bits;
        data= reader2_receiv_data;
    }

    part_bit = bits/2;
    one_number = 0;

/*    if((data&0xffffff00)!=0)
    {
        return 0;
    }
*/
    if(bits%2)
    {
        part_bit++;
    }
    for(i=0;i<part_bit;i++)
    {
        if((data&(0x01<<i))!=0)
        {
            one_number++;
        }
    }
    if((one_number%2)==0)
    {
        return 0;
    }

    one_number=0;
    for(i=part_bit;i<bits;i++)
    {
        if((data&0x01<<i)!=0)
        {
            one_number++;
        }
    }
    if((one_number%2)!=0)
    {
        return 0;
    }
    return 1;
}

static void press_data(unsigned long data)
{
    switch(data&0xff)
    {
        case 0x51:
        gpio_set_value( RELAY1_PIN ,1);
        gpio_set_value( RD1_GLED_PIN ,1);
        break;
        case 0x52:
        gpio_set_value( RELAY2_PIN ,1);
        gpio_set_value( RD2_GLED_PIN ,1);
        break;
        case 0x61:
        gpio_set_value( RELAY1_PIN ,0);
        gpio_set_value( RD1_GLED_PIN ,0);
        break;
        case 0x62:
        gpio_set_value( RELAY2_PIN ,0);
        gpio_set_value( RD2_GLED_PIN ,0);
        break;
        default:
        break;
    }
}

static void clear_readerdata(int num)
{
    if(num==1)
    {
        reader1_received_bits=0;
        reader1_receiv_data=0;
    }
    if(num==2)
    {
        reader2_received_bits=0;
        reader2_receiv_data=0;
    }
}

static void wieganddata_handler(int num)
{
    if(num==1)
    {
        if((reader1_received_bits>=26)&&(reader1_received_bits<=37))   //26bits-37bits
        {
            if(check_patity(1))  //valid data
            {
                reader1_receiv_data &=((0x01<<(reader1_received_bits-1))^0xfffffffff);  //remove first patity
                reader1_receiv_data >>=1; //remove last patity
                press_data((unsigned long)(reader1_receiv_data));
                
            }
        }
    }

    if(num==2)
    {
        if((reader2_received_bits>=26)&&(reader2_received_bits<=37))   //26bits-37bits
        {
            if(check_patity(2))
            {
                reader2_receiv_data &=((0x01<<(reader2_received_bits-1))^0xfffffffff);  //remove first patity
                reader2_receiv_data >>=1; //remove last patity
                press_data((unsigned long)(reader2_receiv_data));
                
            }
        }
    }
}



static void timer3_handler(unsigned long arg)
{
    
    mod_timer(&test3_timer,jiffies+timer3_period);
    if(reader1_receiving)
    {
        reader1_receiv_timecnt++;
        if(reader1_receiv_timecnt>=10)  //10msec no wiegand signal
        {
            reader1_receiv_timecnt=0;
            reader1_receiving=0;  //receiv finish
            wieganddata_handler(1);
            printk("Reader1 Wiegand received finish,received %d bits,receiced data %d.\n",reader1_received_bits,reader1_receiv_data);
            clear_readerdata(1);  //clear received data
        }
    }
    if(reader2_receiving)
    {
        reader2_receiv_timecnt++;
        if(reader2_receiv_timecnt>=10)  //10msec no wiegand signal
        {
            reader2_receiv_timecnt=0;
            reader2_receiving=0;  //receiv finish
            wieganddata_handler(2);
            printk("Reader2 Wiegand received finish,received %d bits,receiced data %d.\n",reader2_received_bits,reader2_receiv_data);
            clear_readerdata(2);  //clear received data
        }
    }
}

static int all_input_test(void)
{
    printk(KERN_INFO "Input port begin test,all input port change will message output! \n");
    test1_timer.function = timer1_handler;
    test1_timer.expires = jiffies + timer1_period;

    init_timer(&test1_timer);
    add_timer(&test1_timer);

    return 0;
}


static int all_output_test(void)
{
    printk(KERN_INFO "Output port begin test,all output port loop driver high 500msec! \n");
    test2_timer.function = timer2_handler;
    test2_timer.expires = jiffies + timer2_period;

    init_timer(&test2_timer);
    add_timer(&test2_timer);

    return 0;
}

static int wiegand_test(void)
{
    printk(KERN_INFO "Test reader1 and reader2 wiegand input,waitting wiegand input!(format:26bits)  \n");
    test3_timer.function = timer3_handler;
    test3_timer.expires = jiffies + timer3_period;

    init_timer(&test3_timer);
    add_timer(&test3_timer);

    gpio_set_value( OUT12V_PIN ,1);  //open reader power

    return 0;
}

static irqreturn_t RD1_D1IntHandler(int irq,void *dev_id)
{
    reader1_receiving=1;
    reader1_receiv_data <<=1;
    reader1_receiv_data |=0x01;
    reader1_received_bits++;
    reader1_receiv_timecnt=0;
    return IRQ_HANDLED;
}

static irqreturn_t RD1_D0IntHandler(int irq,void *dev_id)
{
    reader1_receiving=1;
    reader1_receiv_data <<=1;
    reader1_received_bits++;
    reader1_receiv_timecnt=0;
    return IRQ_HANDLED;
}

static irqreturn_t RD2_D1IntHandler(int irq,void *dev_id)
{
    reader2_receiving=1;
    reader2_receiv_data <<=1;
    reader2_receiv_data |=0x01;
    reader2_received_bits++;
    reader2_receiv_timecnt=0;
    return IRQ_HANDLED;
}

static irqreturn_t RD2_D0IntHandler(int irq,void *dev_id)
{
    reader2_receiving=1;
    reader2_receiv_data <<=1;
    reader2_received_bits++;
    reader2_receiv_timecnt=0;
    return IRQ_HANDLED;
}


static long test_drv_ioctrl(struct file *files,unsigned int cmd,unsigned long arg)
{
    printk("cmd is %d,arg is %d\n",cmd,(unsigned int)(arg));

    switch(cmd)
    {
        case IOCTL_INPUT:
        if(testmode!=1)
        {
            if(testmode==2)
                del_timer(&test2_timer);
            else if(testmode==3)    
                del_timer(&test3_timer);
            testmode = 1;
            reset_output();
            all_input_test();
        }
        else
        {
            printk("MATCH2 was in input testting!\n");
        }
        break;
        case IOCTL_OUTPUT:
        if(testmode!=2)
        {
            if(testmode==1)
                del_timer(&test1_timer);
            else if(testmode==3)
                del_timer(&test3_timer);
            testmode = 2;
            all_output_test();
        }
        else
        {
            printk("MATCH2 was in output testting!\n");
        }
        break;
        case IOCTL_ETHERNET:
        if(testmode==1)
        {
            del_timer(&test1_timer);
        }
        else if(testmode==2)
        {
            del_timer(&test2_timer);
        }
        else if(testmode==3)
        {
            del_timer(&test3_timer);
        }
        reset_output();
        testmode = 0;
        break;
        case IOCTL_UART4:
        if(testmode==1)
        { 
            del_timer(&test1_timer);
        }
        else if(testmode==2)
        {
            del_timer(&test2_timer);
        }
        else if(testmode==3)
        {
            del_timer(&test3_timer);
        }
        reset_output();
        testmode = 0;
        break;
        case IOCTL_UART5:
        if(testmode==1)
        { 
            del_timer(&test1_timer);
        }
        else if(testmode==2)
        {
            del_timer(&test2_timer);
        }
        else if(testmode==3)
        {
            del_timer(&test3_timer);
        }
        reset_output();
        testmode = 0;
        break;
        case IOCTL_WIEGAND:
        if(testmode!=3)
        {
            reset_output();
            if(testmode==1)
                del_timer(&test1_timer);
            else if(testmode==2)
                del_timer(&test2_timer);
            testmode = 3;
            wiegand_test();
        }
        else
        {
            printk("MATCH2 was in wiegand testting!\n");
        }
        break;

        default:
        break;
    }
    return 0;
}

static int test_drv_release(struct inode *inode,struct file *file)
{
    printk(KERN_EMERG "MATCH2 TEST RELEASE\n");
    return 0;
}

static int test_drv_open(struct inode *inode,struct file *file)
{
    printk(KERN_EMERG "MATCH2 TEST OPEN\n");
    return 0;
}

static struct file_operations test_drv_ops ={
    .owner       =   THIS_MODULE,
    .open        =   test_drv_open,
    .release     =   test_drv_release,
    .unlocked_ioctl =  test_drv_ioctrl,
};


static int gpio_init(void)
{
    int ret;
    int res = 0;
    int irqno;

    ret = gpio_request(LED1_PIN,"LED1_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST NUC980_PB8 FAILED!\n");
        res |= (0x01<<0);
    }
    else
    {
      gpio_direction_output(LED1_PIN,1);  //running led set output
      gpio_set_value(LED1_PIN,1);    //running led off
    }


/******Reader1 wiegand input and gled&rled output******************/
    ret = gpio_request(RD1_D1_PIN ,"RD1_D1_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD1_D1_PIN FAILED!\n");
        res |= (0x01<<1);
    }
    else
    {
//        gpio_direction_input(RD1_D1_PIN);  //wiegand rd1_d1 set input
        irqno = gpio_to_irq(RD1_D1_PIN);
        request_irq(irqno,RD1_D1IntHandler,
                    IRQF_TRIGGER_RISING,"RD1_D1_PIN",NULL);
    }
    
    ret = gpio_request(RD1_D0_PIN ,"RD1_D0_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD1_D0_PIN FAILED!\n");
        res |= (0x01<<2);
    }
    else
    {
//        gpio_direction_input(RD1_D0_PIN);  //wiegand rd1_d0 set input 
        irqno = gpio_to_irq(RD1_D0_PIN);
        request_irq(irqno,RD1_D0IntHandler,
                    IRQF_TRIGGER_RISING,"RD1_D0_PIN",NULL);
    }
       
    ret = gpio_request(RD1_GLED_PIN ,"RD1_GLED_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD1_GLED_PIN FAILED!\n");
        res |= (0x01<<3);
    }
    else
    {
        gpio_direction_output(RD1_GLED_PIN,0);  //rd1_gled set output and value low
    }
    
    ret = gpio_request(RD1_RLED_PIN ,"RD1_RLED_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD1_RLED_PIN FAILED!\n");
        res |= (0x01<<4);
    }
    else
    {
        gpio_direction_output(RD1_RLED_PIN,0);  //rd1_rled set output and value low 
    }

/******Reader2 wiegand input and gled&rled output*******************/
    ret = gpio_request(RD2_D1_PIN ,"RD2_D1_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD2_D1_PIN FAILED!\n");
        res |= (0x01<<5);
    }
    else
    {
//        gpio_direction_input(RD2_D1_PIN);  //wiegand rd2_d1 set input
        irqno = gpio_to_irq(RD2_D1_PIN);
        request_irq(irqno,RD2_D1IntHandler,
                    IRQF_TRIGGER_RISING,"RD2_D1_PIN",NULL);
    }
    
    ret = gpio_request(RD2_D0_PIN ,"RD2_D0_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD2_D0_PIN FAILED!\n");
        res |= (0x01<<6);
    }
    else
    {
//        gpio_direction_input(RD2_D0_PIN);  //wiegand rd2_d0 set input 
        irqno = gpio_to_irq(RD2_D0_PIN);
        request_irq(irqno,RD2_D0IntHandler,
                    IRQF_TRIGGER_RISING,"RD2_D0_PIN",NULL);
    }
      
    ret = gpio_request(RD2_GLED_PIN ,"RD2_GLED_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD2_GLED_PIN FAILED!\n");
        res |= (0x01<<7);
    }
    else
    {
        gpio_direction_output(RD2_GLED_PIN,0);  //rd2_gled set output and value low
    }
    
    ret = gpio_request(RD2_RLED_PIN ,"RD2_RLED_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RD2_RLED_PIN FAILED!\n");
        res |= (0x01<<8);
    }
    else
    {
        gpio_direction_output(RD2_RLED_PIN,0);  //rd2_rled set output and value low
    }

/**************************Relay output**************************/
    ret = gpio_request(RELAY3_PIN ,"RELAY3_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RELAY3_PIN FAILED!\n");
        res |= (0x01<<9);
    }
    else
    {
        gpio_direction_output(RELAY3_PIN,0);  //relay3 set output and value low
    }
      
    ret = gpio_request(RELAY2_PIN ,"RELAY2_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RELAY2_PIN FAILED!\n");
        res |= (0x01<<10);
    }
    else
    {
        gpio_direction_output(RELAY2_PIN,0);  //relay2 set output and value low 
    }
    
    ret = gpio_request(RELAY1_PIN ,"RELAY1_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST RELAY1_PIN FAILED!\n");
        res |= (0x01<<11);
    }
    else
    {
        gpio_direction_output(RELAY1_PIN,0);  //relay1 set output and value low
    }
       
/**************************Sounder and Buzzer output**************************/
    ret = gpio_request( SOUNDER_PIN ,"SOUNDER_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST SOUNDER_PIN FAILED!\n");
        res |= (0x01<<12);
    }
    else
    {
        gpio_direction_output(SOUNDER_PIN,0);  //sounder control set output and value low
    }
      
    ret = gpio_request( BUZZER_PIN ,"BUZZER_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST BUZZER_PIN FAILED!\n");
        res |= (0x01<<13);
    }
    else
    {
        gpio_direction_output(BUZZER_PIN,0);  //buzzer control set output and value low
    }
        

/**************************button led and output 12v control output**************************/
    ret = gpio_request( BUT_LED_PIN ,"BUT_LED_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST BUT_LED_PIN FAILED!\n");
        res |= (0x01<<14);
    }
    else
    {
        gpio_direction_output(BUT_LED_PIN,0);  //button led set output and value low 
    }

    ret = gpio_request( OUT12V_PIN ,"OUT12V_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST OUT12V_PIN FAILED!\n");
        res |= (0x01<<15);
    }
    else
    {
        gpio_direction_output(OUT12V_PIN,0);  //output 12v  control set output and value low 
    }
    

/***********************************Other input***************************************/
    ret = gpio_request( SENSER_PIN ,"SENSER_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST SENSER_PIN FAILED!\n");
        res |= (0x01<<16);
    }
    else
    {
        gpio_direction_input(SENSER_PIN);  //senser in set input
    }
    
    ret = gpio_request( ARM_PIN ,"ARM_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST ARM_PIN FAILED!\n");
        res |= (0x01<<17);
    }
    else
    {
        gpio_direction_input(ARM_PIN);  //alarm in set input
    }
     
    ret = gpio_request( PSU_PIN ,"PSU_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST PSU_PIN FAILED!\n");
        res |= (0x01<<18);
    }
    else
    {
        gpio_direction_input(PSU_PIN);  //psu in set input
    }

    ret = gpio_request( CAT_PIN ,"CAT_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST CAT_PIN FAILED!\n");
        res |= (0x01<<19);
    }
    else
    {
        gpio_direction_input(CAT_PIN);  //cat in set input
    }   

    ret = gpio_request( BUTTON_PIN ,"BUTTON_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST BUTTON_PIN FAILED!\n");
        res |= (0x01<<20);
    }
    else
    {
        gpio_direction_input(BUTTON_PIN);  //button in set input
    }  

    ret = gpio_request( DIR485_PIN ,"DIR485_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST DIR485_PIN FAILED!\n");
        res |= (0x01<<21);
    }
    else
    {
        gpio_direction_input(DIR485_PIN);  //rs485 dir in set input
    }

    ret = gpio_request( TAMPER_PIN ,"TAMPER_PIN");
    if(ret < 0)
    {
        printk(KERN_EMERG "GPIO REQUEST TAMPER_PIN FAILED!\n");
        res |= (0x01<<22);
    }
    else
    {
        gpio_direction_input(TAMPER_PIN);  //tamper in set input
    }

    return res;                
}

static int test_drv_init(void)
{
    int res;
    printk(KERN_EMERG "MODULE test_drv init!\n");
    res = gpio_init();
    if(res != 0)
    {
        printk("GPIO input and output init error,error code %d!\n",res);
        return res;
    }

    major = register_chrdev(0,"test_drv",&test_drv_ops);

    jt_gpio_class = class_create(THIS_MODULE,"jt_gpio_class");

    if(!jt_gpio_class)
    {
        printk(KERN_EMERG "jt gpio class create failed!\n");
        return -1;
    }

    jt_gpio_device = device_create(jt_gpio_class,NULL,MKDEV(major,0),NULL,"test_drv");
    if(!jt_gpio_device)
    {
        printk(KERN_EMERG "jt gpio device create failed!\n");
        return -1;
    }

    return 0;
}

static void test_drv_exit(void)
{
    printk(KERN_EMERG "module test_drv exit!\n");
    
    unregister_chrdev(major,"test_drv");
    gpio_free(LED1_PIN);
    gpio_free( RD1_D1_PIN );
    gpio_free( RD1_D0_PIN );
    gpio_free( RD1_GLED_PIN );
    gpio_free( RD1_RLED_PIN );
    gpio_free( RD2_D1_PIN );
    gpio_free( RD2_D0_PIN );
    gpio_free( RD2_GLED_PIN );
    gpio_free( RD2_RLED_PIN );
    gpio_free( TAMPER_PIN );
    gpio_free( PSU_PIN );
    gpio_free( CAT_PIN );
    gpio_free( BUT_LED_PIN);
    gpio_free( BUTTON_PIN );
    gpio_free( ARM_PIN );
    gpio_free( SENSER_PIN );
    gpio_free( RELAY1_PIN );
    gpio_free( RELAY2_PIN );
    gpio_free( RELAY3_PIN );
    gpio_free( SOUNDER_PIN );
    gpio_free( BUZZER_PIN );
    gpio_free( DIR485_PIN );
    gpio_free( OUT12V_PIN );


    device_unregister(jt_gpio_device);
    class_destroy(jt_gpio_class);

    del_timer(&test_timer);

}



module_init(test_drv_init);
module_exit(test_drv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("WANG");







