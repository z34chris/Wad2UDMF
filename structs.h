#include "defs.h"

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
	ushort v1;
	ushort v2;
	short flags;
	short special;
	short tag;
	ushort right_side;
	ushort left_side;
};

struct zLINEDEF
{
	ushort v1;
	ushort v2;
	ushort flags;
	uchar special;
	uchar arg1;
	uchar arg2;
	uchar arg3;
	uchar arg4;
	uchar arg5;
	ushort right_side;
	ushort left_side;
};

struct SIDEDEF
{
	short x_offset;
	short y_offset;
	char upper_tex[8];
	char lower_tex[8];
	char mid_tex[8];
	ushort sector;
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
	uchar special;
	char arg1;
	char arg2;
	char arg3;
	char arg4;
	char arg5;
};

struct HEADER
{
	char wad_type[3];
	int num_lumps;
	int start_of_directory;
};

struct ENTRY
{
	int lump_offset;
	int lump_size;
	char lump_name[8];
};

struct MAPSCRIPT
{
	int behavior_offset;
	int script_offset;
	char script_name[8];
};