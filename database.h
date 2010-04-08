#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <mysql/mysql.h>

MYSQL   *database_get                 (void);
void     database_release             (MYSQL *mysql);

void     ast_log_mysql_error          (const char *func_name);
int      ensure_mysql_connection      (MYSQL *mysql);

void     database_init                (void);
void     database_reset               (void);
void     database_clean               (void);

#endif

/* ex:set ts=2 et sw=2 ai: */
