#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mohammad El-Sabae");
MODULE_DESCRIPTION("Timer with good usec accuracy.");
MODULE_VERSION("0.1");

static struct hrtimer __my_timer;
static ktime_t __interval;
static struct timeval tv;
static bool flag_for_whatever;

enum hrtimer_restart timer_callback(struct hrtimer *timer)
{
	do_gettimeofday(&tv);

	// bind the timer to hard time requirements
	// based on a flag for whatever
	if(flag_for_whatever)
	{
		tv.tv_usec = 500000;
	}
	else
	{
		tv.tv_sec ++;
		tv.tv_usec = 0;
	}

	__interval = timeval_to_ktime(tv);
	hrtimer_start(timer, __interval, HRTIMER_MODE_ABS);

	//printk(KERN_INFO "tick.");

	// toggle
	flag_for_whatever = !flag_for_whatever;

	// now handle the flag and what it means while timer waits to expire
	if(flag_for_whatever)
	{

	}
	else
	{

	}

	return HRTIMER_NORESTART;
}

static int __init timer_init(void)
{
	flag_for_whatever = false;
	hrtimer_init(&__my_timer, CLOCK_REALTIME, HRTIMER_MODE_ABS);

	__my_timer.function = &timer_callback;
	__interval = ktime_get();

	do_gettimeofday(&tv);

	tv.tv_sec += 2;
	tv.tv_usec = 0;

	__interval = timeval_to_ktime(tv);
	hrtimer_start(&__my_timer, __interval, HRTIMER_MODE_ABS);
	return 0;
}

static void __exit timer_exit(void)
{
	hrtimer_cancel(&__my_timer);
	printk(KERN_INFO "exiting\n");
}

module_init(timer_init);
module_exit(timer_exit);

