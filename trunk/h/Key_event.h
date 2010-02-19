/* 
 * File:   Key_event.h
 * Author: necto
 *
 * Created on February 17, 2010, 8:37 PM
 */

#ifndef _KEY_EVENT_H
#define	_KEY_EVENT_H

#include "mstdint.h"

enum Key_id
{
        /** @name ASCII mapped keysyms
         *  The keyboard syms have been cleverly chosen to map to ASCII
         */
        /*@{*/
	KI_UNKNOWN		= 0,
	KI_FIRST		= 0,
	KI_BACKSPACE		= 8,
	KI_TAB		= 9,
	KI_CLEAR		= 12,
	KI_RETURN		= 13,
	KI_PAUSE		= 19,
	KI_ESCAPE		= 27,
	KI_SPACE		= 32,
	KI_EXCLAIM		= 33,
	KI_QUOTEDBL		= 34,
	KI_HASH		= 35,
	KI_DOLLAR		= 36,
	KI_AMPERSAND		= 38,
	KI_QUOTE		= 39,
	KI_LEFTPAREN		= 40,
	KI_RIGHTPAREN		= 41,
	KI_ASTERISK		= 42,
	KI_PLUS		= 43,
	KI_COMMA		= 44,
	KI_MINUS		= 45,
	KI_PERIOD		= 46,
	KI_SLASH		= 47,
	KI_0			= 48,
	KI_1			= 49,
	KI_2			= 50,
	KI_3			= 51,
	KI_4			= 52,
	KI_5			= 53,
	KI_6			= 54,
	KI_7			= 55,
	KI_8			= 56,
	KI_9			= 57,
	KI_COLON		= 58,
	KI_SEMICOLON		= 59,
	KI_LESS		= 60,
	KI_EQUALS		= 61,
	KI_GREATER		= 62,
	KI_QUESTION		= 63,
	KI_AT			= 64,
	/*
	   Skip uppercase letters
	 */
	KI_LEFTBRACKET	= 91,
	KI_BACKSLASH		= 92,
	KI_RIGHTBRACKET	= 93,
	KI_CARET		= 94,
	KI_UNDERSCORE		= 95,
	KI_BACKQUOTE		= 96,
	KI_a			= 97,
	KI_b			= 98,
	KI_c			= 99,
	KI_d			= 100,
	KI_e			= 101,
	KI_f			= 102,
	KI_g			= 103,
	KI_h			= 104,
	KI_i			= 105,
	KI_j			= 106,
	KI_k			= 107,
	KI_l			= 108,
	KI_m			= 109,
	KI_n			= 110,
	KI_o			= 111,
	KI_p			= 112,
	KI_q			= 113,
	KI_r			= 114,
	KI_s			= 115,
	KI_t			= 116,
	KI_u			= 117,
	KI_v			= 118,
	KI_w			= 119,
	KI_x			= 120,
	KI_y			= 121,
	KI_z			= 122,
	KI_DELETE		= 127,
	/* End of ASCII mapped keysyms */
        /*@}*/

