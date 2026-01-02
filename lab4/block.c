#include <linux/fs.h>
#include <linux/uaccess.h>
#include "osfs.h"

/**
 * Function: osfs_get_block_number
 * Description: 將 logical block number 轉換為 physical block number，支援 multi-level indexing
 * Inputs:
 *   - inode: VFS inode
 *   - logical_block: 邏輯 block 編號
 *   - physical_block: 輸出的實體 block 編號
 *   - create: 1 表示如果 block 不存在則分配，0 表示只查詢
 * Returns:
 *   - 0 on success
 *   - -ENOSPC if allocation failed
 *   - -EFBIG if logical_block is too large
 */
int osfs_get_block_number(struct inode *inode, uint32_t logical_block, 
                          uint32_t *physical_block, int create)
{
    struct osfs_inode *osfs_inode = inode->i_private;
    struct osfs_sb_info *sb_info = inode->i_sb->s_fs_info;
    uint32_t *indirect_block;
    uint32_t *double_indirect_block;
    uint32_t indirect_index, double_index;
    int ret;

    // Case 1: Direct blocks (0-9)
    if (logical_block < OSFS_DIRECT_BLOCKS) {
        if (osfs_inode->i_block[logical_block] == 0) {
            if (!create) {
                *physical_block = 0;  // Sparse file: block not allocated
                return 0;
            }
            // Allocate new block
            ret = osfs_alloc_data_block(sb_info, &osfs_inode->i_block[logical_block]);
            if (ret != 0)
                return ret;
            osfs_inode->i_blocks++;
        }
        *physical_block = osfs_inode->i_block[logical_block];
        return 0;
    }

    logical_block -= OSFS_DIRECT_BLOCKS;

    // Case 2: Single indirect blocks (10-1033)
    if (logical_block < PTRS_PER_BLOCK) {
        // Check if single indirect block exists
        if (osfs_inode->i_block[OSFS_SINGLE_INDIRECT] == 0) {
            if (!create) {
                *physical_block = 0;
                return 0;
            }
            // Allocate indirect block
            ret = osfs_alloc_data_block(sb_info, &osfs_inode->i_block[OSFS_SINGLE_INDIRECT]);
            if (ret != 0)
                return ret;
            osfs_inode->i_blocks++;
            
            // Clear the indirect block
            indirect_block = (uint32_t *)(sb_info->data_blocks + 
                            osfs_inode->i_block[OSFS_SINGLE_INDIRECT] * BLOCK_SIZE);
            memset(indirect_block, 0, BLOCK_SIZE);
        }

        // Get pointer to indirect block
        indirect_block = (uint32_t *)(sb_info->data_blocks + 
                        osfs_inode->i_block[OSFS_SINGLE_INDIRECT] * BLOCK_SIZE);

        // Check if data block is allocated
        if (indirect_block[logical_block] == 0) {
            if (!create) {
                *physical_block = 0;
                return 0;
            }
            // Allocate data block
            ret = osfs_alloc_data_block(sb_info, &indirect_block[logical_block]);
            if (ret != 0)
                return ret;
            osfs_inode->i_blocks++;
        }
        *physical_block = indirect_block[logical_block];
        return 0;
    }

    logical_block -= PTRS_PER_BLOCK;

    // Case 3: Double indirect blocks (1034+)
    if (logical_block < PTRS_PER_BLOCK * PTRS_PER_BLOCK) {
        double_index = logical_block / PTRS_PER_BLOCK;
        indirect_index = logical_block % PTRS_PER_BLOCK;

        // Check if double indirect block exists
        if (osfs_inode->i_block[OSFS_DOUBLE_INDIRECT] == 0) {
            if (!create) {
                *physical_block = 0;
                return 0;
            }
            // Allocate double indirect block
            ret = osfs_alloc_data_block(sb_info, &osfs_inode->i_block[OSFS_DOUBLE_INDIRECT]);
            if (ret != 0)
                return ret;
            osfs_inode->i_blocks++;
            
            // Clear the double indirect block
            double_indirect_block = (uint32_t *)(sb_info->data_blocks + 
                                   osfs_inode->i_block[OSFS_DOUBLE_INDIRECT] * BLOCK_SIZE);
            memset(double_indirect_block, 0, BLOCK_SIZE);
        }

        // Get pointer to double indirect block
        double_indirect_block = (uint32_t *)(sb_info->data_blocks + 
                               osfs_inode->i_block[OSFS_DOUBLE_INDIRECT] * BLOCK_SIZE);

        // Check if single indirect block exists
        if (double_indirect_block[double_index] == 0) {
            if (!create) {
                *physical_block = 0;
                return 0;
            }
            // Allocate single indirect block
            ret = osfs_alloc_data_block(sb_info, &double_indirect_block[double_index]);
            if (ret != 0)
                return ret;
            osfs_inode->i_blocks++;
            
            // Clear the indirect block
            indirect_block = (uint32_t *)(sb_info->data_blocks + 
                            double_indirect_block[double_index] * BLOCK_SIZE);
            memset(indirect_block, 0, BLOCK_SIZE);
        }

        // Get pointer to single indirect block
        indirect_block = (uint32_t *)(sb_info->data_blocks + 
                        double_indirect_block[double_index] * BLOCK_SIZE);

        // Check if data block is allocated
        if (indirect_block[indirect_index] == 0) {
            if (!create) {
                *physical_block = 0;
                return 0;
            }
            // Allocate data block
            ret = osfs_alloc_data_block(sb_info, &indirect_block[indirect_index]);
            if (ret != 0)
                return ret;
            osfs_inode->i_blocks++;
        }
        *physical_block = indirect_block[indirect_index];
        return 0;
    }

    // File too large
    pr_err("osfs_get_block_number: logical_block %u exceeds maximum file size\n", logical_block);
    return -EFBIG;
}
