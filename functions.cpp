#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"
#include "defs.h"
#include "structs.h"
#include "functions.h"



void get_input_parms(int arg_count, char *arg_verb[], FILE **in_ptr, FILE **out_ptr)
{
	//int i = 1;
	//char a[] = ".WAD";
	if(arg_count > 1)
	{
		for (int i=1; i<arg_count; i++)
		{
			if((strstr(arg_verb[i], "-input") && strlen(arg_verb[i]) == strlen("-input")) || (strstr(arg_verb[i], "-i") && strlen(arg_verb[i]) == strlen("-i")))
			{
				if((i+1) <= arg_count && arg_verb[i+1])
				{
					*in_ptr = fopen(arg_verb[i+1],"rb");

					if(*in_ptr == NULL)
					{
						cout << "Invalid input file, choose another\n";
						exit(1);
					}
				}
			}
			
			if((strstr(arg_verb[i], "-output") && strlen(arg_verb[i]) == strlen("-output")) || (strstr(arg_verb[i], "-o") && strlen(arg_verb[i]) == strlen("-o")))
			{
				if((i+1) <= arg_count && arg_verb[i+1])
				{
					*out_ptr = fopen(arg_verb[i+1],"rb");
					if(*out_ptr != NULL)
					{
						fclose(*out_ptr);
						cout << "Output file already exists, choose another\n";
						exit(1);
					}
					else
					{
						//fclose(out_ptr);
						*out_ptr = fopen(arg_verb[i+1],"wb");
						//fclose(out_ptr);
					}
				}
			}

			if(strstr(arg_verb[i], "-h") && strlen(arg_verb[i]) == strlen("-h"))
			{
				if(arg_count == 2)
				{//claudmfu
					cout << "wad2udmf version 0.3.5b\n";
					cout << "By: Chris Freund, (C) 2012, 2011, 2010, 2009\n";
					cout << "z34chris@yahoo.com\n\n";
					cout << "Usage: wad2udmf [switch] [value]...\n";
					cout << "====================================================\n";
					cout << "Switches: -h        displays this help dialogue\n";
					cout << "          -input  (-i)  binary wadfile to convert\n";
					cout << "          -output (-o)  converted udmf output file\n";
					cout << "          -cl       copy other non-map lumps\n";
					cout << "          -maps     specified maps to convert\n";
					cout << "          (previous two features will be added later)\n";
					exit(0);
				}
			}

			if(strstr(arg_verb[i], "-maps") && strlen(arg_verb[i]) == strlen("-maps"))
			{
				//if((i+1) <= arg_count && arg_verb[i+1])
				//{
					cout << "This not yet included feature will allow the user\n";
					cout << "to convert selected maps.\n";
					exit(0);
				//}
			}
		}
	}
	else
	{
		cout << "type \'wad2udmf -h\' for help";
		exit(0);
	}

}

char* stolower(char *c)
{
	int i=0;
	while(i < sizeof(c))
	{
		c[i] = tolower(c[i]);
		i++;
	}
	return c;
}

void write_lumpdirectory(FILE *out_ptr, ENTRY E[])
{
	int i = 1;
	
	printf("numlumps %d\n", E[0].lump_size);
	while(i <= E[0].lump_size)
	{
		fwrite((char*)&E[i],1,sizeof(E[i]),out_ptr);
		i++;
	}
}

void write_lump(FILE *in_ptr, FILE *out_ptr, long lump_offset)
{
	ENTRY L;
	uchar buf = NULL;
	int i = 0;

	fseek(in_ptr,lump_offset,SEEK_SET);
	fread(&L, sizeof(ENTRY), 1, in_ptr);
	//printf("%d %d %s\n",L.lump_offset,L.lump_size, L.lump_name);
	fseek(in_ptr,L.lump_offset,SEEK_SET);
	//printf("%d\n",ftell(in_ptr));
	while(i < L.lump_size)
	{
		fread(&buf,1,1, in_ptr);
		fwrite(&buf,1,1,out_ptr);
		i++;
	}
}

