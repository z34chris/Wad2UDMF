#include <stdio.h>
#include <string.h>

#define uchar unsigned char
#define ushort unsigned short
#define uint unsigned int
#define ulong unsigned long

//#include <iostream>
//using namespace std;

struct SECTOR
{
	short floor;
	short ceil;
	char floor_tex[8];
	char ceil_tex[8];
	short light;
	short type;
	short tag;
};

struct xSECTOR
{
	short floor;
	short ceil;
	char floor_tex[9];
	char ceil_tex[9];
	short light;
	short type;
	short tag;
};

struct VERTEX
{
	short x;
	short y;
};

struct LINEDEF
{
	short v1;
	short v2;
	short flags;
	short special;
	short tag;
	short right_side;
	short left_side;
};

struct zLINEDEF
{
	short v1;
	short v2;
	short flags;
	char special;
	char arg1;
	char arg2;
	char arg3;
	char arg4;
	char arg5;
	short right_side;
	short left_side;
};

struct SIDEDEF
{
	short x_offset;
	short y_offset;
	char upper_tex[8];
	char lower_tex[8];
	char mid_tex[8];
	short sector;
};

struct THING
{
	short x_pos;
	short y_pos;
	short angle;
	short type;
	short flags;
};

struct zTHING
{
	short id;
	short x_pos;
	short y_pos;
	short height;
	short angle;
	short type;
	short flags;
	char special;
	char arg1;
	char arg2;
	char arg3;
	char arg4;
	char arg5;
};

struct HEADER
{
	char wad_type[4];
	int num_lumps;
	int start_of_directory;
};

struct ENTRY
{
	uint lump_offset;
	uint lump_size;
	char lump_name[8];
};

struct MAP
{
	ENTRY name;
	ENTRY things;
	ENTRY linedefs;
	ENTRY sidedefs;
	ENTRY vertexes;
	ENTRY sectors;
};

void write_sectors(void);
void write_sidedefs(void);
void write_linedefs(void);
void write_vertexes(uint offset, uint size);
void write_things(void);

char *maplist[6][11] = {"MAP01", "E1M1", "E1M2", "E1M3", "E1M4", "E1M5", "E1M6", "E1M7", "E1M8", "E1M9", 0};

