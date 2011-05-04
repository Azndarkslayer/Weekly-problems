#include <stdio.h>
#include <stdlib.h>

/*-------------------- DEFINES --------------------*/

#define MAXX 5
#define MAXY 3

// same as
// const int MAXX=5
// const int MAXY=3

/*-------------------- GLOBALS --------------------*/

char board[MAXY+2][MAXX+2] =
   {
     "       ",
     " 23231 ",
     " 21131 ",
     " 33131 ",
     "       "
   };

int soly[50];
int solx[50];
int lastsol=0;


/*-------------- void print_board() --------------*/
/*
Inputs:  char board[MAXY+2][MAXX+2] 
Returns: 

print board

05/14/99 14:34:10
zamansky
*/
void print_board(char board[MAXY+2][MAXX+2])
{
int x,y;
for (y=1;y<=MAXY;y++)
  {
    for (x=1;x<=MAXX;x++)
      {
	printf("%c",board[y][x]);
	// same as cout << board[y][x]; 
      }
    printf("\n");
    // same as cout << endl;
  }
printf("\n");
}


/*-------------- void remove_digit_pals() --------------*/
/*
Inputs:  char board[MAXY+2][MAXX+2]
         int x
         int y
         char c 
Returns: 

replace board[y][x] with a space and do the same with all
its "digit pals"

05/14/99 14:48:14
zamansky
*/
void remove_digit_pals(char board[MAXY+2][MAXX+2],int x, int y,char c)
{
  if (x<1||x>MAXX||y<1||y>MAXY)
    return;

  if (board[y][x] != c)
    return;

  /* erase the digit pals */
  board[y][x] = ' ';

  remove_digit_pals(board,x+1,y,c);
  remove_digit_pals(board,x-1,y,c);
  remove_digit_pals(board,x,y+1,c);
  remove_digit_pals(board,x,y-1,c);

}

/*-------------- void copy_board() --------------*/
/*
Inputs:  char src[MAXY+2][MAXX+2] char dst[MAXY+2][MAXX+2] 
Returns: 


05/18/99 14:17:40
zamansky
*/
void copy_board(char src[MAXY+2][MAXX+2], char dst[MAXY+2][MAXX+2])
{
int x,y;

for (x=0;x<=MAXX+1;x++)
  {
    for (y=0;y<=MAXY+1;y++)
      {
	dst[y][x] = src[y][x];
      }
  }
}




/*-------------- void push_down() --------------*/
/*
Inputs:  char board[MAXY+2][MAXX+2] 
Returns: 

loop through all the rows on the board from bottom to top
  look at each column for a space (empty)
    if we have an empty, scan up and swap the space with the
    first non-empty

05/17/99 13:17:22
zamansky
*/
void push_down(char board[MAXY+2][MAXX+2])
{
int x,y,t;

/* loop through all the rows on the board from bottom to top */
for (y=MAXY;y>=1;y--)
  {
    /* look at each column for a space (empty) */
    for (x=1; x<=MAXX;x++)
      {
	/*if we have an empty, scan up and swap the space with the
	  first non-empty */
	if (board[y][x]==' ')
	  {

	    t=y-1;
	    while(t>=1 && board[t][x]==' ')
	      t--;

	    if (t>=1)
	      {
		board[y][x] = board[t][x];
		board[t][x] = ' ';
	      }
	  }
      }
  }
}


/*-------------- void push_over() --------------*/
/*
Inputs:  char board[MAXY+2][MAXX+2] 
Returns: 

***** YOU MUST CALL push_down FIRST FOR THIS TO WORK ********

loop through each column
  if the column is empty, look for the first non empty column
  and swap it with this empty column

05/17/99 13:36:30
zamansky
*/
void push_over(char board[MAXY+2][MAXX+2])
{
int x,y,z,t;

for (x=1;x<=MAXX;x++)
  {
    /* if the bottom element of a column is empty, so is the row */
    if (board[MAXY][x]==' ')
      {
	/* I ain't got no body */
	for (t=x+1 ; t<=MAXX && board[MAXY][t]==' ' ; t++)
	  ; /* NULL statement for loop */
	
	if (t<=MAXX)
	  {
	    for (z=1;z<=MAXY;z++)
	      {
		board[z][x] = board[z][t];
		board[z][t] = ' ';
	      }
	  }
      }
  }
}


/*-------------- int not_isolated() --------------*/
/*
Inputs:  char board[MAXY+2][MAXX+2]
         int x
         int y 
Returns: 

05/18/99 14:23:28
zamansky
*/
int not_isolated(char board[MAXY+2][MAXX+2], int x, int y)
{
char c;

if (board[y][x] == ' ')
  return 0;

c = board[y][x];
if (board[y+1][x] == c ||
    board[y-1][x] == c ||
    board[y][x-1] == c ||
    board[y][x+1] == c
    )
  return 1;

return 0;

}

int solved(char board[MAXY+2][MAXX+2])
{
  return board[MAXY][1]==' ';
}


/*-------------- void solve() --------------*/
/*
Inputs:  char board[MAXY+2][MAXX+2] 
Returns: 

solve a digit-pals board using blind search

05/18/99 14:19:41
zamansky
*/
void solve(char board[MAXY+2][MAXX+2], int *done)
{
int x,y;
char temp[MAXY+2][MAXX+2];

copy_board(board,temp);

if (solved(board))
  {
    printf("solved\n");
    *done=1;
  }
else
  {

    for (x=1;x<=MAXX;x++)
      {
	for (y=1;y<=MAXY;y++)
	  {
	    if (board[y][x] != ' ' && not_isolated(board,x,y) && !*done)
	      {
		remove_digit_pals(board,x,y,board[y][x]);
		push_down(board);
		push_over(board);
		solve(board,done);
		if (*done)
		  {
		    solx[lastsol]=x;
		    soly[lastsol]=y;
		    lastsol++;
		  }

		copy_board(temp,board);
	      }
	  }
      }
  }
}

int main()
{
char temp[MAXY+2][MAXX+2];
int *done;
int i;

/* done = new int */
done = (int *)malloc(sizeof(int));

*done=0;
print_board(board);
copy_board(board,temp);
  
solve(board,done);

for (i=lastsol-1;i>=0;i--)
  {
    remove_digit_pals(temp,solx[i],soly[i],temp[soly[i]][solx[i]]);
    push_down(temp);
    push_over(temp);
    print_board(temp);
  }


return 0;
}
