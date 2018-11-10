#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>

void bubble_sort(int *array, unsigned int size)
{
	int i = 0;
	int j = 0;
	int temp = 0;

	for (i = 0; i < size; i++ ) {
		for (j = 0; j < size - i - 1; j++) {

			if (array[j] < array[j + 1]) {
				temp = array[j];
				array[j] = array[j+1];
				array[j+1] = temp;
			}
		}	
	}
}

void print_array(int *array, unsigned int size, char *prefix)
{
	int i = 0;
	
	printk(KERN_NOTICE "%s, size: %d", prefix, size);
	for (i = 0; i < size; i++) {
		printk(KERN_NOTICE "%d ", array[i]);
	}
}

static int bubble_init(void)
{
	int array[] = {1, 3, 5, 7, 9, 2, 4, 6, 8};
	int array_len = sizeof(array)/sizeof(int);
	
	print_array(array, array_len, "Before sort");
	bubble_sort(array, array_len);	
	print_array(array, array_len, "After sort");

	return 0;
}

static void bubble_exit(void)
{
	printk("Bye!\n");

}

module_init(bubble_init);
module_exit(bubble_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

