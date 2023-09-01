#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "structs.h"
#include "functions.h"

char lmpsNOT2copy[14][9] = {"THINGS","LINEDEFS","SIDEDEFS","VERTEXES","SEGS","SSECTORS",
						"NODES","SECTORS","REJECT","BLOCKMAP","BEHAVIOR","SCRIPTS",0};

char maplist[69][6] = {"E1M1","E1M2","E1M3","E1M4","E1M5","E1M6","E1M7","E1M8","E1M9",//8
					 "E2M1","E2M2","E2M3","E2M4","E2M5","E2M6","E2M7","E2M8","E2M9",//17
					 "E3M1","E3M2","E3M3","E3M4","E3M5","E3M6","E3M7","E3M8","E3M9",//26
					 "E4M1","E4M2","E4M3","E4M4","E4M5","E4M6","E4M7","E4M8","E4M9",//35
					 "MAP01","MAP02","MAP03","MAP04","MAP05","MAP06","MAP07","MAP08","MAP09",//44
					 "MAP10","MAP11","MAP12","MAP13","MAP14","MAP15","MAP16","MAP17","MAP18","MAP19",//54
					 "MAP20","MAP21","MAP22","MAP23","MAP24","MAP25","MAP26","MAP27","MAP28","MAP29",//64
					 "MAP30","MAP31","MAP32",0};//68

/*
//void myfunc(FILE*, FILE*);
void myfunc(FILE **in_ptr, FILE **out_ptr)
{
	*in_ptr = fopen("c:\tab8.wad","rb");
	*out_ptr = fopen("c:\data\udmf8.wad","wb");

	cout << in_ptr << " " << out_ptr << "\n";
}
*/

