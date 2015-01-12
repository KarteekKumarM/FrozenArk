
#define CON_CHANNEL_ALL			(1<<0)
#define CON_CHANNEL_ERROR		(1<<1)
#define CON_CHANNEL_WARNING		(1<<2)
#define CON_CHANNEL_RENDER		(1<<3)
#define CON_CHANNEL_SYSTEM		(1<<4)
#define CON_CHANNEL_ASSET_DB	(1<<5)
#define CON_CHANNEL_COUNT 6

void Con_Init();
void Con_Shutdown();
void Con_PrintLn( short channel, char *format, ... );
