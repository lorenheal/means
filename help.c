#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>
#include <execinfo.h>
#include <ctype.h>
#include <stdarg.h>
#include "help.h"

#define OPT_HAS_ARG (true)
#define OPT_NO_ARG (false)

unsigned int error_count = 0;

/*
* OptString is the list of getopt(3) options.
* Used in help(), getopthelp(), and main().
*/

void help(char *progname, char *optstring, char *helptext)
	{
	printf("Usage:\n%s ", progname);
	getopthelp(optstring);
	printf("%s\n", helptext);
	}

void nhelp(char *progname, const Option *opttable)
	{
	char * optstring;
	printf("Usage:\n%s ", progname);
	optstring = buildOptString(opttable);
	getopthelp(optstring);
	for(Option *o = (Option *) opttable; o && o->flag; o++)
		{
		if(*(o->arg))
			printf(" -%s [%s] %s\n", o->flag, o->arg, o->func);
		else
			printf(" -%s %s\n", o->flag, o->func);
		}
	debug_usage();
	}

/*
* getopthelp - decode and display getopt(3) options
* -- optstring is a string of getopt(3) options
* --
*/

void
getopthelp(char *optstring)
	{
	int i;
	int grouping = false;
	char *s, c;
	s = strdup(optstring);
	if(DEBUG(DBG_GETOPT)) printf("optstring: %s\n",s);
	for(i = 0; (c = s[i]) != 0; i++)
		{
		if(isalpha(c))
			{
			if(s[i+1] == ':')
				{
				if(grouping)
					{
					printf("] ");
					grouping = false;
					}
				printf("-%c arg ", c);
				}
			else
				{
				if(!grouping)
					{
					printf("-[");
					grouping = true;
					}
				printf("%c", c);
				}
			}
		else
			{
			if(grouping) printf("] ");
			grouping = false;
			}
		}
	if(grouping)
		{
		printf("] ");
		grouping = false;
		}
	printf("[args ...]\n");
	free(s);
	}


/************************************************************
 * getopttablesize()
 * 	Takes an Option *table
 * 	Returns the number  of entries
 *
 ************************************************************/

int
getopttablesize(const Option *table)
	{
	int ret = 0;
	if(table)
		{
		while(table[ret].flag != NULL)
			ret++;
		}
	return(ret);
	}

char *debug_optstring(void)
	{
	char *s = malloc(DBG_NBITS + 1);
	if(s) memset(s, 0, DBG_NBITS+1);
	int i = 0;
	for (const struct dbgopt *d = dbgopts; d->letter; d++)
		s[i++] = d->letter;
	s[i] = '\0';
	return s;
	}

/*
* buildOptstring - turn a help table into a string for getopt(3)
*
*/
char *
buildOptString (const Option *opttable)
	{
	int i = 0, options = 0;
	char *s;
	char *ret = NULL;
	if(opttable == NULL)
		{
		printf("Null opttable");
		return(ret);
		}
	options = getopttablesize(opttable);
	ret = malloc((2*options)+1);
	if(ret)
		memset(ret, 0, (2*options)+1);
	s = ret;
	for(i=0; opttable[i].flag != NULL ; s++,i++)
		{
		*s=*(opttable[i].flag);
		if(strlen(opttable[i].arg)>0)
			*(++s) = ':';
		}
	*s='\0';
	return (ret);
	}

unsigned setdebug(const char *options)
	{
	unsigned ret = debug_mask;
	if (!options) return ret;
	for (const char *p = options; *p; p++)
		{
		const struct dbgopt *d = dbgopts;
		for (; d->letter; d++)
			if (d->letter == *p)
				{
				ret |= d->bit;
				break;
				}
		if (!d->letter)            /* unknown letter */
			{
			diagnose(DIAG_WARNING,DBG_GETOPT,0,0, "Unknown debug area '%c'\n", *p);
			debug_usage();
			}
		}
	return ret;
	}

void debug_usage(void)
	{
	fputs("debug areas:", stderr);
	for (const struct dbgopt *d = dbgopts; d->letter; d++)
		fprintf(stderr, " %c=%s", d->letter, d->help);
	fputc('\n', stderr);
	}

/*
* diagnose(severity, area, line, col, fmt, \ldots)
* - errors always print and increment a count; debug prints only if the area bit is set.
* - yyerror and the runtime “undefined” paths call it.
* - main returns non-zero if the count is non-zero. One exit point.
* - Optional: print the offending line with a caret.
* - Column can wait if you do not have it yet.

void diagnose(int severity, int area, int line, int col, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stdout, fmt, ap);   / * or stderr, but pick one * /
    va_end(ap);
    fflush(stdout);
}
setvbuf(stdout, NULL, _IONBF, 0);
setvbuf(stderr, NULL, _IONBF, 0);

*/

/* enum severity { DIAG_FATAL, DIAG_DEBUG, DIAG_WARNING, DIAG_ERROR };*/
void diagnose(enum severity sev, unsigned area, int line, int col,
              const char *fmt, ...)
	{
	(void)col;                       /* unused for now */
	FILE *out = stderr;              /* errors and warnings belong on stderr */
	/**/
	int should_print = 0;
	int should_count = 0;
	int should_exit  = 0;
	int exit_code    = 0;
	/**/
	switch (sev)
		{
		case DIAG_DEBUG:
			should_print = DEBUG(area);
			break;
		case DIAG_WARNING:
			should_print = 1;
			break;
		case DIAG_ERROR:
			should_print = 1;
			should_count = 1;
			break;
		case DIAG_FATAL:
			should_print = 1;
			should_count = 1;
			should_exit  = 1;
			exit_code    = (area != DBG_OK) ? (int)area : 1;
			break;
		default:
			break;
		}
	/**/
	if (should_print && fmt)
		{
		va_list ap;
		va_start(ap, fmt);
		if (line > 0)
			fprintf(out, "line %d: ", line);
		vfprintf(out, fmt, ap);
		fputc('\n', out);
		va_end(ap);
		}
	/**/
	if (should_count)
		error_count++;
	/**/
	if (should_exit)
		exit(exit_code);
	}