	/** @name International keyboard syms */
        /*@{*/
	KI_WORLD_0		= 160,		/* 0xA0 */
	KI_WORLD_1		= 161,
	KI_WORLD_2		= 162,
	KI_WORLD_3		= 163,
	KI_WORLD_4		= 164,
	KI_WORLD_5		= 165,
	KI_WORLD_6		= 166,
	KI_WORLD_7		= 167,
	KI_WORLD_8		= 168,
	KI_WORLD_9		= 169,
	KI_WORLD_10		= 170,
	KI_WORLD_11		= 171,
	KI_WORLD_12		= 172,
	KI_WORLD_13		= 173,
	KI_WORLD_14		= 174,
	KI_WORLD_15		= 175,
	KI_WORLD_16		= 176,
	KI_WORLD_17		= 177,
	KI_WORLD_18		= 178,
	KI_WORLD_19		= 179,
	KI_WORLD_20		= 180,
	KI_WORLD_21		= 181,
	KI_WORLD_22		= 182,
	KI_WORLD_23		= 183,
	KI_WORLD_24		= 184,
	KI_WORLD_25		= 185,
	KI_WORLD_26		= 186,
	KI_WORLD_27		= 187,
	KI_WORLD_28		= 188,
	KI_WORLD_29		= 189,
	KI_WORLD_30		= 190,
	KI_WORLD_31		= 191,
	KI_WORLD_32		= 192,
	KI_WORLD_33		= 193,
	KI_WORLD_34		= 194,
	KI_WORLD_35		= 195,
	KI_WORLD_36		= 196,
	KI_WORLD_37		= 197,
	KI_WORLD_38		= 198,
	KI_WORLD_39		= 199,
	KI_WORLD_40		= 200,
	KI_WORLD_41		= 201,
	KI_WORLD_42		= 202,
	KI_WORLD_43		= 203,
	KI_WORLD_44		= 204,
	KI_WORLD_45		= 205,
	KI_WORLD_46		= 206,
	KI_WORLD_47		= 207,
	KI_WORLD_48		= 208,
	KI_WORLD_49		= 209,
	KI_WORLD_50		= 210,
	KI_WORLD_51		= 211,
	KI_WORLD_52		= 212,
	KI_WORLD_53		= 213,
	KI_WORLD_54		= 214,
	KI_WORLD_55		= 215,
	KI_WORLD_56		= 216,
	KI_WORLD_57		= 217,
	KI_WORLD_58		= 218,
	KI_WORLD_59		= 219,
	KI_WORLD_60		= 220,
	KI_WORLD_61		= 221,
	KI_WORLD_62		= 222,
	KI_WORLD_63		= 223,
	KI_WORLD_64		= 224,
	KI_WORLD_65		= 225,
	KI_WORLD_66		= 226,
	KI_WORLD_67		= 227,
	KI_WORLD_68		= 228,
	KI_WORLD_69		= 229,
	KI_WORLD_70		= 230,
	KI_WORLD_71		= 231,
	KI_WORLD_72		= 232,
	KI_WORLD_73		= 233,
	KI_WORLD_74		= 234,
	KI_WORLD_75		= 235,
	KI_WORLD_76		= 236,
	KI_WORLD_77		= 237,
	KI_WORLD_78		= 238,
	KI_WORLD_79		= 239,
	KI_WORLD_80		= 240,
	KI_WORLD_81		= 241,
	KI_WORLD_82		= 242,
	KI_WORLD_83		= 243,
	KI_WORLD_84		= 244,
	KI_WORLD_85		= 245,
	KI_WORLD_86		= 246,
	KI_WORLD_87		= 247,
	KI_WORLD_88		= 248,
	KI_WORLD_89		= 249,
	KI_WORLD_90		= 250,
	KI_WORLD_91		= 251,
	KI_WORLD_92		= 252,
	KI_WORLD_93		= 253,
	KI_WORLD_94		= 254,
	KI_WORLD_95		= 255,		/* 0xFF */
        /*@}*/

	/** @name Numeric keypad */
        /*@{*/
	KI_KP0		= 256,
	KI_KP1		= 257,
	KI_KP2		= 258,
	KI_KP3		= 259,
	KI_KP4		= 260,
	KI_KP5		= 261,
	KI_KP6		= 262,
	KI_KP7		= 263,
	KI_KP8		= 264,
	KI_KP9		= 265,
	KI_KP_PERIOD		= 266,
	KI_KP_DIVIDE		= 267,
	KI_KP_MULTIPLY	= 268,
	KI_KP_MINUS		= 269,
	KI_KP_PLUS		= 270,
	KI_KP_ENTER		= 271,
	KI_KP_EQUALS		= 272,
        /*@}*/

	/** @name Arrows + Home/End pad */
        /*@{*/
	KI_UP			= 273,
	KI_DOWN		= 274,
	KI_RIGHT		= 275,
	KI_LEFT		= 276,
	KI_INSERT		= 277,
	KI_HOME		= 278,
	KI_END		= 279,
	KI_PAGEUP		= 280,
	KI_PAGEDOWN		= 281,
        /*@}*/

	/** @name Function keys */
        /*@{*/
	KI_F1			= 282,
	KI_F2			= 283,
	KI_F3			= 284,
	KI_F4			= 285,
	KI_F5			= 286,
	KI_F6			= 287,
	KI_F7			= 288,
	KI_F8			= 289,
	KI_F9			= 290,
	KI_F10		= 291,
	KI_F11		= 292,
	KI_F12		= 293,
	KI_F13		= 294,
	KI_F14		= 295,
	KI_F15		= 296,
        /*@}*/

