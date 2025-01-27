#include <argp.h>
#include <ft_ls.h>
#include "docs.h"

struct argp_option options[] =
{
	{
		.name	= 0,
		.key	= 'l',
		.arg	= 0,
		.flags	= 0,
		.doc	= "use a long listing format", 
		0
	},
	{
		.name	= "recursive",
		.key	= 'R',
		.arg	= 0,
		.flags	= 0,
		.doc	= "list subdirectories recursively", 
		0
	},
	{
		.name	= "all", 
		.key	= 'a',
		.arg	= 0,
		.flags	= 0,
		.doc	= "do not ignore entries starting with .",
		0
	},
	{
		.name	= "reverse",
		.key	= 'r',
		.arg	= 0,
		.flags	= 0,
		.doc	= "reverse order while sorting",
		0
	},
	{
		.name	= 0,
		.key	='t',
		.arg	= 0,
		.flags	= 0,
		.doc	= "sort by time, newest first",
		0
	},
	{
		.name	= "directory",
		.key	= 'd',
		.arg	= 0,
		.flags	= 0,
		.doc	= "list directories themselves, not their contents", 0},
	{ 0 }
};

static int parse_opt (int key, char *arg, struct argp_state *state)
{
	(void) state;
	(void) arg;
	switch (key)
	{
		case 'd': printf (".\n"); break;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int	ret;
	const struct argp argp = {
		.options	= options,
		.parser		= parse_opt,
		.args_doc	= LS_DOC,
		.doc		= DESC,
		0
	};

	ret = argp_parse(&argp, argc, argv, 0, 0, 0);
	return (ret);
}
