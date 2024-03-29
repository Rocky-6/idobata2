#ifndef _IDOBATA_LIST_H_
#define _IDOBATA_LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define L_USERNAME 15

typedef struct _imember {
  char username[L_USERNAME];
  int sock;
  struct _imember *next;
} *imember;

typedef struct {
  imember top;
} list;

imember make_cell(char *username, int sock, imember member);
list *make_list(void);
void delete_cell(imember member);
void delete_list(list *ls);
imember nth_cell(imember member, int n);
int nth(list *ls, int n, bool *err);
bool insert_nth(list *ls, int n, char *name, int sock);
bool create_member(list *ls, char *name, int sock);
void print_list(list *ls);

#endif
