/*
 * Rh850 exception helper routines
 *
 * Copyright (c) 2011-2012 Jia Liu <proljc@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "exec/exec-all.h"
#include "exception.h"

void HELPER(exception)(CPURh850State *env, uint32_t excp)
{
    Rh850CPU *cpu = rh850_env_get_cpu(env);

    raise_exception(cpu, excp);
}

static void QEMU_NORETURN do_range(CPURh850State *env, uintptr_t pc)
{
    Rh850CPU *cpu = rh850_env_get_cpu(env);
    CPUState *cs = CPU(cpu);

    cs->exception_index = EXCP_RANGE;
    cpu_loop_exit_restore(cs, pc);
}

void HELPER(ove_cy)(CPURh850State *env)
{
    if (env->sr_cy) {
        do_range(env, GETPC());
    }
}

void HELPER(ove_ov)(CPURh850State *env)
{
    if (env->sr_ov < 0) {
        do_range(env, GETPC());
    }
}

void HELPER(ove_cyov)(CPURh850State *env)
{
    if (env->sr_cy || env->sr_ov < 0) {
        do_range(env, GETPC());
    }
}
