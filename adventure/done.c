/*  $NetBSD: done.c,v 1.8 2003/08/07 09:36:50 agc Exp $ */

/*-
 * Copyright (c) 1991, 1993
 *  The Regents of the University of California.  All rights reserved.
 *
 * The game adventure was originally written in Fortran by Will Crowther
 * and Don Woods.  It was later translated to C and enhanced by Jim
 * Gillogly.  This code is derived from software contributed to Berkeley
 * by Jim Gillogly at The Rand Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

//static char sccsid[] = "@(#)done.c    8.1 (Berkeley) 5/31/93";
//__RCSID("$NetBSD: done.c,v 1.8 2003/08/07 09:36:50 agc Exp $");

#include "hdr.h"
#include "extern.h"

#include "show.h"

int
score()
{               /* sort of like 20000 */
    int     scor, i;
    mxscor = scor = 0;
    for (i = 50; i <= maxtrs; i++) {
        if (ptext[i].txtlen == 0)
            continue;
        k = 12;
        if (i == chest)
            k = 14;
        if (i > chest)
            k = 16;
        if (prop[i] >= 0)
            scor += 2;
        if (place[i] == 3 && prop[i] == 0)
            scor += k - 2;
        mxscor += k;
    }
    scor += (maxdie - numdie) * 10;
    mxscor += maxdie * 10;
    if (!(scorng || gaveup))
        scor += 4;
    mxscor += 4;
    if (dflag != 0)
        scor += 25;
    mxscor += 25;
    if (closng)
        scor += 25;
    mxscor += 25;
    if (closed) {
        if (bonus == 0)
            scor += 10;
        if (bonus == 135)
            scor += 25;
        if (bonus == 134)
            scor += 30;
        if (bonus == 133)
            scor += 45;
    }
    mxscor += 45;
    if (place[magzin] == 108)
        scor++;
    mxscor++;
    scor += 2;
    mxscor += 2;
    for (i = 1; i <= hntmax; i++)
        if (hinted[i])
            scor -= hints[i][2];
    return (scor);
}

void
done(entry)     /* entry=1 means goto 13000 */  /* game is over */
    int     entry;  /* entry=2 means goto 20000 */  /* 3=19000 */
{
    int     i, sc;
    if (entry == 1)
        mspeak(1);
    if (entry == 3)
        rspeak(136);
    show_string("\n\n\nYou scored ");
    show_int((sc = score()));
    show_string(" out of a possible ");
    show_int(mxscor);
    show_string(" using ");
    show_int(turns);
    show_string(" turns.\n");
    for (i = 1; i <= clsses; i++)
        if (cval[i] >= sc) {
            speak(&ctext[i]);
            if (i == clsses - 1) {
                show_string("To achieve the next higher rating");
                show_string(" would be a neat trick!\n\n");
                show_string("Congratulations!!\n");
                while(1) continue;
            }
            k = cval[i] + 1 - sc;
            show_string("To achieve the next higher rating, you need ");
            show_int(k);
            show_string(" more point");
            if (k == 1)
                show_string(".\n");
            else
                show_string("s.\n");
            while(1) continue;
        }
    show_string("You just went off my scale!!!\n");
    while(1) continue;
}


void
die(entry)          /* label 90 */
    int     entry;
{
    int     i;
    if (entry != 99) {
        rspeak(23);
        oldlc2 = loc;
    }
    if (closng) {       /* 99 */
        rspeak(131);
        numdie++;
        done(2);
    }
    yea = yes(81 + numdie * 2, 82 + numdie * 2, 54);
    numdie++;
    if (numdie == maxdie || !yea)
        done(2);
    place[water] = 0;
    place[oil] = 0;
    if (toting(lamp))
        prop[lamp] = 0;
    for (i = 100; i >= 1; i--) {
        if (!toting(i))
            continue;
        k = oldlc2;
        if (i == lamp)
            k = 1;
        drop(i, k);
    }
    loc = 3;
    oldloc = loc;
}