void main(void)
{
	HEADER h1;
	ENTRY L;
	//MAP M;
	LINEDEF LDF;
	SIDEDEF SDF;
	VERTEX V;
	SECTOR S;
	THING T;
	//xSECTOR s2;
	FILE *fs;
	char wad_t[5];
	char lump_8[9];
	char buffer_8[9];
	uint last_offset;
	int i = 0,a,b;
	char blank[] = {0x45,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

	memset(wad_t,0,sizeof(wad_t));
	memset(lump_8,0,sizeof(lump_8));
	memset(buffer_8,0,sizeof(buffer_8));

	fs = fopen("C:/Documents and Settings/Chris Freund/Desktop/tab8.wad", "rb");
	fread(&h1,sizeof(HEADER),1,fs);
	fseek(fs,h1.start_of_directory,SEEK_SET);

	strncpy(wad_t, h1.wad_type,4);

	//printf("wadtype = %s number of lumps = %d directory located at %d\n", wad_t, h1.num_lumps, h1.start_of_directory);
	printf("namespace = \"ZDoomTranslated\";\n\n");
	
	while(i < h1.num_lumps)
	{
		fread(&L, sizeof(ENTRY), 1, fs);

		if(strstr(L.lump_name, "MAP01"))
		{
		}

		if(strstr(L.lump_name, "SECTORS") && !strstr(L.lump_name, "SSECTORS"))
		{
			b=0;
			a = ftell(fs);
			last_offset = a;
			fseek(fs,L.lump_offset,SEEK_SET);

			while(b<1)//(L.lump_size / sizeof(S)))
			{
				fread(&S, sizeof(SECTOR),1,fs);
				printf("sector //sector %d \n{\n",b);
				printf("heightfloor = %d;\nheightceiling = %d;\n",S.floor,S.ceil);
				
				if(strlen(S.floor_tex) > 8)
				{
					strncpy(buffer_8, S.floor_tex, 8);
					printf("texturefloor = \"%s\";\n",buffer_8);
				}
				else
				printf("texturefloor = \"%s\";\n",S.floor_tex);
				
				if(strlen(S.ceil_tex) > 8)
				{
					strncpy(buffer_8, S.ceil_tex, 8);
					printf("textureceiling = \"%s\";\n",buffer_8);
				}
				else
				printf("textureceiling = \"%s\";\n",S.ceil_tex);

				printf("lightlevel = %d;\nid = %d;\n",S.light,S.tag);
				printf("}\n\n");
				b++;
			}

			fseek(fs,last_offset,SEEK_SET);
		}

		if(strstr(L.lump_name, "SIDEDEFS"))
		{
			b=0;
			a = ftell(fs);
			last_offset = a;
			fseek(fs,L.lump_offset,SEEK_SET);
			
			while(b<1) //(L.lump_size / sizeof(SDF)))
			{
				fread(&SDF, sizeof(SIDEDEF),1,fs);
				printf("sidedef //sidedef %d \n{\n",b);
				if(strlen(SDF.upper_tex) > 1)
				{
					if(strlen(SDF.upper_tex) > 8)
					{
						strncpy(buffer_8, SDF.upper_tex, 8);
						printf("texturetop = \"%s\";\n",buffer_8);
					}
					else
					printf("texturetop = \"%s\";\n",SDF.upper_tex);
				}

				if(strlen(SDF.mid_tex) > 1)
				{
					if(strlen(SDF.mid_tex) > 8)
					{
						strncpy(buffer_8, SDF.mid_tex, 8);
						printf("texturemiddle = \"%s\";\n",buffer_8);
					}
					else
					printf("texturemiddle = \"%s\";\n",SDF.mid_tex);
				}

				if(strlen(SDF.lower_tex) > 1)
				{
					if(strlen(SDF.lower_tex) > 8)
					{
						strncpy(buffer_8, SDF.lower_tex, 8);
						printf("texturebottom = \"%s\";\n",buffer_8);
					}
					else
					printf("texturebottom = \"%s\";\n",SDF.lower_tex);
				}

				if(SDF.x_offset){printf("offsetx = %d;\n",SDF.x_offset);}
				if(SDF.y_offset){printf("offsety = %d;\n",SDF.y_offset);}
				printf("sector = %d;\n}\n\n",SDF.sector);
				b++;
			}

			fseek(fs,last_offset,SEEK_SET);
		}

		if(strstr(L.lump_name, "LINEDEFS"))
		{
			b=0;
			a = ftell(fs);
			last_offset = a;
			fseek(fs,L.lump_offset,SEEK_SET);

			while(b<20) //(L.lump_size / sizeof(LDF)))
			{
				fread(&LDF, sizeof(LINEDEF),1,fs);
				
				printf("linedef //linedef %d \n{\n",b);
				if(LDF.tag > 0)
				{
					printf("id = %d;\n",LDF.tag);
				}
				else
					printf("id = 0;\n");
				if(LDF.flags > 0)
				{
					if(LDF.flags & 0x0001)
						printf("blocking = true;\n");
					if(LDF.flags & 0x0002)
						printf("blockmonsters = true;\n");
					if(LDF.flags & 0x0004)
						printf("twosided = true;\n");
					if(LDF.flags & 0x0008)
						printf("dontpegtop = true;\n");
					if(LDF.flags & 0x0010)
						printf("dontpegbottom = true;\n");
					if(LDF.flags & 0x0020)
						printf("secret = true;\n");
					if(LDF.flags & 0x0040)
						printf("blocksound = true;\n");
					if(LDF.flags & 0x0080)
						printf("dontdraw = true;\n");
					if(LDF.flags & 0x0100)
						printf("mapped = true;\n");
				}

				if(LDF.right_side > -1)
					printf("sidefront = %d;\n",LDF.right_side);
				if(LDF.left_side > -1)
					printf("sideback = %d;\n",LDF.left_side);
				printf("v1 = %d;\nv2 = %d;\n",LDF.v1,LDF.v2);
				printf("}\n\n");

				b++;
			}

			fseek(fs,last_offset,SEEK_SET);
		}

		if(strstr(L.lump_name, "VERTEXES"))
		{
			b=0;
			a = ftell(fs);
			last_offset = a;
			fseek(fs,L.lump_offset,SEEK_SET);
			printf("\n%d\n",sizeof(V));
			while(b<1) //(L.lump_size / sizeof(V)))
			{
				fread(&V, sizeof(VERTEX),1,fs);
				printf("vertex //vertex %d \n{\nx = %d.0;\ny = %d.0;\n}\n\n",b,V.x, V.y);
				b++;
			}

			fseek(fs,last_offset,SEEK_SET);
		}

		if(strstr(L.lump_name, "THINGS"))
		{
			b=0;
			a = ftell(fs);
			last_offset = a;
			fseek(fs,L.lump_offset,SEEK_SET);

			while(b<1) //(L.lump_size / sizeof(T)))
			{
				fread(&T, sizeof(THING),1,fs);
				printf("thing //thing %d \n{\n", b);
				printf("x = %d;\ny = %d;\nangle = %d;\ntype = %d;\n",T.x_pos,T.y_pos,T.angle,T.type);
				if(T.flags & 0x0001)
					printf("skill1 = true;\nskill2 = true;\n");
				if(T.flags & 0x0002)
					printf("skill3 = true;\n");
				if(T.flags & 0x0004)
					printf("skill4 = true;\nskill5 = true;\n");
				if(T.flags & 0x0008)
					printf("single = true;\n");
				if(T.flags & 0x0010)
					printf("ambush = true;\n");
				if(T.flags & 0x0020)
					printf("dm = true;\n");
				if(T.flags & 0x0040)
					printf("coop = true;\n");
				printf("}\n\n");
				b++;
			}

			fseek(fs,last_offset,SEEK_SET);
		}
		i++;
		//if(strlen(L.lump_name) > 8)
		//{
			//strncpy(lump_8, L.lump_name, 8);
			//printf("lump %s lump size = %d lump located at %d\n", lump_8, L.lump_size, L.lump_offset);
		//}
		//else
		//printf("lump %s lump size = %d lump located at %d\n", L.lump_name, L.lump_size, L.lump_offset);
		
		//(i < h1.num_lumps && h1.num_lumps < 50)||(i < 50 && h1.num_lumps > 50)
		
	}
	

	fclose(fs);
}

void write_sectors(void){}
void write_sidedefs(void){}
void write_linedefs(void){}

void write_vertexes(uint offset, uint size)
{
}

void write_things(void){}