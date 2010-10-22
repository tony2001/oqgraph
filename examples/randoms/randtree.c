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


/* random tree example (for testing) by arjen@openquery.com
   unidirectional to create a simple tree, no weights
   - gcc -o randtree randtree.c
   - ./randtree 10000 >randtree.sql
   - create a randtree_graph OQGRAPH table
   - mysql -u username dbname <randtree.sql
   - you'll now have a tree to play with, root node 0, items <= 9999
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define sqlprint(origid,destid) printf("INSERT INTO randtree_graph (origid,destid) VALUES (%d,%d);\n", (origid), (destid))



int main (int argc, char *argv[])
{
	int items;
	int i, r;

	if (argc != 2) {
		fprintf(stderr,"Usage: %s <items>\n",argv[0]);
		exit (1);
	}

	items = atoi(argv[1]);
	if (items < 2) {
		fprintf(stderr,"too small\n");
		exit (1);
	}

	srand((unsigned) time(NULL));

	for (i = 1; i < items; i++) {
		// we don't want to stick the rand() call into the sqlprint macro, just in case we want to use it multiple times
		r = rand() % i;
		sqlprint(r,i);
	}

	exit (0);
}/*main()*/


/* end of randtree.c */
