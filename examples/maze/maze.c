/* Copyright (C) 2009 Arjen G Lentz for Open Query

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


/* quick maze example by arjen@openquery.com
   like (deep-recursive!) depth-first algo from
   http://en.wikipedia.org/wiki/Maze_generation_algorithm
   1. Start at a particular cell and call it the "exit."
   2. Mark the current cell as visited, and get a list of its neighbors.
      For each neighbor, starting with a randomly selected neighbor:
      a. If that neighbor hasn't been visited,
         remove the wall between this cell and that neighbor,
         and then recurse with that neighbor as the current cell.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int width, height;
int *maze;
enum { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8 };
#define roomofs(x,y) (((y) * width) + (x))
#define roomptr(x,y) (maze + roomofs((x),(y)))
#define isdoor(x,y,direction) ((*roomptr((x),(y))) & direction)
#define varprint(side,x,y) printf("SET @maze_exit_%c = %d;  -- (%d,%d)\n",(side),roomofs((x),(y)),(x),(y))
#define sqlprint(x1,y1,x2,y2) printf("INSERT INTO maze_graph (origid,destid) VALUES (%d,%d);  -- (%d,%d) -> (%d,%d)\n",roomofs((x1),(y1)),roomofs((x2),(y2)),(x1),(y1),(x2),(y2))


void roomwalk (int x, int y)
{
	int direction_order[4];
	int nx, ny, nd;
	int i, j, r;

	// all directions, in random order - yes this is dumb&crude
	for (i = 0; i < 4; i++) {
		do {		
			r = 1 << (rand() % 4);
			for (j = 0; j < i && r != direction_order[j]; j++);
		} while (j < i);

		direction_order[i] = r;
	}

	for (i = 0; i < 4; i++) {
		switch (direction_order[i]) {
			case NORTH:
				nx = x;
				ny = y - 1;
				nd = SOUTH;
				break;
			case EAST:
				nx = x + 1;
				ny = y;
				nd = WEST;
				break;
			case SOUTH:
				nx = x;
				ny = y + 1;
				nd = NORTH;
				break;
			case WEST:
				nx = x - 1;
				ny = y;
				nd = EAST;
				break;
		}

		// boundary, don't walk outside ;-)
		if (nx < 0 || nx >= width || ny < 0 || ny >= height)
			continue;

		if (!*roomptr(nx,ny)) {	// only unvisited
			*roomptr(x,y) |= direction_order[i];
			*roomptr(nx,ny) |= nd;
			// walk into this room (recurse)
			roomwalk(nx,ny);
		}
	}
}/*roomwalk()*/


// mazeprint() can deal with the exits being anywhere
void mazeprint (void)
{
	int x, y;

	for (y = 0; y < height; y++) {
		if (y == 0) {	// N edge
			putc('+',stderr);	// NW corner
			for (x = 0; x < width; x++) {
				putc(isdoor(x,y,NORTH) ? ' ' : '-',stderr);
				putc('+',stderr);	// E between edge
			}
			putc('\n',stderr);
		}

		// W edge
		putc(isdoor(0,y,WEST) ? ' ' : '|',stderr);

		for (x = 0; x < width; x++) {
			putc(' ',stderr);	// room
			putc(isdoor(x,y,EAST) ? ' ' : '|',stderr);
		}
		putc('\n',stderr);

		for (x = 0; x < width; x++) {
			putc('+',stderr);	// W inbetween edge
			putc(isdoor(x,y,SOUTH) ? ' ' : '-',stderr);
		}
		putc('+',stderr);		// E inbetween edge
		putc('\n',stderr);
	}
}/*mazeprint()*/


// mazesql() doesn't currently print the exits
void mazesql (void)
{
	int x, y;
	int room;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			room = *roomptr(x,y);
			if (y > 0 && (room & NORTH)) sqlprint(x,y,x,y-1);
			if (x < width && (room & EAST)) sqlprint(x,y,x+1,y);
			if (y < height && (room & SOUTH)) sqlprint(x,y,x,y+1);
			if (x > 0 && (room & WEST)) sqlprint(x,y,x-1,y);
		}
	}
}/*mazesql()*/



int main (int argc, char *argv[])
{
	int x,y;

	if (argc != 3) {
		fprintf(stderr,"Usage: %s <width> <height>\n",argv[0]);
		exit (1);
	}

	width = atoi(argv[1]);
	height = atoi(argv[2]);
	if (width < 3 || height < 3) {
		fprintf(stderr,"too small\n");
		exit (1);
	}
	printf("-- maze width=%d height=%d\n",width,height);

	if ((maze = calloc(width * height,sizeof (int))) == NULL) {
		fprintf(stderr,"can't allocate maze\n");
		exit (1);
	}

	srand((unsigned) time(NULL));

	// start from W edge, just, because
	roomwalk(0, rand() % height);

	// our two exits: one on W, one on E
	x = 0;
	y = rand() % height;
	*roomptr(x,y) |= WEST;
	varprint('w',x,y);

	x = width - 1;
	y = rand() % height;
	*roomptr(x,y) |= EAST;
	varprint('e',x,y);

	// rest of SQL paths
	mazesql();

	// ascii maze print
	mazeprint();

	free(maze);

	exit (0);
}/*main()*/


/* end of maze.c */
