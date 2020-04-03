void kc8halt(void);
void kc8init(void);
void kc8power(int argc,char **argv);
void(*ttybreak)();
void mount_device(char *n,char *f);
void dump_devices(FILE *f);
void close_devices(void);
void register_device(int(*m)(),void(*d)(),int u,char *n,char *l,char *f);
void ttyoctal(int num,int digits,char *suffix);
