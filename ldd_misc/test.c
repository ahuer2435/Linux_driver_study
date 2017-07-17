#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

#include <linux/wait.h>
#include <linux/sched.h>

#include <asm/uaccess.h>
#include <linux/errno.h>

#define DEBUG_SWITCH    1
#if DEBUG_SWITCH
	#define P_DEBUG(fmt, args...)   printk("<1>" "<kernel>[%s]"fmt, __FUNCTION__, ##args)
#else
	#define P_DEBUG(fmt, args...)   printk("<7>" "<kernel>[%s]"fmt, __FUNCTION__, ##args)
#endif

#define DEV_SIZE 100

struct _data_t{
	char kbuf[DEV_SIZE];
        int elem_num;
        int space_num;
        wait_queue_head_t read_queue;	//1、定义读等待队列头
        wait_queue_head_t write_queue;	//1、定义写等待队列头
};

struct _data_t my_data;

int test_close(struct inode *node, struct file *filp)
{
	return 0;
}

ssize_t test_read(struct file *filp, char __user *buf, size_t count, loff_t *offset)
{
	int ret;

        if(filp->f_flags & O_NONBLOCK){
                return - EAGAIN;
        }

	/*休眠*/
	P_DEBUG("read data.....\n");
        P_DEBUG("dev->elem_num = %d.\n",my_data.elem_num);
        if(wait_event_interruptible(my_data.read_queue, my_data.elem_num > 0)){
                return - ERESTARTSYS;
        }

        if(count > my_data.elem_num){
            count = my_data.elem_num;
        }
        if (copy_to_user(buf, my_data.kbuf, count)){
		ret = - EFAULT;
	}else{
		ret = count;
                my_data.elem_num -= count;
                my_data.space_num += count;
                P_DEBUG("read %d bytes, str = %s, elem_num:[%d]\n", count, my_data.kbuf,my_data.elem_num);
                wake_up_interruptible(&my_data.write_queue);
	}
	return ret;		//返回实际写入的字节数或错误号
}

ssize_t test_write(struct file *filp, const char __user *buf, size_t count, loff_t *offset)
{
	int ret;

        P_DEBUG("write data.....\n");
        if(wait_event_interruptible(my_data.write_queue, my_data.space_num > 0)){
                return - ERESTARTSYS;
        }

        if(count > my_data.space_num){
            count = my_data.space_num;
        }
        if(copy_from_user(my_data.kbuf, buf, count)){
		ret = - EFAULT;
	}else{
		ret = count;
                my_data.elem_num += count;
                my_data.space_num -= count;
                P_DEBUG("write %d bytes, space_num:[%d]\n", count, my_data.space_num);
                P_DEBUG("kbuf is [%s]\n", my_data.kbuf);
		/*唤醒*/
                wake_up_interruptible(&my_data.read_queue);
	}

	return ret;		//返回实际写入的字节数或错误号
}

#define DEVICE_NAME "misc_dev"
struct file_operations dev_fops = {
	.release = test_close,
	.write = test_write,
	.read = test_read,
};

struct miscdevice misc_dev={
    .minor = MISC_DYNAMIC_MINOR,
    .name = DEVICE_NAME,
    .fops = &dev_fops,
};

static int __init test_init(void)	//模块初始化函数
{
	int result = 0;
        my_data.elem_num = 0;
        my_data.space_num = DEV_SIZE - my_data.elem_num;

        if(result < 0){
		P_DEBUG("register devno errno!\n");
		goto err0;
	}

        /*初始化等待队列头，注意函数调用的位置，需要放在cdev_add前面，使得使用设备时，系统已经准备好*/
        init_waitqueue_head(&my_data.read_queue);
        init_waitqueue_head(&my_data.write_queue);

        result = misc_register(&misc_dev);
	if(result < 0){
                P_DEBUG("misc_dev errno!\n");
                goto err0;
	}

	printk("hello kernel\n");
	return 0;

err0:
        misc_deregister(&misc_dev);
	return result;
}

static void __exit test_exit(void)		//模块卸载函数
{
        misc_deregister(&misc_dev);
	printk("good bye kernel\n");
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("qiao yan");
MODULE_VERSION("v0.1");
