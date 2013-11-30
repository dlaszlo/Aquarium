/*
 * IRcodes.h
 *
 *  Created on: 2013.09.02.
 *      Author: Dávid
 */

#include <stdio.h>

#ifndef IRCODES_H_
#define IRCODES_H_

#define KEY_FOLDER_PREV 605
#define KEY_FOLDER_NEXT 669
#define KEY_REPEAT 541
#define KEY_CHANNEL_PREV 733
#define KEY_CHANNEL_NEXT 765
#define KEY_EQ 573
#define KEY_TRACK_PREV 31
#define KEY_TRACK_NEXT 87
#define KEY_PLAY_PAUSE 111
#define KEY_MINUS 103
#define KEY_PLUS 79
#define KEY_0 151
#define KEY_1 207
#define KEY_2 231
#define KEY_3 645
#define KEY_4 239
#define KEY_5 199
#define KEY_6 677
#define KEY_7 701
#define KEY_8 693
#define KEY_9 685

static uint16_t IR_KEYS[] =
{
		KEY_FOLDER_PREV, KEY_FOLDER_NEXT, KEY_REPEAT, KEY_CHANNEL_PREV, KEY_CHANNEL_NEXT,
		KEY_EQ, KEY_TRACK_PREV, KEY_TRACK_NEXT, KEY_PLAY_PAUSE, KEY_MINUS, KEY_PLUS,
		KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9
};

#endif /* IRCODES_H_ */
