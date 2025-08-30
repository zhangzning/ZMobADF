/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <unistd.h>
#include <android/fdsan.h>
#include "private/bionic_defs.h"
#include "private/bionic_fdtrack.h"
#include "pthread_internal.h"
// ZZNADD
#include <async_safe/log.h>
#include <async_safe/async_safe_send.h>
#include <string.h>
// ZZNADD END

__BIONIC_WEAK_FOR_NATIVE_BRIDGE_INLINE
int __clone_for_fork() {
  pthread_internal_t* self = __get_thread();
  int result = clone(nullptr, nullptr, (CLONE_CHILD_SETTID | CLONE_CHILD_CLEARTID | SIGCHLD), nullptr, nullptr, nullptr, &(self->tid));
  if (result == 0) {
    self->set_cached_pid(gettid());
  }
  return result;
}

int fork() {
  __bionic_atfork_run_prepare();
  int result = __clone_for_fork();
  // ZZNADD 
  pid_t pid_t_type = getpid();
  int pid_int = static_cast<int>(pid_t_type);
  ZMobADF_HEADER zh = initZMobADF_HEADER(0, 0, 0, pid_int, 1, ZMobADF_PILE_TYPE_FORK);
  setLength(2, zh.data_length);   // fork 出来的pid长度不超过2字节，所以设置data长度为2
  setLength(sizeof(zh) + 2, zh.total_length); // 总长度 = 头长度 + 数据长度
  // fork的结果pid
  unsigned char fork_pid[2];
  fork_pid[0] = (result >> 8) & 0xFF;
  fork_pid[1] = result & 0xFF;
  // 拼接结果
  size_t total_size = sizeof(zh) + sizeof(fork_pid);
  unsigned char result_char[total_size];
  memcpy(result_char, &zh, sizeof(zh));
  memcpy(result_char + sizeof(zh), &fork_pid, sizeof(fork_pid));
  // ZZNADD END
  if (result == 0) {
    android_fdsan_set_error_level(ANDROID_FDSAN_ERROR_LEVEL_DISABLED);
    android_fdtrack_set_globally_enabled(false);
    __set_stack_and_tls_vma_name(true);
    __bionic_atfork_run_child();
  } else {
    __bionic_atfork_run_parent();
  }
  return result;
}