MAPSCRIPT write_udmfmap(FILE *in_ptr, FILE *out_ptr, int &lump_index)
{
	ENTRY L;
	SECTOR S;
	SIDEDEF SDF;
	LINEDEF LDF;
	zLINEDEF zLDF;
	THING T;
	zTHING zT;
	VERTEX V;
	MAPSCRIPT M;

	char buffer_8[9];
	char maplmps [13][9] = {"THINGS","LINEDEFS","SIDEDEFS","VERTEXES",
							"SEGS","SSECTORS","NODES","SECTORS",
							"REJECT","BLOCKMAP","BEHAVIOR","SCRIPT",0};
	int i=0, wad_format = _DOOM;
	float x = 1.1;
	uint udmfitem_num,last_offset;

	memset(buffer_8,0,sizeof(buffer_8));
	memset((void*)&L,0,sizeof(L));
	memset((void*)&S,0,sizeof(S));
	memset((void*)&SDF,0,sizeof(SDF));
	memset((void*)&LDF,0,sizeof(LDF));
	memset((void*)&zLDF,0,sizeof(zLDF));
	memset((void*)&T,0,sizeof(T));
	memset((void*)&zT,0,sizeof(zT));
	memset((void*)&V,0,sizeof(V));
	memset((void*)&M,0,sizeof(M));

	//*******begin check for behavior lump
	last_offset = ftell(in_ptr);

	fread(&L, sizeof(ENTRY), 1, in_ptr);
	while(strstr(L.lump_name, maplmps[i]) && i < (sizeof(maplmps)/sizeof(maplmps[0])-1))
	{
		if(strstr(L.lump_name, "BEHAVIOR"))
		{
			printf("hexen format map\n");
			wad_format = _ZDOOM;
		}

		fread(&L, sizeof(ENTRY), 1, in_ptr);
		i++;
	}

	fseek(in_ptr,last_offset,SEEK_SET);
	i = 0;
	//*******end check for behavior lump

	if(wad_format == _ZDOOM)
	{
		fprintf(out_ptr,"namespace = \"ZDoom\";\n\n");
	}else
	{
		fprintf(out_ptr,"namespace = \"ZDoomTranslated\";\n\n");
	}

	fprintf(out_ptr,"//Converted to UDMF with wad2udmf by Christopher M Freund\n\n");

	fread(&L, sizeof(ENTRY), 1, in_ptr);

	while(strstr(L.lump_name, maplmps[i]) && i < (sizeof(maplmps)/sizeof(maplmps[0])-1))
	{
		if(strlen(L.lump_name) > 8)
		{
			strncpy(buffer_8, L.lump_name, 8);
			printf("%s\n",buffer_8);
		}
		else
			printf("%s\n",L.lump_name);

		if(strstr(L.lump_name, "SSECTORS")
			|| strstr(L.lump_name, "SEGS")
			|| strstr(L.lump_name, "REJECT")
			|| strstr(L.lump_name, "BLOCKMAP")
			|| strstr(L.lump_name, "NODES"))
		{
			//check for other map lumps and increment lump index.
			lump_index++;
		}

		if(strstr(L.lump_name, "SECTORS") && !strstr(L.lump_name, "SSECTORS"))
		{
			lump_index++;
			udmfitem_num=0;
			last_offset = ftell(in_ptr);
			fseek(in_ptr,L.lump_offset,SEEK_SET);

			while(udmfitem_num < (L.lump_size / (uint)sizeof(S)))
			{
				fread(&S, sizeof(SECTOR),1,in_ptr);
				fprintf(out_ptr,"sector //sector %d \n{\n",udmfitem_num);
				fprintf(out_ptr,"heightfloor = %d;\nheightceiling = %d;\n",S.floor,S.ceil);
				
				if(strlen(S.floor_tex) > 8)
				{
					strncpy(buffer_8, S.floor_tex, 8);
					fprintf(out_ptr,"texturefloor = \"%s\";\n",buffer_8);
				}
				else
				fprintf(out_ptr,"texturefloor = \"%s\";\n",S.floor_tex);
				
				if(strlen(S.ceil_tex) > 8)
				{
					strncpy(buffer_8, S.ceil_tex, 8);
					fprintf(out_ptr,"textureceiling = \"%s\";\n",buffer_8);
				}
				else
				fprintf(out_ptr,"textureceiling = \"%s\";\n",S.ceil_tex);

				if(wad_format == _ZDOOM)
				{
					if(S.type > 0)
					{
						fprintf(out_ptr,"special = %d;\n",S.type);
					}
					else
						fprintf(out_ptr,"special = 0;\n");
				}
				else
				{
					if(S.type > 0)
					{
						fprintf(out_ptr,"type = %d;\n",S.type);
					}
					else
						fprintf(out_ptr,"type = 0;\n");
				}

				if(S.tag)
				{
					fprintf(out_ptr,"id = %d;\n",S.tag);
					fprintf(out_ptr,"dropactors = true;\n");
				}
				else
					fprintf(out_ptr,"id = 0;\n");

				fprintf(out_ptr,"lightlevel = %d;\n",S.light);
				fprintf(out_ptr,"}\n\n");
				udmfitem_num++;
			}

			fseek(in_ptr,last_offset,SEEK_SET);
			//printf("%x\n",ftell(in_ptr));
		}

		if(strstr(L.lump_name, "SIDEDEFS"))
		{
			lump_index++;
			udmfitem_num=0;
			last_offset = ftell(in_ptr);
			fseek(in_ptr,L.lump_offset,SEEK_SET);
			
			while(udmfitem_num < (L.lump_size / (uint)sizeof(SDF)))
			{
				fread(&SDF, sizeof(SIDEDEF),1,in_ptr);
				fprintf(out_ptr,"sidedef //sidedef %d \n{\n",udmfitem_num);
				if(strlen(SDF.upper_tex) > 1)
				{
					if(strlen(SDF.upper_tex) > 8)
					{
						strncpy(buffer_8, SDF.upper_tex, 8);
						fprintf(out_ptr,"texturetop = \"%s\";\n",buffer_8);
					}
					else
					fprintf(out_ptr,"texturetop = \"%s\";\n",SDF.upper_tex);
				}

				if(strlen(SDF.mid_tex) > 1)
				{
					if(strlen(SDF.mid_tex) > 8)
					{
						strncpy(buffer_8, SDF.mid_tex, 8);
						fprintf(out_ptr,"texturemiddle = \"%s\";\n",buffer_8);
					}
					else
					fprintf(out_ptr,"texturemiddle = \"%s\";\n",SDF.mid_tex);
				}

				if(strlen(SDF.lower_tex) > 1)
				{
					if(strlen(SDF.lower_tex) > 8)
					{
						strncpy(buffer_8, SDF.lower_tex, 8);
						fprintf(out_ptr,"texturebottom = \"%s\";\n",buffer_8);
					}
					else
					fprintf(out_ptr,"texturebottom = \"%s\";\n",SDF.lower_tex);
				}

				if(SDF.x_offset){fprintf(out_ptr,"offsetx = %d;\n",SDF.x_offset);}
				if(SDF.y_offset){fprintf(out_ptr,"offsety = %d;\n",SDF.y_offset);}
				fprintf(out_ptr,"sector = %d;\n}\n\n",SDF.sector);
				udmfitem_num++;
			}

			fseek(in_ptr,last_offset,SEEK_SET);
			//printf("%x\n",ftell(in_ptr));
		}

		if(strstr(L.lump_name, "LINEDEFS"))
		{
			lump_index++;
			udmfitem_num=0;
			last_offset = ftell(in_ptr);
			fseek(in_ptr,L.lump_offset,SEEK_SET);

			if(wad_format == _ZDOOM) //cf fixed 11.23.2010 one of these was = and not ==
			{
				while(udmfitem_num < (L.lump_size / (uint)sizeof(zLDF)))
				{
					fread(&zLDF, sizeof(zLINEDEF),1,in_ptr); 
					
					fprintf(out_ptr,"linedef //linedef %d \n{\n",udmfitem_num);
					if(zLDF.special)
					{
						//fprintf(out_ptr,"special = %d;\n",zLDF.special);
						if(zLDF.special == 121 && (zLDF.arg2 & 0x10))
						{
							fprintf(out_ptr,"wrapmidtex = true;\n");
						}
						else if(zLDF.special == 121 && (zLDF.arg2 & 0x20))
						{
							fprintf(out_ptr,"midtex3d = true;\n");
						}
						else if(zLDF.special == 121) //fixed Line_SetIdentification
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							fprintf(out_ptr,"id = %d;\n",zLDF.arg1);
							fprintf(out_ptr,"extraflags = %d;\n",zLDF.arg2);
						}
						else if(zLDF.special == 222) //Scroll_Texture_Model
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							fprintf(out_ptr,"id = %d;\n",0);
							fprintf(out_ptr,"scrollbits = %d;\n",zLDF.arg2);
						}
						else if(zLDF.special == 208) //fixed TranslucentLine
						{
							//fprintf(out_ptr,"special = %d;\n",zLDF.special);
							//fprintf(out_ptr,"arg0 = 0;\n");
							//fprintf(out_ptr,"arg1 = %d;\n",zLDF.arg2);
							//fprintf(out_ptr,"arg2 = %d;\n",zLDF.arg3);
							//fprintf(out_ptr,"translucent = true;\n");
							//fprintf(out_ptr,"id = %d;\n",zLDF.arg1);
							x = (float)zLDF.arg2/256;
							fprintf(out_ptr,"alpha = %1.2f;\n",x);
							fprintf(out_ptr,"renderstyle = \"%s\";\n",(zLDF.arg3)?"add":"translucent"); //fixed 10.2.2010
						}
						else if(zLDF.special == 215) //Teleport_Line
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							fprintf(out_ptr,"id = %d;\n",zLDF.arg1);
							fprintf(out_ptr,"arg0 = %d;\n",0);
							fprintf(out_ptr,"arg1 = %d;\n",zLDF.arg2);
						}
						else if(zLDF.special == 1) //fixed Polyobj_StartLine
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							fprintf(out_ptr,"arg0 = %d;\n",zLDF.arg1);
							fprintf(out_ptr,"arg1 = %d;\n",zLDF.arg2);
							fprintf(out_ptr,"arg2 = %d;\n",zLDF.arg3);
							//fprintf(out_ptr,"arg3 = %d;\n",0);
						}
						else if(zLDF.special == 5) //fixed Polyobj_Explicit
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							fprintf(out_ptr,"arg0 = %d;\n",zLDF.arg1);
							fprintf(out_ptr,"arg1 = %d;\n",zLDF.arg2);
							fprintf(out_ptr,"arg2 = %d;\n",zLDF.arg3);
							fprintf(out_ptr,"arg3 = %d;\n",zLDF.arg4);
							fprintf(out_ptr,"arg4 = %d;\n",0);
						}
						else if(zLDF.special == 181) //fixed Plane_Align
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							fprintf(out_ptr,"arg0 = %d;\n",zLDF.arg1);
							fprintf(out_ptr,"arg1 = %d;\n",zLDF.arg2);
							fprintf(out_ptr,"arg2 = %d;\n",0);
						}
						else
						{
							fprintf(out_ptr,"special = %d;\n",zLDF.special);
							
							if(zLDF.arg1)
								fprintf(out_ptr,"arg0 = %d;\n",zLDF.arg1);
							if(zLDF.arg2)
								fprintf(out_ptr,"arg1 = %d;\n",zLDF.arg2);
							if(zLDF.arg3)
								fprintf(out_ptr,"arg2 = %d;\n",zLDF.arg3);
							if(zLDF.arg4)
								fprintf(out_ptr,"arg3 = %d;\n",zLDF.arg4);
							if(zLDF.arg5)
								fprintf(out_ptr,"arg4 = %d;\n",zLDF.arg5);
						}
					}

					//if(zLDF.flags)
					//{
						if(zLDF.flags < 1)
							fprintf(out_ptr,"playercross = true;\n");
						if(zLDF.flags & 0x0001)
							fprintf(out_ptr,"blocking = true;\n");
						if(zLDF.flags & 0x0002)
							fprintf(out_ptr,"blockmonsters = true;\n");
						if(zLDF.flags & 0x0004)
							fprintf(out_ptr,"twosided = true;\n");
						if(zLDF.flags & 0x0008)
							fprintf(out_ptr,"dontpegtop = true;\n");
						if(zLDF.flags & 0x0010)
							fprintf(out_ptr,"dontpegbottom = true;\n");
						if(zLDF.flags & 0x0020)
							fprintf(out_ptr,"secret = true;\n");
						if(zLDF.flags & 0x0040)
							fprintf(out_ptr,"blocksound = true;\n");
						if(zLDF.flags & 0x0080)
							fprintf(out_ptr,"dontdraw = true;\n");
						if(zLDF.flags & 0x0100)
							fprintf(out_ptr,"mapped = true;\n");
						if(zLDF.flags & 0x0200)
							fprintf(out_ptr,"repeatspecial = true;\n");

