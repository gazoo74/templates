/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Gaël PORTAY <gael.portay@savoirfairelinux.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef _PEXEC_H_
#define _PEXEC_H_

#include <unistd.h>
#include <sys/types.h>

int pexecl(pid_t *pid, const char *path, const char *arg, ...);
int pexeclp(pid_t *pid, const char *file, const char *arg, ...);
int pexecle(pid_t *pid, const char *path, const char *arg, ...);
int pexecv(pid_t *pid, const char *path, char * const argv[]);
int pexecvp(pid_t *pid, const char *file, char * const argv[]);
int pexecve(pid_t *pid, const char *path, char *const argv[],
	    char *const envp[]);

#ifdef _GNU_SOURCE
int pexecvpe(pid_t *pid, const char *file, char * const argv[],
	     char * const envp[]);
#endif

#endif
