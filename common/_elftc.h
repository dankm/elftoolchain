/*-
 * Copyright (c) 2009 Joseph Koshy
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $Id$
 */

/**
 ** Miscellanous definitions needed by multiple components.
 **/

#ifndef	_ELFTC_H
#define	_ELFTC_H

#ifndef	NULL
#define NULL 	((void *) 0)
#endif

#ifndef	offsetof
#define	offsetof(T, M)		((int) &((T*) 0) -> M)
#endif

/*
 * Supply macros missing from <sys/queue.h>
 */

#ifndef	STAILQ_FOREACH_SAFE
#define STAILQ_FOREACH_SAFE(var, head, field, tvar)            \
       for ((var) = STAILQ_FIRST((head));                      \
            (var) && ((tvar) = STAILQ_NEXT((var), field), 1);  \
            (var) = (tvar))
#endif

#ifndef	STAILQ_LAST
#define STAILQ_LAST(head, type, field)                                  \
        (STAILQ_EMPTY((head)) ?                                         \
                NULL :                                                  \
                ((struct type *)(void *)                                \
                ((char *)((head)->stqh_last) - offsetof(struct type, field))))
#endif

#ifndef	TAILQ_FOREACH_SAFE
#define TAILQ_FOREACH_SAFE(var, head, field, tvar)                      \
	for ((var) = TAILQ_FIRST((head));                               \
	    (var) && ((tvar) = TAILQ_NEXT((var), field), 1);            \
	    (var) = (tvar))
#endif

/*
 * VCS Ids.
 */

#ifndef	ELFTC_VCSID

#if defined(__DragonFly__)
#define	ELFTC_VCSID(ID)		__RCSID(ID)
#endif

#if defined(__FreeBSD__)
#define	ELFTC_VCSID(ID)		__FBSDID(ID)
#endif

#if defined(__linux__) || defined(__GNU__) || defined(__GLIBC__)
#if defined(__GNUC__)
#define	ELFTC_VCSID(ID)		__asm__(".ident\t\"" ID "\"")
#else
#define	ELFTC_VCSID(ID)		/**/
#endif
#endif

#if defined(__NetBSD__)
#define	ELFTC_VCSID(ID)		__RCSID(ID)
#endif

#endif	/* ELFTC_VCSID */

/*
 * Provide an equivalent for getprogname(3).
 */

#ifndef	ELFTC_GETPROGNAME

#if defined(__DragonFly__) || defined(__FreeBSD__) || defined(__NetBSD__)

#include <stdlib.h>

#define	ELFTC_GETPROGNAME()	getprogname()

#endif	/* defined(__FreeBSD__) || defined(__NetBSD__) */


#if defined(__linux__)

/*
 * GLIBC based systems have a global 'char *' pointer referencing
 * the executable's name.
 */
extern const char *program_invocation_short_name;

#define	ELFTC_GETPROGNAME()	program_invocation_short_name

#endif	/* __linux__ */

#endif	/* ELFTC_GETPROGNAME */


/**
 ** Per-OS configuration.
 **/

#if defined(__DragonFly__)

#include <osreldate.h>
#include <sys/endian.h>

#define	ELFTC_BYTE_ORDER			_BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		_LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		_BIG_ENDIAN

#endif

#if defined(__linux__)

#include <endian.h>

#define	ELFTC_BYTE_ORDER			__BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		__LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		__BIG_ENDIAN

/*
 * Debian GNU/Linux is missing strmode(3).
 */
#define	ELFTC_HAVE_STRMODE			0

/* Whether we need to supply {be,le}32dec. */
#define ELFTC_NEED_BYTEORDER_EXTENSIONS		1

#define	roundup2	roundup

#endif	/* __linux__ */


#if defined(__FreeBSD__)

#include <osreldate.h>
#include <sys/endian.h>

#define	ELFTC_BYTE_ORDER			_BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		_LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		_BIG_ENDIAN

#define	ELFTC_HAVE_STRMODE			1
#if __FreeBSD_version <= 900000
#define	ELFTC_BROKEN_YY_NO_INPUT		1
#endif
#endif	/* __FreeBSD__ */


#if defined(__NetBSD__)

#include <sys/param.h>
#include <sys/endian.h>

#define	ELFTC_BYTE_ORDER			_BYTE_ORDER
#define	ELFTC_BYTE_ORDER_LITTLE_ENDIAN		_LITTLE_ENDIAN
#define	ELFTC_BYTE_ORDER_BIG_ENDIAN		_BIG_ENDIAN

#define	ELFTC_HAVE_STRMODE			1
#if __NetBSD_Version__ <= 599002100
/* from src/doc/CHANGES: flex(1): Import flex-2.5.35 [christos 20091025] */
/* and 5.99.21 was from Wed Oct 21 21:28:36 2009 UTC */
#  define ELFTC_BROKEN_YY_NO_INPUT		1
#endif
#endif	/* __NetBSD __ */

#endif	/* _ELFTC_H */
