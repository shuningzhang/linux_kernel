
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/configfs.h>

struct itworld123_test_group {
	struct configfs_subsystem cs_subsys;

};

static struct config_group  *itworld123_make_group(struct config_group *group,
						   const char *name)
{
	printk("make group\n");
	return 0;
}

static void itworld123_drop_item(struct config_group *group, 
				 struct config_item *item)
{
	printk("drop item\n");
}

static struct configfs_group_operations itworld123_group_group_ops = {
	.make_group = itworld123_make_group,
	.drop_item = itworld123_drop_item,
};

static struct config_item_type itworld123_group_type = {
	.ct_group_ops = &itworld123_group_group_ops,
	.ct_owner = THIS_MODULE,
};

static struct itworld123_test_group itworld123_test_group = {
	.cs_subsys = {
		.su_group = {
			.cg_item = {
				.ci_namebuf = "test",
				.ci_type = &itworld123_group_type,
			},
		},
	},

};

static int configfsp_init(void)
{
	int ret = -1;

	printk("Hello, itworld123.com \n");
	config_group_init(&itworld123_test_group.cs_subsys.su_group);
	mutex_init(&itworld123_test_group.cs_subsys.su_mutex);
	ret = configfs_register_subsystem(&itworld123_test_group.cs_subsys);
	if ( ret ) {
		printk("configfs register failed!\n");
	}
	
	return 0;
}

static void configfsp_exit(void)
{
	configfs_unregister_subsystem(&itworld123_test_group.cs_subsys);
	printk("Bye!\n");

}

module_init(configfsp_init);
module_exit(configfsp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SunnyZhang<shuningzhang@126.com>");

