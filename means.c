#include "means.h"
#include "symtab.h"
#include "ast.h"
#include "exec.h"
#include "params.h"
#include "builtins.h"
#include "means.tab.h"
#include "help.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

extern FILE *yyin;
extern int yylineno;
void yyerror(const char *s);
extern int yylex(void);
int yylex_destroy(void);

extern struct stmt *cur_head;

unsigned int debug_mask = 0;

extern struct scope *defining_scope;

static const Option OptTable[] =
	{
		{ "W", "",             "suppress multi-return warning" },
		{ "f", "filename",     "Input file" },
		{ "d", "xxx",          "debug mask" },
		{ "e", "[target]",     "emit: i interpret, c C" },
		{ "h", "",             "Show help" },
		{ "o", "outfile",      "Output file" },
		{ "c", "",             "Compile only (do not link)" },
		{ "S", "",             "Emit assembly" },
		{ "E", "",             "Dump AST" },
		{ "g", "",             "Generate debug info" },
		{ "O", "level",        "Optimisation level" },
		{ "I", "dir",          "Add include path" },
		{ "L", "dir",          "Add library path" },
		{ "l", "lib",          "Link with library" },
		{ "D", "name[=value]", "Define constant" },
		{ "v", "",             "Verbose" },
		{ "target", "triple",  "Target triple" },
		{ NULL, NULL, NULL }
	};

int main(int argc, char *argv[])
	{
	char *optstring = buildOptString(OptTable);
	char *outfile = NULL;
	int opt;

	diagnose(DIAG_DEBUG, DBG_EXEC, 0, 0,
	         "ST_CALL=%d ST_RETURN=%d ST_BLOCK=%d\n",
	         ST_CALL, ST_RETURN, ST_BLOCK);
	diagnose(DIAG_DEBUG, DBG_GETOPT, 0, 0, "optstring: %s\n", optstring);

	while ((opt = getopt(argc, argv, optstring)) != -1)
		{
		switch (opt)
			{
			case 'W':
				/* warn_multi_return = 0; -- later */
				break;
			case 'f':
				yyin = fopen(optarg, "r");
				if (!yyin)
					{
					diagnose(DIAG_ERROR, DBG_STDIO, 0, 0,
					         "cannot open %s", optarg);
					return 1;
					}
				break;
			case 'd':
				debug_mask = setdebug(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				nhelp(argv[0], OptTable);
				return 0;
			default:
				nhelp(argv[0], OptTable);
				return 1;
			}
		}

	if (outfile)
		{
		if (freopen(outfile, "w", stdout) == NULL)
			{
			diagnose(DIAG_ERROR, DBG_STDIO, 0, 0,
			         "cannot open output %s", outfile);
			return 1;
			}
		}

	if (argc > optind && !yyin)
		{
		yyin = fopen(argv[optind], "r");
		if (!yyin)
			{
			diagnose(DIAG_ERROR, DBG_STDIO, 0, 0,
			         "cannot open %s", argv[optind]);
			return 1;
			}
		}
	if (!yyin)
		yyin = stdin;

	global_scope = scope_new("global", NULL);
	defining_scope = global_scope;
	register_builtins(global_scope);

	int rc = yyparse();
	if (rc || error_count)
		return 1;
	struct stmt *top = cur_head;   /* or whatever holds the top-level list */
	exec_stmts(cur_head, global_scope);

	/* free_stmts(cur_head); -- when the free suite exists */

	if (yyin != stdin)
		fclose(yyin);
	free_stmts(top);               /* then release it */
	scope_free(global_scope);
	yylex_destroy();
	if(optstring) free(optstring);
	return error_count ? 1 : 0;
	}
