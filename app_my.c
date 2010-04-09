/* app_my: damn simple MySQL application for asterisk
 * Copyright (C) 2010 - Steve Frécinaux
 * Licensed under the GPL2+
 */

#include <asterisk.h>
#include <asterisk/cli.h>
#include <asterisk/module.h>

#include "config.h"
#include "database.h"
#include "query.h"

static int load_module   (void);
static int reload_module (void);
static int unload_module (void);

/*****************************************************************************
 * CLI                                                                       *
 *****************************************************************************/

static char *
handle_cli_my_reload (struct ast_cli_entry *e,
                      int cmd,
                      struct ast_cli_args *a)
{
  switch (cmd)
    {
      case CLI_INIT:
        e->command = "my reload";
        e->usage = "Reload the mysql application configuration";
        return NULL;

      case CLI_GENERATE:
        return NULL;

      default:
        reload_module ();
        return CLI_SUCCESS;
    }
}

static struct ast_cli_entry cli_entry = AST_CLI_DEFINE (handle_cli_my_reload, "Reload the mysql application");

/*****************************************************************************
 * Module                                                                    *
 *****************************************************************************/

ASTERISK_FILE_VERSION (__FILE__, VERSION)

static int
load_module (void)
{
  config_init ();
  database_init ();
  query_init (ast_module_info);

  ast_cli_register (&cli_entry);

  return 0;
}

static int
reload_module (void)
{
  config_reset ();
  database_reset ();

  return 0;
}

static int
unload_module (void)
{
  query_clean ();
  database_clean ();
  config_clean ();

  ast_cli_unregister (&cli_entry);

  return 0;
}

AST_MODULE_INFO(ASTERISK_GPL_KEY,
                AST_MODFLAG_DEFAULT,
                "BeIP MySQL Application",
                .load = load_module,
                .unload = unload_module,
                .reload = reload_module);

/* ex:set ts=2 et sw=2 ai: */
