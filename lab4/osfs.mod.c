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
	{ 0x9ed5252c, "inode_init_owner" },
	{ 0xa61fd7aa, "__check_object_size" },
	{ 0x58b4ca2a, "d_instantiate" },
	{ 0x552e0ea6, "new_inode" },
	{ 0x0e142fe9, "unregister_filesystem" },
	{ 0xbf3a3510, "simple_statfs" },
	{ 0x6d3c3445, "d_make_root" },
	{ 0x6dcc2e5c, "d_splice_alias" },
	{ 0xa2c4ecbf, "current_time" },
	{ 0x8fe5f808, "iput" },
	{ 0x0e142fe9, "register_filesystem" },
	{ 0x180fd053, "simple_inode_init_ts" },
	{ 0xe8213e80, "_printk" },
	{ 0xaa47b76e, "__arch_copy_to_user" },
	{ 0x9a479b25, "make_kuid" },
	{ 0x30c65558, "strnlen" },
	{ 0x90a48d82, "__ubsan_handle_out_of_bounds" },
	{ 0xd7a59a65, "vmalloc_noprof" },
	{ 0x54ae3a7c, "set_nlink" },
	{ 0xc397dc30, "strncmp" },
	{ 0x23801573, "from_kgid" },
	{ 0xc609ff70, "strncpy" },
	{ 0xe54e0a6b, "__fortify_panic" },
	{ 0xf373df81, "default_llseek" },
	{ 0xb4c39fc9, "from_kuid" },
	{ 0x0e023ac7, "memset" },
	{ 0x013841b3, "__insert_inode_hash" },
	{ 0xafee03d8, "make_kgid" },
	{ 0x2a482d2f, "mount_nodev" },
	{ 0xf1de9e85, "vfree" },
	{ 0xaaaa20e7, "generic_delete_inode" },
	{ 0xb0a6f9c0, "generic_file_open" },
	{ 0x9c4ed43a, "alt_cb_patch_nops" },
	{ 0xde9b4ca2, "d_parent_ino" },
	{ 0xbd4bf510, "__mark_inode_dirty" },
	{ 0xf373df81, "generic_file_llseek" },
	{ 0xe4118690, "inode_set_ctime_to_ts" },
	{ 0x86c49e96, "nop_mnt_idmap" },
	{ 0x425ef116, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x9ed5252c,
	0xa61fd7aa,
	0x58b4ca2a,
	0x552e0ea6,
	0x0e142fe9,
	0xbf3a3510,
	0x6d3c3445,
	0x6dcc2e5c,
	0xa2c4ecbf,
	0x8fe5f808,
	0x0e142fe9,
	0x180fd053,
	0xe8213e80,
	0xaa47b76e,
	0x9a479b25,
	0x30c65558,
	0x90a48d82,
	0xd7a59a65,
	0x54ae3a7c,
	0xc397dc30,
	0x23801573,
	0xc609ff70,
	0xe54e0a6b,
	0xf373df81,
	0xb4c39fc9,
	0x0e023ac7,
	0x013841b3,
	0xafee03d8,
	0x2a482d2f,
	0xf1de9e85,
	0xaaaa20e7,
	0xb0a6f9c0,
	0x9c4ed43a,
	0xde9b4ca2,
	0xbd4bf510,
	0xf373df81,
	0xe4118690,
	0x86c49e96,
	0x425ef116,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"inode_init_owner\0"
	"__check_object_size\0"
	"d_instantiate\0"
	"new_inode\0"
	"unregister_filesystem\0"
	"simple_statfs\0"
	"d_make_root\0"
	"d_splice_alias\0"
	"current_time\0"
	"iput\0"
	"register_filesystem\0"
	"simple_inode_init_ts\0"
	"_printk\0"
	"__arch_copy_to_user\0"
	"make_kuid\0"
	"strnlen\0"
	"__ubsan_handle_out_of_bounds\0"
	"vmalloc_noprof\0"
	"set_nlink\0"
	"strncmp\0"
	"from_kgid\0"
	"strncpy\0"
	"__fortify_panic\0"
	"default_llseek\0"
	"from_kuid\0"
	"memset\0"
	"__insert_inode_hash\0"
	"make_kgid\0"
	"mount_nodev\0"
	"vfree\0"
	"generic_delete_inode\0"
	"generic_file_open\0"
	"alt_cb_patch_nops\0"
	"d_parent_ino\0"
	"__mark_inode_dirty\0"
	"generic_file_llseek\0"
	"inode_set_ctime_to_ts\0"
	"nop_mnt_idmap\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "3FF1D3B5667BDE1A56D3457");
