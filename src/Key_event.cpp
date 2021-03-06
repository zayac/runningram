/*
 * File:   Key_event.cpp
 * Author: necto
 *
 * Created on January 21, 2010, 3:04 PM
 */
#include "Key_event.h"

const char* Key_names[] =
{
	"unknown", //0
	"", //1
	"", //2
	"", //3
	"", //4
	"", //5
	"", //6
	"", //7
	"backspace", //8
	"tab", //9
	"", //10
	"", //11
	"clear", //12
	"return", //13
	"", //14
	"", //15
	"", //16
	"", //17
	"", //18
	"pause", //19
	"", //20
	"", //21
	"", //22
	"", //23
	"", //24
	"", //25
	"", //26
	"escape", //27
	"", //28
	"", //29
	"", //30
	"", //31
	"space", //32
	"exclaim", //33
	"quotedbl", //34
	"hash", //35
	"dollar", //36
	"", //37
	"ampersand", //38
	"quote", //39
	"leftparen", //40
	"rightparen", //41
	"asterisk", //42
	"plus", //43
	"comma", //44
	"minus", //45
	"period", //46
	"slash", //47
	"0", //48
	"1", //49
	"2", //50
	"3", //51
	"4", //52
	"5", //53
	"6", //54
	"7", //55
	"8", //56
	"9", //57
	"colon", //58
	"semicolon", //59
	"less", //60
	"equals", //61
	"greater", //62
	"question", //63
	"at", //64

	"", //65
	"", //66
	"", //67
	"", //68
	"", //69
	"", //70
	"", //71
	"", //72
	"", //73
	"", //74
	"", //75
	"", //76
	"", //77
	"", //78
	"", //79
	"", //80
	"", //81
	"", //82
	"", //83
	"", //84
	"", //85
	"", //86
	"", //87
	"", //88
	"", //89
	"", //90
	/*
	   skip uppercase letters
	 */
	"leftbracket", //91
	"backslash", //92
	"rightbracket",// 93
	"caret", //94
	"underscore", //95
	"backquote", //96
	"a", //97
	"b", //98
	"c", //99
	"d", //100
	"e", //101
	"f", //102
	"g", //103
	"h", //104
	"i", //105
	"j", //106
	"k", //107
	"l", //108
	"m", //109
	"n", //110
	"o", //111
	"p", //112
	"q", //113
	"r", //114
	"s", //115
	"t", //116
	"u", //117
	"v", //118
	"w", //119
	"x", //120
	"y", //121
	"z", //122
	"", //123
	"", //124
	"", //125
	"", //126
	"delete", //127
	/* end of ascii mapped keysyms */
        /*@}*/
	"", //128
	"", //129
	"", //130
	"", //131
	"", //132
	"", //133
	"", //134
	"", //135
	"", //136
	"", //137
	"", //138
	"", //139
	"", //140
	"", //141
	"", //142
	"", //143
	"", //144
	"", //145
	"", //146
	"", //147
	"", //148
	"", //149
	"", //150
	"", //151
	"", //152
	"", //153
	"", //154
	"", //155
	"", //156
	"", //157
	"", //158
	"", //159

	/** @name international keyboard syms */
        /*@{*/
	"world_0", //160 /* 0xa0 */
	"world_1", //161
	"world_2", //162
	"world_3", //163
	"world_4", //164
	"world_5", //165
	"world_6", //166
	"world_7", //167
	"world_8", //168
	"world_9", //169
	"world_10", //170
	"world_11", //171
	"world_12", //172
	"world_13", //173
	"world_14", //174
	"world_15", //175
	"world_16", //176
	"world_17", //177
	"world_18", //178
	"world_19", //179
	"world_20", //180
	"world_21", //181
	"world_22", //182
	"world_23", //183
	"world_24", //184
	"world_25", //185
	"world_26", //186
	"world_27", //187
	"world_28", //188
	"world_29", //189
	"world_30", //190
	"world_31", //191
	"world_32", //192
	"world_33", //193
	"world_34", //194
	"world_35", //195
	"world_36", //196
	"world_37", //197
	"world_38", //198
	"world_39", //199
	"world_40", //200
	"world_41", //201
	"world_42", //202
	"world_43", //203
	"world_44", //204
	"world_45", //205
	"world_46", //206
	"world_47", //207
	"world_48", //208
	"world_49", //209
	"world_50", //210
	"world_51", //211
	"world_52", //212
	"world_53", //213
	"world_54", //214
	"world_55", //215
	"world_56", //216
	"world_57", //217
	"world_58", //218
	"world_59", //219
	"world_60", //220
	"world_61", //221
	"world_62", //222
	"world_63", //223
	"world_64", //224
	"world_65", //225
	"world_66", //226
	"world_67", //227
	"world_68", //228
	"world_69", //229
	"world_70", //230
	"world_71", //231
	"world_72", //232
	"world_73", //233
	"world_74", //234
	"world_75", //235
	"world_76", //236
	"world_77", //237
	"world_78", //238
	"world_79", //239
	"world_80", //240
	"world_81", //241
	"world_82", //242
	"world_83", //243
	"world_84", //244
	"world_85", //245
	"world_86", //246
	"world_87", //247
	"world_88", //248
	"world_89", //249
	"world_90", //250
	"world_91", //251
	"world_92", //252
	"world_93", //253
	"world_94", //254
	"world_95", //255 /* 0xff */
        /*@}*/

	/** @name numeric keypad */
        /*@{*/
	"kp0", //256
	"kp1", //257
	"kp2", //258
	"kp3", //259
	"kp4", //260
	"kp5", //261
	"kp6", //262
	"kp7", //263
	"kp8", //264
	"kp9", //265
	"kp_period", //266
	"kp_divide", //267
	"kp_multiply", //268
	"kp_minus", //269
	"kp_plus", //270
	"kp_enter", //271
	"kp_equals", //272
        /*@}*/

	/** @name arrows + home/end pad */
        /*@{*/
	"up", //273
	"down", //274
	"right", //275
	"left", //276
	"insert", //277
	"home", //278
	"end", //279
	"pageup", //280
	"pagedown", //281
        /*@}*/

	/** @name function keys */
        /*@{*/
	"f1", //282
	"f2", //283
	"f3", //284
	"f4", //285
	"f5", //286
	"f6", //287
	"f7", //288
	"f8", //289
	"f9", //290
	"f10", //291
	"f11", //292
	"f12", //293
	"f13", //294
	"f14", //295
	"f15", //296
	"", //297
	"", //298
	"", //299
        /*@}*/

	/** @name key state modifier keys */
        /*@{*/
	"numlock", //300
	"capslock", //301
	"scrollock", //302
	"rshift", //303
	"lshift", //304
	"rctrl", //305
	"lctrl", //306
	"ralt", //307
	"lalt", //308
	"rmeta", //309
	"lmeta", //310
	"lsuper", //311 /**< left "windows" key */
	"rsuper", //312 /**< right "windows" key */
	"mode", //313 /**< "alt gr" key */
	"compose", //314 /**< multi-key compose key */
        /*@}*/

	/** @name miscellaneous function keys */
        /*@{*/
	"help", //315
	"print", //316
	"sysreq", //317
	"break", //318
	"menu", //319
	"power", //320 /**< power macintosh power key */
	"euro", //321 /**< some european keyboards */
	"undo", //322 /**< atari keyboard has undo */
	"end"};


const char* getKeyName (Key_id id)
{
	return Key_names [id];
}
#include <string.h>
Key_id getKeyId (const char* name)
{
	if (strlen(name) == 1 && 0 <= name[0] && name[0] <= 127)//ASCII mapped part
		return (Key_id)name[0];

	for (int i = 0; i < NKEYS; ++i)
	{
		if (strcmp (name, Key_names[i]) == 0) return (Key_id)i;
	}
	return KI_UNKNOWN;

}