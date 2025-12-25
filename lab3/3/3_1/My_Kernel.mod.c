#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x9479a1e8, "strnlen" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x092a35a2, "_copy_to_user" },
	{ 0xd272d446, "__rcu_read_lock" },
	{ 0x4d40f3a6, "const_pcpu_hot" },
	{ 0x40a621c5, "snprintf" },
	{ 0xd272d446, "__rcu_read_unlock" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0xd272d446, "__fentry__" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0x82c6f73b, "proc_create" },
	{ 0xe8213e80, "_printk" },
	{ 0xba157484, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x9479a1e8,
	0xa61fd7aa,
	0x092a35a2,
	0xd272d446,
	0x4d40f3a6,
	0x40a621c5,
	0xd272d446,
	0xe54e0a6b,
	0xd272d446,
	0xd272d446,
	0x82c6f73b,
	0xe8213e80,
	0xba157484,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"strnlen\0"
	"__check_object_size\0"
	"_copy_to_user\0"
	"__rcu_read_lock\0"
	"const_pcpu_hot\0"
	"snprintf\0"
	"__rcu_read_unlock\0"
	"__fortify_panic\0"
	"__fentry__\0"
	"__x86_return_thunk\0"
	"proc_create\0"
	"_printk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7E12567D3417B5E2A20E4D6");
