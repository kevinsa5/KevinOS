#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

void sh_readKFS(char*);
void sh_textDump(char*);
void sh_memDump(char*);
void sh_help(char*);
void sh_null(char*);
void sh_cat(char*);
void sh_head(char*);
void sh_ls(char*);
void sh_edit(char*);
void sh_read(char*);
void sh_glissando(char*);
void sh_mario(char*);
void sh_sleep(char*);
void sh_beep(char*);
void sh_htoi(char*);
void sh_itoh(char*);
void sh_int(char*);
void sh_rand(char*);
void sh_colorTable(char*);
void sh_charTable(char*);
void sh_hexTable(char*);
void sh_dataTypes(char*);
void sh_sti(char*);
void sh_cli(char*);
void sh_buildData(char*);
void sh_memalloc(char*);
void sh_memfree(char*);
void sh_poke(char*);
void sh_history(char*);
void sh_memusg(char*);
void sh_millis(char*);
void sh_foo(char*);

void clearBuffer();
int getFilesPointer(char*);

#endif
