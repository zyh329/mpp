/*
 * Copyright 2010 Rockchip Electronics S.LSI Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define MODULE_TAG "rk_malloc"
#include "rk_type.h"
#include "rk_log.h"
#include "rk_env.h"
#include "rk_malloc.h"
#include "rk_list.h"

#include "os_malloc.h"

// export configure for script detection
#define CONFIG_OSAL_MEM_LIST        "osal_mem_list"
#define CONFIG_OSAL_MEM_STUFF       "osal_mem_stuff"


// default memory align size is set to 32
#define RK_OSAL_MEM_ALIGN       32

// osal_mem_flag bit mask
#define OSAL_MEM_LIST_EN        (0x00000001)
#define OSAL_MEM_STUFF_EN       (0x00000002)

static RK_S32 osal_mem_flag = -1;
static struct list_head mem_list;

struct mem_node {
    struct list_head list;
    void *ptr;
    size_t size;
};

static void get_osal_mem_flag()
{
    if (osal_mem_flag < 0) {
        RK_U32 val;
        rk_get_env_u32(CONFIG_OSAL_MEM_LIST, &val, 0);
        if (val) {
            osal_mem_flag |= OSAL_MEM_LIST_EN;
        }
        rk_get_env_u32(CONFIG_OSAL_MEM_STUFF, &val, 0);
        if (val) {
            osal_mem_flag |= OSAL_MEM_STUFF_EN;
        }
        INIT_LIST_HEAD(&mem_list);
    }
}

void *rk_mpp_malloc(char *tag, size_t size)
{
    void *ptr;
    get_osal_mem_flag();
    if (0 == os_malloc(&ptr, RK_OSAL_MEM_ALIGN, size))
        return ptr;
    else
        return NULL;
}

void rk_mpp_free(void *ptr)
{
    get_osal_mem_flag();
    os_free(ptr);
}
