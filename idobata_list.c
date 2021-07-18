#include "idobata_list.h"

imember make_cell(char *username, int sock, imember member) {
  imember new_member = (imember)malloc(sizeof(struct _imember));
  if(new_member != NULL) {
    strcpy(new_member->username, username);
    new_member->sock = sock;
    new_member->next = member;
  }
  return new_member;
}

list *make_list(void) {
  char dummy[] = "xxx";
  list *ls = (list*)malloc(sizeof(list));
  if(ls != NULL) {
    ls->top = make_cell(dummy, 0, NULL);
    if(ls->top == NULL) {
      free(ls);
      return NULL;
    }
  }
  return ls;
}

void delete_cell(imember member)
{
  while (member != NULL) {
    imember temp = member->next;
    free(member);
    member = temp;
  }
}

// リストの削除
void delete_list(list *ls)
{
  delete_cell(ls->top);
  free(ls);
}

imember nth_cell(imember member, int n)
{
  for (int i = -1; member != NULL; i++, member = member->next)
    if (i == n) break;
  return member;
}

int nth(list *ls, int n, bool *err)
{
  imember member = nth_cell(ls->top, n);
  if (member == NULL) {
    *err = false;
    return 0;
  }
  *err = true;
  return member->sock;
}

bool insert_nth(list *ls, int n, char *name, int sock)
{
  imember member = nth_cell(ls->top, n - 1);
  if (member == NULL) return false;
  member->next = make_cell(name, sock, member->next);
  return true;
}

bool delete_nth(list *ls, int n) {
  imember member = nth_cell(ls->top, n-1);
  if(member == NULL || member->next == NULL) return false;
  imember temp = member->next;
  member->next = member->next->next;
  free(temp);
  return true;
}

// 先頭に追加
bool push(list *ls, char *name, int sock)
{
  return insert_nth(ls, 0, name, sock);
}

void print_list(list *ls)
{
  for(imember member = ls->top->next; member != NULL; member = member->next)
    printf("%s %d\n", member->username, member->sock);
}
