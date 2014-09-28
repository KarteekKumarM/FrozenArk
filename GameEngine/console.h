
#define CON_CHANNEL_ERROR		(1<<0)
#define CON_CHANNEL_WARNING		(1<<1)
#define CON_CHANNEL_RENDER		(1<<2)
#define CON_CHANNEL_SYSTEM		(1<<3)

void C_PrintLn(short channel, char *format, ...);
