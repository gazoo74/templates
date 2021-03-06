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

#include "pexec.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int vpexecl(pid_t *pid, const char *path, int argc, const char *arg0, va_list ap);
int vpexeclp(pid_t *pid, const char *file, int argc, const char *arg0, va_list ap);
int vpexecle(pid_t *pid, const char *path, int argc, const char *arg0, va_list ap);

int vpexecl(pid_t *pid, const char *path, int argc, const char *argv0, va_list ap)
{
	int i;
	char *argv[argc+1];
	va_list ap2;

	/* Construct argv */
	va_copy(ap2, ap);
	argv[0] = (char *)argv0;
	for (i = 1; i < argc; i++)
		argv[i] = va_arg(ap2, char *);
	argv[i] = NULL;
	va_end(ap2);

	return pexecv(pid, path, argv);
}

int vpexeclp(pid_t *pid, const char *file, int argc, const char *argv0, va_list ap)
{
	int i;
	char *argv[argc+1];
	va_list ap2;

	/* Construct argv */
	va_copy(ap2, ap);
	argv[0] = (char *)argv0;
	for (i = 1; i < argc; i++)
		argv[i] = va_arg(ap2, char *);
	argv[i] = NULL;
	va_end(ap2);

	return pexecvp(pid, file, argv);
}

int vpexecle(pid_t *pid, const char *path, int argc, const char *argv0, va_list ap)
{
	int i;
	char *argv[argc+1];
	char **envp;
	va_list ap2;

	/* Construct argv and set envp*/
	va_copy(ap2, ap);
	argv[0] = (char *)argv0;
	for (i = 1; i < argc; i++)
		argv[i] = va_arg(ap2, char *);
	argv[i] = NULL;
	envp = va_arg(ap, char **);
	va_end(ap2);

	return pexecve(pid, path, argv, envp);
}

int pexecl(pid_t *pid, const char *path, const char *argv, ...)
{
	int argc;
	va_list ap;

	/* Count variadic arguments */
	va_start(ap, argv);
	for (argc = 1; va_arg(ap, const char *); argc++);
	va_end(ap);

	return vpexecl(pid, path, argc, argv, ap);
}

int pexeclp(pid_t *pid, const char *file, const char *argv, ...)
{
	int argc;
	va_list ap;

	/* Count variadic argument */
	va_start(ap, argv);
	for (argc = 1; va_arg(ap, const char *); argc++);
	va_end(ap);

	return vpexeclp(pid, file, argc, argv, ap);
}

int pexecle(pid_t *pid, const char *path, const char *argv, ...)
{
	int argc;
	va_list ap;

	/* Count variadic argument */
	va_start(ap, argv);
	for (argc = 1; va_arg(ap, const char *); argc++);
	va_end(ap);

	return vpexecle(pid, path, argc, argv, ap);
}

int pexecv(pid_t *pid, const char *path, char * const argv[])
{
	int error;
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");

		error = errno;
		close(fd[0]);
		close(fd[1]);
		errno = error;

		return -1;
	}

	/* Parent */
	if (p) {
		if (pid)
			*pid = p;

		close(fd[1]);
		return fd[0];
	}

	/* Child */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	open("/dev/null", O_RDONLY);
	dup2(fd[1], STDOUT_FILENO);
	open("/dev/null", O_WRONLY);

	close(fd[0]);
	close(fd[1]);

	execv(path, argv);
	_exit(127);
}

int pexecvp(pid_t *pid, const char *file, char * const argv[])
{
	int error;
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");

		error = errno;
		close(fd[0]);
		close(fd[1]);
		errno = error;

		return -1;
	}

	/* Parent */
	if (p) {
		if (pid)
			*pid = p;

		close(fd[1]);
		return fd[0];
	}

	/* Child */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	open("/dev/null", O_RDONLY);
	dup2(fd[1], STDOUT_FILENO);
	open("/dev/null", O_WRONLY);

	close(fd[0]);
	close(fd[1]);

	execvp(file, argv);
	_exit(127);
}

int pexecve(pid_t *pid, const char *path, char *const argv[], char *const envp[])
{
	int error;
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");

		error = errno;
		close(fd[0]);
		close(fd[1]);
		errno = error;

		return -1;
	}

	/* Parent */
	if (p) {
		if (pid)
			*pid = p;

		close(fd[1]);
		return fd[0];
	}

	/* Child */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	open("/dev/null", O_RDONLY);
	dup2(fd[1], STDOUT_FILENO);
	open("/dev/null", O_WRONLY);

	close(fd[0]);
	close(fd[1]);

	execve(path, argv, envp);
	_exit(127);
}

#ifdef _GNU_SOURCE
int pexecvpe(pid_t *pid, const char *file, char * const argv[], char * const envp[])
{
	int error;
	int fd[2];
	pid_t p;

	if (pipe(fd) == -1) {
		perror("pipe");
		return -1;
	}

	p = fork();
	if (p == -1) {
		perror("fork");

		error = errno;
		close(fd[0]);
		close(fd[1]);
		errno = error;

		return -1;
	}

	/* Parent */
	if (p) {
		if (pid)
			*pid = p;

		close(fd[1]);
		return fd[0];
	}

	/* Child */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	open("/dev/null", O_RDONLY);
	dup2(fd[1], STDOUT_FILENO);
	open("/dev/null", O_WRONLY);

	close(fd[0]);
	close(fd[1]);

	execvpe(file, argv, envp);
	_exit(127);
}
#endif