/*cf fix 1.9.2012
This whole section concerning the linedef triggering was flipped upside down
in order to filter out the bits.  Some of these settings would cause a false
trigger in some events because the numbers where being AND'ed and a lesser
value would return even though a larger number was what was being AND'ed to
begin with.  Also, a check was added to make sure the return value was at
least the bitwise number we were looking for.
*/
						if ((zLDF.flags & 0x1C00) >= 0x1C00)
							fprintf(out_ptr,"missilecross = true;\nimpact = true;\n");//0x1c00  projectile cross or hit
						else if((zLDF.flags & 0x1800) >= 0x1800)
							fprintf(out_ptr,"blocking = true;\n");//0x1800  passthru	
						else if((zLDF.flags & 0x1400) >= 0x1400)
							fprintf(out_ptr,"missilecross = true;\n");//0x1400
						else if((zLDF.flags & 0x1000) >= 0x1000)
							fprintf(out_ptr,"playerpush = true;\n");//0x1000
						else if((zLDF.flags & 0x0C00) >= 0x0C00)
							fprintf(out_ptr,"impact = true;\n");//0xc00
						else if((zLDF.flags & 0x0800) >= 0x0800)
							fprintf(out_ptr,"monstercross = true;\n");//0x800
						else if((zLDF.flags & 0x0400) >= 0x0400)
							fprintf(out_ptr,"playeruse = true;\n");//0x400
						else
							fprintf(out_ptr,"playercross = true;\n");//0x0

						if(zLDF.flags & 0x2000)
							fprintf(out_ptr,"monsteractivate = true;\n");
						if(zLDF.flags & 0x4000)
							fprintf(out_ptr,"blockplayers = true;\n");
						if(zLDF.flags & 0x8000)
							fprintf(out_ptr,"blockeverything = true;\n");
							//fprintf(out_ptr,"playercross = true;\n");
						//if(zLDF.flags & 0x1800)
						//	fprintf(out_ptr,"use by playr pass thru;\n");
						//if(zLDF.flags & 0x2000)
						//	fprintf(out_ptr,"activ by all;\n");
						//if(zLDF.flags & 0x8000)
						//	fprintf(out_ptr,"block everything;\n");
					//}

					if(zLDF.right_side == 65535) //cf fix 12.08.2010
						fprintf(out_ptr,"sidefront = %d;\n",-1);
					else
						fprintf(out_ptr,"sidefront = %d;\n",zLDF.right_side);
						//printf("missing front side\n");

					if(zLDF.left_side == 65535) //cf fix 12.08.2010
						fprintf(out_ptr,"sideback = %d;\n",-1);
					else
						fprintf(out_ptr,"sideback = %d;\n",zLDF.left_side);
						//printf("missing back side\n");

					if(zLDF.v1 == -1)
						zLDF.v1 = 0;

					if(zLDF.v2 == -1)
						zLDF.v2 = 0;

					fprintf(out_ptr,"v1 = %d;\nv2 = %d;\n",zLDF.v1,zLDF.v2);
					fprintf(out_ptr,"}\n\n");

					udmfitem_num++;
				}
			}
			else
			{
				while(udmfitem_num < (L.lump_size / (uint)sizeof(LINEDEF)))
				{
					fread(&LDF, sizeof(LINEDEF),1,in_ptr);
					
					fprintf(out_ptr,"linedef //linedef %d \n{\n",udmfitem_num);
					if(LDF.tag)
					{
						fprintf(out_ptr,"id = %d;\n",LDF.tag);
					}
					else
						fprintf(out_ptr,"id = 0;\n");
					
					if(LDF.special)
					{
						fprintf(out_ptr,"special = %d;\n",LDF.special);
					}

					if(LDF.flags)
					{
						if(LDF.flags & 0x0001)
							fprintf(out_ptr,"blocking = true;\n");
						if(LDF.flags & 0x0002)
							fprintf(out_ptr,"blockmonsters = true;\n");
						if(LDF.flags & 0x0004)
							fprintf(out_ptr,"twosided = true;\n");
						if(LDF.flags & 0x0008)
							fprintf(out_ptr,"dontpegtop = true;\n");
						if(LDF.flags & 0x0010)
							fprintf(out_ptr,"dontpegbottom = true;\n");
						if(LDF.flags & 0x0020)
							fprintf(out_ptr,"secret = true;\n");
						if(LDF.flags & 0x0040)
							fprintf(out_ptr,"blocksound = true;\n");
						if(LDF.flags & 0x0080)
							fprintf(out_ptr,"dontdraw = true;\n");
						if(LDF.flags & 0x0100)
							fprintf(out_ptr,"mapped = true;\n");
					}

					//if(LDF.right_side > -1)//cf fix 11.23.2010
						fprintf(out_ptr,"sidefront = %d;\n",LDF.right_side);
					//else
						//fprintf(out_ptr,"sidefront = %d;\n",0);
						//printf("missing front\n");
					///if(LDF.left_side > -1)//cf fix 11.23.2010
						fprintf(out_ptr,"sideback = %d;\n",LDF.left_side);
					//else
						//fprintf(out_ptr,"sideback = %d;\n",0);
						//printf("missing back\n");
					fprintf(out_ptr,"v1 = %d;\nv2 = %d;\n",LDF.v1,LDF.v2);
					fprintf(out_ptr,"}\n\n");

					udmfitem_num++;
				}
			}


			fseek(in_ptr,last_offset,SEEK_SET);
			//printf("%x\n",ftell(in_ptr));
		}

		if(strstr(L.lump_name, "VERTEXES"))
		{
			lump_index++;
			udmfitem_num=0;
			last_offset = ftell(in_ptr);
			fseek(in_ptr,L.lump_offset,SEEK_SET);
			//fprintf(out_ptr,"\n%d\n",sizeof(V)); //fixed 11.2.2010
			while(udmfitem_num<(L.lump_size / (uint)sizeof(VERTEX)))
			{
				fread(&V, sizeof(VERTEX),1,in_ptr);
				fprintf(out_ptr,"vertex //vertex %d \n{\nx = %d.0;\ny = %d.0;\n}\n\n",udmfitem_num,V.x, V.y);
				udmfitem_num++;
			}

			fseek(in_ptr,last_offset,SEEK_SET);
			//printf("%x\n",ftell(in_ptr));
		}

		if(strstr(L.lump_name, "THINGS"))
		{
			lump_index++;
			udmfitem_num=0;
			last_offset = ftell(in_ptr);
			fseek(in_ptr,L.lump_offset,SEEK_SET);

			if(wad_format == _ZDOOM)
			{
				while(udmfitem_num < (L.lump_size / (uint)sizeof(zT)))
				{
					fread(&zT, sizeof(zTHING),1,in_ptr);
					fprintf(out_ptr,"thing //thing %d \n{\n", udmfitem_num);
					fprintf(out_ptr,"id = %d;\nx = %d.0;\ny = %d.0;\nheight = %d.0;\nangle = %d;\ntype = %d;\n",zT.id,zT.x_pos,zT.y_pos,zT.height,zT.angle,zT.type);
					
					if(zT.flags & 0x0001)
						fprintf(out_ptr,"skill1 = true;\nskill2 = true;\n");
					if(zT.flags & 0x0002)
						fprintf(out_ptr,"skill3 = true;\n");
					if(zT.flags & 0x0004)
						fprintf(out_ptr,"skill4 = true;\nskill5 = true;\n");
					if(zT.flags & 0x0008)
						fprintf(out_ptr,"ambush = true;\n");
					if(zT.flags & 0x0010)
						fprintf(out_ptr,"dormant = true;\n");
					if(zT.flags & 0x0100)
						fprintf(out_ptr,"single = true;\n");
					if(zT.flags & 0x0200)
						fprintf(out_ptr,"coop = true;\n");
					if(zT.flags & 0x0400)
						fprintf(out_ptr,"dm = true;\n");

					//if(zT.special > -1)
					//{
						fprintf(out_ptr,"special = %d;\n",zT.special);
						if(zT.arg1)
							fprintf(out_ptr,"arg0 = %d;\n",zT.arg1);
						if(zT.arg2)
							fprintf(out_ptr,"arg1 = %d;\n",zT.arg2);
						if(zT.arg3)
							fprintf(out_ptr,"arg2 = %d;\n",zT.arg3);
						if(zT.arg4)
							fprintf(out_ptr,"arg3 = %d;\n",zT.arg4);
						if(zT.arg5)
							fprintf(out_ptr,"arg4 = %d;\n",zT.arg5);
					//}
					/*
					if(T.flags & 0x0020)
						fprintf(out_ptr,"dm = true;\n");
					if(T.flags & 0x0040)
						fprintf(out_ptr,"coop = true;\n");
					*/
					fprintf(out_ptr,"}\n\n");
					udmfitem_num++;
				}
			}
			else
			{
				while(udmfitem_num < (L.lump_size / (uint)sizeof(T)))
				{
					fread(&T, sizeof(THING),1,in_ptr);
					fprintf(out_ptr,"thing //thing %d \n{\n", udmfitem_num);
					fprintf(out_ptr,"x = %d.0;\ny = %d.0;\nangle = %d;\ntype = %d;\n",T.x_pos,T.y_pos,T.angle,T.type);
					if(T.flags & 0x0001)
						fprintf(out_ptr,"skill1 = true;\nskill2 = true;\n");
					if(T.flags & 0x0002)
						fprintf(out_ptr,"skill3 = true;\n");
					if(T.flags & 0x0004)
						fprintf(out_ptr,"skill4 = true;\nskill5 = true;\n");
					if(T.flags & 0x0008)
						fprintf(out_ptr,"ambush = true;\n");
					if(T.flags & 0x000f)
						fprintf(out_ptr,"single = true;\n");
					if(T.flags & 0x0010)
						fprintf(out_ptr,"dm = true;\ncoop = true;\n");
					/*
					if(T.flags & 0x0020)
						fprintf(out_ptr,"dm = true;\n");
					if(T.flags & 0x0040)
						fprintf(out_ptr,"coop = true;\n");
					*/
					fprintf(out_ptr,"}\n\n");
					udmfitem_num++;
				}
			}

			fseek(in_ptr,last_offset,SEEK_SET);
			//printf("%x\n",ftell(in_ptr));
		}

		if(strstr(L.lump_name, "BEHAVIOR"))
		{
			lump_index++;
			M.behavior_offset = ftell(in_ptr) - sizeof(ENTRY);
		}

		if(strstr(L.lump_name, "SCRIPT"))
		{
			lump_index++;
			M.script_offset = ftell(in_ptr) - sizeof(ENTRY);
			strcpy(M.script_name, L.lump_name);
		}

		last_offset = ftell(in_ptr);
		fread(&L, sizeof(ENTRY), 1, in_ptr);
		i++;
	}
	fseek(in_ptr,last_offset,SEEK_SET);

	return M;
}