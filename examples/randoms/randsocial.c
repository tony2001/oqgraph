/* Copyright (C) 2010 Arjen G Lentz for Open Query

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


/* random social network example (for testing) by arjen@openquery.com
   create a social network, N items, X friend connects, Y fan connects, max Z weight
   (for friend connects, items will end up with more as each creates X)
   [note: weight only applied to friend connections]
   [note: oqgraph ignores dup vertices]
   - gcc -o randsocial randsocial.c
   - ./randsocial 10000 3 1 2 >randsocial.sql
   - create a randsocial_graph OQGRAPH table
   - mysql -u username dbname <randsocial.sql
   - you'll now have a social network of the specified size to play with
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define sqlprint_fan(origid,destid) printf("INSERT INTO randsocial_graph (origid,destid) VALUES (%d,%d);\n", (origid), (destid))
#define sqlprint_friend(origid,destid,weight) printf("INSERT INTO randsocial_graph (origid,destid,weight) VALUES (%d,%d,%d),(%d,%d,%d);\n", \
						 (origid), (destid), (weight), (destid), (origid), (weight))


/* return random between 0 and n-1, not being x
*/
int randnotx (int n, int x)
{
	int r;

	do r = rand() % n;
	while (r == x);

	return (r);
}/*randnotx()*/


int main (int argc, char *argv[])
{
	int items, friends, fanof, maxweight;
	int i, j, r, w;

	if (argc != 5) {
		fprintf(stderr,"Usage: %s <items> <friends> <fan-of> <maxweight>\n",argv[0]);
		exit (1);
	}

	items	= atoi(argv[1]);
	friends	= atoi(argv[2]);
	fanof	= atoi(argv[3]);
	maxweight = atoi(argv[4]);
	if (items < 2 ||
		friends < 0 || friends >= items ||
		fanof < 0 || fanof >= items ||
		(!friends && !fanof) ||
		maxweight < 0
           ) {
		fprintf(stderr,"invalid network parameters\n");
		exit (1);
	}

	srand((unsigned) time(NULL));

	for (i = 0; i < items; i++) {
		for (j = 0; j < friends; j++) {
			// we don't want to stick the rand() call into the sqlprint macro, just in case we want to use it multiple times
			r = randnotx(items,i);
			w = maxweight > 0 ? (rand() % maxweight) : 0;
			sqlprint_friend(i,r,w);
		}
		for (j = 0; j < fanof; j++) {
			// we don't want to stick the rand() call into the sqlprint macro, just in case we want to use it multiple times
			r = randnotx(items,i);
			sqlprint_fan(i,r);
		}
	}

	exit (0);
}/*main()*/


/* end of randtree.c */
