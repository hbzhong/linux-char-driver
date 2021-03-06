/***********************
  name: leds.c
  copyright: loop
  build date: 2010.3.7
************************/


#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/errno.h>


#define DEVICE_NAME "led"
#define LED_MAJOR  231

/* 用来指定LED所用的GPIO引脚 */
static unsigned long led_table [] =
{
	S3C2410_GPB5,
	S3C2410_GPB6,
	S3C2410_GPB7,
	S3C2410_GPB8,
};

/* 用来指定GPIO引脚的功能：输出 */
static unsigned int led_cfg_table [] =
{
	S3C2410_GPB5_OUTP,
	S3C2410_GPB6_OUTP,
	S3C2410_GPB7_OUTP,
	S3C2410_GPB8_OUTP,
};

static int Led_open(
	struct inode *inode,
	struct file  *file)
{
	int i;
	for (i=0; i<4; i++)
	{
		s3c2410_gpio_cfgpin(led_table[i], led_cfg_table[i]);
	}
	return 0;
}

static int Led_ioctl(
	struct inode *inode, 
	struct file *file, 
	unsigned int cmd, 
	unsigned long arg)
{
	if (arg > 4)
	{
		return -EINVAL;
	}

	switch(cmd)
	{
		case 0:
			// 设置指定引脚的输出电平为0
			s3c2410_gpio_setpin(led_table[arg], 0);
			return 0;

		case 1:
			// 设置指定引脚的输出电平为1
			s3c2410_gpio_setpin(led_table[arg], 1);
			return 0;

		default:
			return -EINVAL;
	}
}

static struct file_operations leds_fops = {
	.owner	=	THIS_MODULE,
	.open   =       Led_open,
	.ioctl	=	Led_ioctl,
};

static int __init Led_init(void)
{
	int ret;
	ret = register_chrdev(LED_MAJOR, DEVICE_NAME, leds_fops);
	if (ret < 0)
	{
		printk (DEVICE_NAME, "can't register\n");	
	}

	printk (DEVICE_NAME, "Initialized \n");
	return 0;
}

static void __exit Led_exit(void)
{	
	printk(DEVICE_NAME, "exit\n");
	unregister_chrdev(LED_MAJOR, DEVICE_NAME);
}

module_init(Led_init);
module_exit(Led_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("loop zhong");
MODULE_DESCRIPTION("GPIO control for EmbedSky SKY2440/TQ2440 Board");



