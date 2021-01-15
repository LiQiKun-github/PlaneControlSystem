#ifndef __CLI_LINK_H__
#define __CLI_LINK_H__




void print_link();
struct cli_t *cli_find(int cfd);
void delete_cli_node(struct cli_t *last,struct cli_t *p);
struct cli_t *last_node(struct cli_t *temp);
void cli_close();
void add_cli_node(int nfd,struct sockaddr_in caddr);

#endif
