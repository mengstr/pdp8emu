extern void(*ttybreak)();

int ttygetc(void);
void ttygets(char *buf,int len);
int ttypoll(void);
void comms_init(void);
void comms_cleanup(void);