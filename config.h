#ifndef __CONFIG_H__
#define __CONFIG_H__

struct my_config {
  char *hostname;
  char *username;
  char *password;
  char *database;
  int timeout;
};

#ifndef IN_CONFIG_C
extern struct my_config config;
#endif


void	config_init  (void);
void	config_reset (void);
void	config_clean (void);

#endif
