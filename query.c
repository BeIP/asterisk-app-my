#include <asterisk.h>
#include <asterisk/linkedlists.h>
#include <asterisk/app.h>
#include <ctype.h>
#include <stdio.h>
#include <asterisk/file.h>
#include <asterisk/logger.h>
#include <asterisk/module.h>
#include <asterisk/options.h>
#include <asterisk/pbx.h>

#include "database.h"

/*****************************************************************************
 * SELECT application.                                                       *
 *****************************************************************************/

static void
prepare_var_name (const char *field_name, char *var_name, unsigned length)
{
  unsigned i = 0;
  const char *f = field_name;
  char *v = var_name;

  while (++i < length && *f != '\0')
    {
      if (!isascii (*f) || !isalnum (*f))
        *v = '_';
      else if (islower (*f))
        *v = toupper (*f);
      else
        *v = *f;
      ++f;
      ++v;
    }
  *v = '\0';
}

static void
set_vars (struct ast_channel *chan,
          MYSQL_RES *result)
{
  unsigned int num_fields;
  MYSQL_FIELD *fields;
  MYSQL_ROW row;
  unsigned i;
  char var_name[20];

  num_fields = mysql_num_fields (result);
  fields = mysql_fetch_fields (result);
  row = mysql_fetch_row (result);

  for (i = 0; i < num_fields; i++)
    {
      prepare_var_name (fields[i].name, var_name, sizeof (var_name));
      pbx_builtin_setvar_helper (chan, var_name, row[i] ? row[i] : "");

      if (option_verbose > 2)
        ast_verbose (VERBOSE_PREFIX_2 "set_vars: %s = %s\n", var_name, row[i] ? row[i] : "");
    }
}

static int
app_my_select_exec (struct ast_channel *chan,
                    void               *data)
{
  int res = 0;
  MYSQL *mysql;
  MYSQL_RES *result;
  unsigned num_rows;
  
  mysql = database_get ();
  ensure_mysql_connection (mysql);

  res = mysql_query (mysql, data);
  if (res != 0)
    return res;

  result = mysql_use_result (mysql);
  num_rows = mysql_num_rows (result);

  /* Fetch the result and set the variables. */
  if (num_rows >= 1)
    set_vars (chan, result);

  /* Discard the eventual remaining rows */
  if (num_rows > 1)
    while (mysql_fetch_row (result));
  mysql_free_result (result);

  database_release (mysql);

  return res;
}


/*****************************************************************************
 * Initialization functions                                                  *
 *****************************************************************************/

void
query_init (void)
{
  ast_register_application ("MySelect",
                            app_my_select_exec,
                            "Select some values in the SQL database",
                            "MySelect(query)\n");
}

void
query_clean (void)
{
  ast_unregister_application ("MySelect");
}

/* ex:set ts=2 et sw=2 ai: */
