#define _DOOM 0
#define _ZDOOM 1
#define _HEXEN 2


MAPSCRIPT write_udmfmap(FILE* input, FILE* output, int&);
void write_lump(FILE* input, FILE* output, long);
//void write_lumpdirectory(FILE* output, struct ENTRY*);
void write_lumpdirectory(FILE* output, struct ENTRY*);
void get_input_parms(int, char*[], FILE** input, FILE** output);
char* stolower(char*);

