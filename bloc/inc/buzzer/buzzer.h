/*
 * buzzer.h
 *
 * Functions to define how the buzzer works (only for MBLed)
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

#ifndef H_BUZZER
#define H_BUZZER

/*
  Do 32,70Hz 65,41Hz 130,81Hz 261,63Hz 523,25Hz 1046,50Hz 2093,00Hz 4186,01Hz
  Do♯ 34,65Hz 69,30Hz 138,59Hz 277,18Hz 554,37Hz 1108,73Hz 2217,46Hz 4434,92Hz
  Ré 36,71Hz 73,42Hz 146,83Hz 293,66Hz 587,33Hz 1174,66Hz 2349,32Hz 4698,64Hz
  Ré♯ 38,89Hz 77,78Hz 155,56Hz 311,13Hz 622,25Hz 1244,51Hz 2489,02Hz 4978,03Hz
  Mi 41,20Hz 82,41Hz 164,81Hz 329,63Hz 659,26Hz 1318,51Hz 2637,02Hz 5274,04Hz
  Fa 43,65Hz 87,31Hz 174,61Hz 349,23Hz 698,46Hz 1396,91Hz 2793,83Hz 5587,65Hz
  Fa♯ 46,25Hz 92,50Hz 185,00Hz 369,99Hz 739,99Hz 1479,98Hz 2959,96Hz 5919,91Hz
  Sol 49,00Hz 98,00Hz 196,00Hz 392,00Hz 783,99Hz 1567,98Hz 3135,96Hz 6271,93Hz
  Sol♯ 51,91Hz 103,83Hz 207,65Hz 415,30Hz 830,61Hz 1661,22Hz 3322,44Hz 6644,88Hz
  La 55,00Hz 110,00Hz 220,00Hz 440,00Hz 880,00Hz 1760,00Hz 3520,00Hz 7040,00Hz
  La♯ 58,27Hz 116,54Hz 233,08Hz 466,16Hz 932,33Hz 1864,66Hz 3729,31Hz 7458,62Hz
  Si 61,74Hz 123,47Hz 246,94Hz 493,88Hz 987,77Hz 1975,53Hz 3951,07Hz 7902,13Hz
*/


#define DO1   33
#define DOd1  35
#define RE1   37
#define REd1  39
#define MI1   41
#define FA1   44
#define FAd1  46
#define SOL1  49
#define SOLd1 52
#define LA1   55
#define LAd1  58
#define SI1   62

#define DO2   65
#define DOd2  69
#define RE2   73
#define REd2  78
#define MI2   82
#define FA2   87
#define FAd2  92
#define SOL2  98
#define SOLd2 104
#define LA2   110
#define LAd2  117
#define SI2   123

#define DO3   131
#define DOd3  139
#define RE3   147
#define REd3  156
#define MI3   165
#define FA3   175
#define FAd3  185
#define SOL3  196
#define SOLd3 207
#define LA3   220
#define LAd3  233
#define SI3   247

/**********************************************************/

#define DO4   262
#define DOd4  277
#define RE4   294
#define REd4  311
#define MI4   330
#define FA4   349
#define FAd4  370
#define SOL4  392
#define SOLd4 415
#define LA4   440
#define LAd4  466
#define SI4   494

#define DO5   523
#define DOd5  554
#define RE5   587
#define REd5  622
#define MI5   659
#define FA5   698
#define FAd5  739
#define SOL5  783
#define SOLd5 830
#define LA5   880
#define LAd5  932
#define SI5   988

#define DO6   1046
#define DOd6  1109
#define RE6   1175
#define REd6  1245
#define MI6   1319
#define FA6   1397
#define FAd6  1480
#define SOL6  1568
#define SOLd6 1661
#define LA6   1760
#define LAd6  1865
#define SI6   1976

#define DO7   2093
#define DOd7  2217
#define RE7   2349
#define REd7  2489
#define MI7   2637
#define FA7   2794
#define FAd7  2960
#define SOL7  3136
#define SOLd7 3322
#define LA7   3520
#define LAd7  3729
#define SI7   3951



/**********************************************************/


#define DO8   4186
#define DOd8  4435
#define RE8   4699
#define REd8  4978
#define MI8   5274
#define FA8   5588
#define FAd8  5920
#define SOL8  6272
#define SOLd8 6645
#define LA8   7040
#define LAd8  7459
#define SI8   7902
 
#define BUZZER_VOLUME  300

void buzzer_init();
void buzzer_put_freq(unsigned long freq);
void buzzer_play_gamme();
void buzzer_check();

#endif
