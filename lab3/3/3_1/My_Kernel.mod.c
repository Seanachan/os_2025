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
	{ 0x7e36375a, "proc_create" },
	{ 0xe8213e80, "_printk" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0xaa47b76e, "__arch_copy_to_user" },
	{ 0x9c4ed43a, "alt_cb_patch_nops" },
	{ 0x425ef116, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x7e36375a,
	0xe8213e80,
	0xa61fd7aa,
	0xaa47b76e,
	0x9c4ed43a,
	0x425ef116,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"proc_create\0"
	"_printk\0"
	"__check_object_size\0"
	"__arch_copy_to_user\0"
	"alt_cb_patch_nops\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "83A58D9E6E525FC1D73071F");
