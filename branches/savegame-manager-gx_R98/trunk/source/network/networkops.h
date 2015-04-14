#ifndef _NETWORKOPS_H_
#define _NETWORKOPS_H_

int DownloadFileToMem(const char *url, u8 **inbuffer, u32 *size, bool resetstate = true);
int DownloadFileToPath(const char *url, const char *dest, bool resetstate = true);
void Initialize_Network(void);
void DeInit_Network(void);
bool IsNetworkInit(void);
bool IsNetworkInitiating(void);
char * GetNetworkIP(void);
void HaltNetworkThread();
void ResumeNetworkThread();
void InitNetworkThread();
void ShutdownNetworkThread();

#endif
