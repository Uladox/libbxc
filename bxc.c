#include <ctype.h>
#include <ncurses.h>

typedef struct {
	int crsy;
	int crsx;
	int height;
	int width;
	int y;
	int x;
	WINDOW *pad;
	WINDOW *border;
} Polt_win;

int
center_y(int height)
{
	return (LINES - height) / 2;
}

int
center_x(int width)
{
	return (COLS - width) / 2;
}

int
right_x(int width)
{
	return (COLS - width);
}

int
left_x(void)
{
	return 0;
}

int
top_y(void)
{
	return 0;
}

int
bottom_y(int height)
{
	return (LINES - height);
}

void
pwin_renew(Polt_win *pwin)
{
	if (pwin->border) {
		wborder(pwin->border, ' ', ' ', ' ',' ',' ',' ',' ',' ');
		delwin(pwin->border);
	}

	pwin->border = newwin(pwin->height, pwin->width, pwin->y, pwin->x);

	/* box(pwin->border, 0, 0); */

	/* if (pwin->selected) */
		wborder(pwin->border, A_BOLD | ACS_VLINE, A_BOLD | ACS_VLINE,
			A_BOLD | ACS_HLINE, A_BOLD | ACS_HLINE,
			A_BOLD | ACS_ULCORNER, A_BOLD | ACS_URCORNER,
			A_BOLD | ACS_LLCORNER, A_BOLD | ACS_LRCORNER);
	/* else */
	/* 	wborder(pwin->border, A_DIM | ACS_VLINE, A_DIM | ACS_VLINE, */
	/* 		A_DIM | ACS_HLINE, A_DIM | ACS_HLINE, */
	/* 		A_DIM | ACS_ULCORNER, A_DIM | ACS_URCORNER, */
	/* 		A_DIM | ACS_LLCORNER, A_DIM | ACS_LRCORNER); */

	wrefresh(pwin->border);

	prefresh(pwin->pad, pwin->crsy, pwin->crsx, pwin->y + 1, pwin->x + 1,
		 pwin->y + pwin->height - 2, pwin->x + pwin->width - 2);
}

/* void */
/* pwin_renew(Polt_win *pwin) */
/* { */
/* 	pwin->win = newwin(pwin->height, pwin->width, pwin->y, pwin->x); */
/* 	/\* box(pwin->win, 0 , 0); *\/ */
/* 	wrefresh(pwin->win); */
/* } */

void
pwin_set(Polt_win *pwin, int height, int width, int starty, int startx)
{
	pwin->border = NULL;
	pwin->pad = newpad(height + 100, width);
	pwin->height = height;
	pwin->width = width;
	pwin->y = starty;
	pwin->x = startx;
	pwin_renew(pwin);
}

void
pwin_change_x(Polt_win *pwin, int amount)
{
	if ((pwin->x += amount) < 0)
		pwin->x = 0;
	else if (pwin->x > right_x(pwin->width))
		pwin->x = right_x(pwin->width);

	pwin_renew(pwin);
}

void
pwin_change_y(Polt_win *pwin, int amount)
{
	if ((pwin->y += amount) < 0)
		pwin->y = 0;
	else if (pwin->y > bottom_y(pwin->height))
		pwin->y = bottom_y(pwin->height);

	pwin_renew(pwin);
}

void
pwin_scroll_x(Polt_win *pwin, int amount)
{
	if ((pwin->crsx += amount) < 0)
		pwin->crsx = 0;

	pwin_renew(pwin);
}

void
pwin_scroll_y(Polt_win *pwin, int amount)
{
	if ((pwin->crsy += amount) < 0)
		pwin->crsy = 0;

	pwin_renew(pwin);
}

/* void pwin_remove(Polt_win *pwin) */
/* { */
/* 	wborder(pwin->win, ' ', ' ', ' ',' ',' ',' ',' ',' '); */
/* 	wrefresh(pwin->win); */
/* 	delwin(pwin->win); */
/* 	pwin->win = NULL; */
/* } */

void
init(void)
{
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE);
}

void
pwin_move(Polt_win *pwin, int ch)
{
	erase();
	refresh();

	if (isascii(ch)) {
		switch (ch) {
		case 'p':
			pwin_scroll_y(pwin, -1);
			return;
		case 'n':
			pwin_scroll_y(pwin, 1);
			return;
		}
	}

	switch(ch) {
	case KEY_LEFT:
		pwin_change_x(pwin, -1);
		break;
	case KEY_RIGHT:
		pwin_change_x(pwin, 1);
		break;
	case KEY_UP:
		pwin_change_y(pwin, -1);
		break;
	case KEY_DOWN:
		pwin_change_y(pwin, 1);
		break;
	}
}

int main(int argc, char *argv[])
{	Polt_win pwin;
	int startx, starty, width, height;
	int ch;

	init();

	height = 10;
	width = 15;
	starty = top_y();
	startx = right_x(width);
	printw("Press F1 to exit");
	refresh();
        pwin_set(&pwin, height, width, starty, startx);

	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "hello world!\n");
	wprintw(pwin.pad, "again\n");
	wprintw(pwin.pad, "cats\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "woot!\n");
	wprintw(pwin.pad, "word!\n");

	pwin_renew(&pwin);
	refresh();

	while((ch = getch()) != KEY_F(1))
		pwin_move(&pwin, ch);

	endwin();

	return 0;
}
