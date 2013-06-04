#ifndef _MUSIC_H_
#define _MUSIC_H_

/*
 * music.h
 *
 *  With this file, you can play music on the MB Leds. 
 *
 * Copyright 2011 - Benjamin Bonny    <benjamin.bonny@gmail.com>,
 *                  Cédric Le Ninivin <cedriclen@gmail.com>,
 *                  Guillaume Normand <guillaume.normand.gn@gmail.com>          
 *
 * All rights reserved.
 * MB Led
 * Telecom ParisTech - ELECINF344/ELECINF381
 *
 * This file is part of MB Led Project.
 *
 * MB Led Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MB Led Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MB Led Project.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */


typedef enum NOTE_VALUE{
  STOP = 0,
  QUAVER = 1,
  CROTCHET = 2,
  CROTCHET_QUAVER = 3,
  MINIM = 4,
  SEMIBREVE = 8,
} NOTE_VALUE;

typedef struct Note{
  uint16_t freq;
  NOTE_VALUE duration;
} Note;

typedef struct Music_score {
  const uint16_t *score;
  uint8_t voice;
  uint16_t start[4];
  uint16_t length;
  uint8_t tempo;
  uint16_t beat;
} Music_score;


void put_note (Note* note,uint16_t freq, NOTE_VALUE duration);
void play_note(Note note);
void play_music (Music_score *score, uint8_t v);
void build_music_score (Music_score *score);

void music_task();

#endif