	/** @name Key state modifier keys */
        /*@{*/
	KI_NUMLOCK		= 300,
	KI_CAPSLOCK		= 301,
	KI_SCROLLOCK		= 302,
	KI_RSHIFT		= 303,
	KI_LSHIFT		= 304,
	KI_RCTRL		= 305,
	KI_LCTRL		= 306,
	KI_RALT		= 307,
	KI_LALT		= 308,
	KI_RMETA		= 309,
	KI_LMETA		= 310,
	KI_LSUPER		= 311,		/**< Left "Windows" key */
	KI_RSUPER		= 312,		/**< Right "Windows" key */
	KI_MODE		= 313,		/**< "Alt Gr" key */
	KI_COMPOSE		= 314,		/**< Multi-key compose key */
        /*@}*/

	/** @name Miscellaneous function keys */
        /*@{*/
	KI_HELP		= 315,
	KI_PRINT		= 316,
	KI_SYSREQ		= 317,
	KI_BREAK		= 318,
	KI_MENU		= 319,
	KI_POWER		= 320,		/**< Power Macintosh power key */
	KI_EURO		= 321,		/**< Some european keyboards */
	KI_UNDO		= 322,		/**< Atari keyboard has Undo */
        /*@}*/

	/* Add any other keys here */

	NKEYS
};

/** Enumeration of valid key mods (possibly OR'd together) */
enum Key_mode
{
	KM_NONE  = 0x0000,
	KM_LSHIFT= 0x0001,
	KM_RSHIFT= 0x0002,
	KM_LCTRL = 0x0040,
	KM_RCTRL = 0x0080,
	KM_LALT  = 0x0100,
	KM_RALT  = 0x0200,
	KM_LMETA = 0x0400,
	KM_RMETA = 0x0800,
	KM_NUM   = 0x1000,
	KM_CAPS  = 0x2000,
	KM_MODE  = 0x4000,
	KM_RESERVED = 0x8000
};

#define KM_CTRL	(KM_LCTRL|KM_RCTRL)
#define KM_SHIFT	(KM_LSHIFT|KM_RSHIFT)
#define KM_ALT	(KM_LALT|KM_RALT)
#define KM_META	(KM_LMETA|KM_RMETA)

#ifdef _SDL_keysym_h
inline SDLKey Get_sdl_code (Key_id what)
{
	return static_cast<SDLKey> (what);
}
inline Key_id Get_my_code (SDLKey what)
{
	return static_cast<Key_id> (what);
}

inline SDLMod Get_sdl_mode (Key_mode what)
{
	return static_cast<SDLMod> (what);
}
inline Key_mode Get_my_mode (SDLMod what)
{
	return static_cast<Key_mode> (what);
}
#endif


enum Events
{
       EV_NOEVENT = 0,			/**< Unused (do not remove) */
       EV_ACTIVEEVENT,			/**< Application loses/gains visibility (for capability with SDL::event) */
       EV_KEYDOWN,			/**< Keys pressed */
       EV_KEYUP,			/**< Keys released */
       EV_MOUSEMOTION,			/**< Mouse moved */
       EV_MOUSEBUTTONDOWN,		/**< Mouse button pressed */
       EV_MOUSEBUTTONUP		/**< Mouse button released */
};

struct Kbd_event
{
	Uint8 type;
	Key_mode mod;
	Key_id ki;
	
	Kbd_event (Uint8 type_, Key_mode mod_, Key_id ki_)
		:type (type_), mod (mod_), ki (ki_) {}
	Kbd_event (const Kbd_event& src)
		:type (src.type), mod (src.mod), ki (src.ki) {}

	bool Contain (const Kbd_event& that) const
	{
		return that.type == type && that.ki == ki && ((that.mod&mod) == that.mod);
	}
#ifdef _SDL_events_h
	Kbd_event (const SDL_KeyboardEvent& src)
		:type (src.type), mod (Get_my_mode (src.keysym.mod)), ki(Get_my_code (src.keysym.sym)) {}
#endif
};

extern const char* Key_names[];

const char* Get_key_name (Key_id);
Key_id Get_key_id (const char* name);

#endif	/* _KEY_EVENT_H */