int main(int argc, char *argv[])
{
	HEADER in_header, out_header;
	ENTRY L,*L2;
	MAPSCRIPT mapcode;
	FILE *fs_in = NULL, *fs_out = NULL;
	char wad_t[5];
	char lump_8[9];
	char buffer_8[9];
	char string_buffer[30];
	uint last_offset, temp;
	size_t read_err;
	int input_lump_index = 0, u = 1, maplist_index = 0, search_index = 0;
	int found = 0, found2 = 0;

	memset(wad_t,0,sizeof(wad_t));
	memset(lump_8,0,sizeof(lump_8));
	memset(buffer_8,0,sizeof(buffer_8));
	memset(string_buffer,0,sizeof(string_buffer));
	memset((void*)&L,0,sizeof(L));
	//memset((void*)&L2,0,sizeof(L2));
	memset((void*)&mapcode,0,sizeof(mapcode));
	
	//myfunc(&fs_in, &fs_out);
	//cout << fs_in << " " << fs_out;
	//exit(0);

	
	get_input_parms(argc, argv, &fs_in, &fs_out);
	printf("open input file and create output file\n");

	strncpy(out_header.wad_type, "PWAD", 4);
	out_header.num_lumps = 0;
	out_header.start_of_directory = 0;

	fwrite((char*)&out_header,1,sizeof(HEADER),fs_out);

	//strncpy(L.lump_name, "_1979", 5);
	//L.lump_offset = 32;
	//L.lump_size = 7;
	//fwrite((char*)&L,1,sizeof(L),fs_out);
	
	//printf("%d\n",ftell(fs_out));
	//fclose(fs_out);

	//***fs_in = fopen("C:/Documents and Settings/Chris Freund/Desktop/chex.wad","rb"); //vc-testrelease-0.3.3/tnt.wad
	//***if (fs_in==NULL) {fputs ("File error",stderr); exit (1);}

	
	fread(&in_header,sizeof(HEADER),1,fs_in);
	fseek(fs_in,in_header.start_of_directory,SEEK_SET);

	strncpy(wad_t, in_header.wad_type,4);
	
	printf("%d\n",in_header.num_lumps);

	L2 = (ENTRY*)malloc(sizeof(ENTRY) * in_header.num_lumps);
	memset((void*)L2,0,sizeof(L2));

	while(input_lump_index < in_header.num_lumps)
	{
		read_err = fread(&L,sizeof(ENTRY),1,fs_in);

		if(read_err <= NULL)
			break;

		

		//sizeof("Extended Module:")
		//temp = ftell(fs_in);  //save file pointer for temp section
		//fseek(fs_in,L.lump_offset,SEEK_SET);
		//fread(&string_buffer,17,1,fs_in);

		search_index = 0;
		found = 0;
		//printf("%d\n",found);
		while(search_index < 13)
		{
			if(strstr(lmpsNOT2copy[search_index++],L.lump_name) != NULL)
			{
				//printf("%s\n",strstr(lmpsNOT2copy[search_index],L.lump_name));
				search_index = 15;
				found = 1;
			}
		}

		maplist_index = 0;
		found2 = 0;
		while(maplist_index < 69)
		{
			if(strstr(maplist[maplist_index++], L.lump_name) != NULL)
			{
				maplist_index = 70;
				found2 = 1;
			}
		}

		//printf("%d%d%s\n",found,found2,L.lump_name);

		if(found == 0 && found2 == 0)
		{
			//printf("%s \n", L.lump_name);
			temp = ftell(fs_in);
			last_offset = ftell(fs_out);
			//printf("%d \n", temp - sizeof(ENTRY));
			write_lump(fs_in, fs_out, temp - sizeof(ENTRY));
			//printf("%d \n", ftell(fs_out));
			strcpy(L2[u].lump_name,L.lump_name);
			L2[u].lump_offset = last_offset;
			L2[u].lump_size = ftell(fs_out) - last_offset;
			u++;
			fseek(fs_in,temp,SEEK_SET);
		}
		
		//fseek(fs_in,temp,SEEK_SET);  //return file pointer from temp section
		//continue;  //fixed 11.7.2010


		if((L.lump_name[0] == 'E' && L.lump_name[1] >= 49 && L.lump_name[1] <= 57 && L.lump_name[2] == 'M') || (L.lump_name[0] == 'M' && L.lump_name[1] == 'A' && L.lump_name[2] == 'P' && !strstr(L.lump_name, "MAPINFO")))
		{
			maplist_index=0;
			while(maplist_index < 69)
			{
				if(strstr(L.lump_name, maplist[maplist_index]))
				{
					maplist_index = 70;
					last_offset = ftell(fs_out);
					printf("%s map found\n", L.lump_name);

					strcpy(L2[u].lump_name,L.lump_name);
					L2[u].lump_offset = last_offset;
					L2[u].lump_size = 0;	
					//printf("%s offset %d size %d\n",L2[u].lump_name,L2[u].lump_offset,L2[u].lump_size);
					u++;

					strcpy(L2[u].lump_name,"TEXTMAP");
					L2[u].lump_offset = last_offset;

					mapcode = write_udmfmap(fs_in, fs_out, input_lump_index);
					temp = ftell(fs_in);

					//printf("behavior %d script %d \n",mapcode.behavior_offset,mapcode.script_offset);
					//get behavior/script_ptr from write_udmf?

					L2[u].lump_size = ftell(fs_out) - last_offset;
					//printf("%s offset %d size %d\n",L2[u].lump_name,L2[u].lump_offset,L2[u].lump_size);
					u++;

					if(mapcode.behavior_offset > 0)
					{
						//printf("behavior offset\n");
						last_offset = ftell(fs_out);
						write_lump(fs_in, fs_out, mapcode.behavior_offset);
						strcpy(L2[u].lump_name,"BEHAVIOR");
						L2[u].lump_offset = last_offset;
						L2[u].lump_size = ftell(fs_out) - last_offset;
						u++;
					}

					if(mapcode.script_offset != NULL)
					{
						//printf("script offset\n");
						last_offset = ftell(fs_out);
						write_lump(fs_in, fs_out, mapcode.script_offset);
						strcpy(L2[u].lump_name,mapcode.script_name);
						L2[u].lump_offset = last_offset;
						L2[u].lump_size = ftell(fs_out) - last_offset;
						u++;
					}

					fseek(fs_in,temp,SEEK_SET);

					strcpy(L2[u].lump_name,"ENDMAP");
					L2[u].lump_offset = ftell(fs_out);
					L2[u].lump_size = 0;
					//printf("%s offset %d size %d\n",L2[u].lump_name,L2[u].lump_offset,L2[u].lump_size);
					u++;
				}//if statement to match map name to list end brace

				maplist_index++;
		
			}//while loop map name search end brace

		}//if statement for map lumps end brace

		//printf("%d %d %d %d %s\n", read_err, u, maplist_index, input_lump_index, L.lump_name);

		input_lump_index++;
	}//main while loop end brace

	u--; //correction for setting variable = 1
	//L2[0].lump_name = 
	
	L2[0].lump_size = u;

	out_header.num_lumps = u;
	out_header.start_of_directory = ftell(fs_out);
	
	printf("write lumps\n");
	write_lumpdirectory(fs_out, L2);

	printf("write header\n");
	rewind(fs_out);
	fwrite((char*)&out_header,1,sizeof(HEADER),fs_out);

	printf("close files\n");
	printf("%d\n", input_lump_index);
	fclose(fs_out);
	fclose(fs_in);
	//printf("%d\n", sizeof(L2));
	free(L2);
	return 0;
}//main function end brace