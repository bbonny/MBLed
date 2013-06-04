/*
 * music.c
 *
 * With this file, you can play music on the MB Leds.
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

#include <FreeRTOS.h>
#include <task.h>

#include "application.h"
#include "buzzer.h"
#include "playlist.h"
#include "music.h"
#include "manager.h"


uint16_t BEAT;


/*
 * put_note (Note * note, uint16_t freq, NOTE_VALUE duration)
 *
 * Creates a note
 *
 */
void put_note (Note * note, uint16_t freq, NOTE_VALUE duration){
  note->freq = freq;
  note->duration = duration;
}

/*
 *  play_note(Note note)
 *
 *  play the note during the good period of time 
 *
 */
void play_note(Note note){
  buzzer_put_freq(0);
  vTaskDelay(10);
  buzzer_put_freq(note.freq);
  vTaskDelay(note.duration * BEAT);
}



/*
 * play_music (Music_score * score, uint8_t v)
 * 
 * Play the music score in parameters
 *
 */
void play_music (Music_score * score, uint8_t v){
  //uint16_t i;
  uint32_t delay;
  Note note;
  BEAT = score->beat;  
  delay= (BEAT * score->tempo *4* score->length /score->voice);
    while (((current_time() % delay) < ((delay/4)-10))
  	 || ((current_time() % delay) > ((delay/4)+10))){}
  for (uint16_t j = score->start[v] ; j < score->start[v+1]-1 ; j+= 2){
    //Music only during the Tetris!
    if(current_demo == MANAGER_DEMO9){
      put_note(&note, score->score[j], score->score[j+1]);
      play_note(note);    
    }
    else
      return;
  }
  buzzer_put_freq(0);
}

/*
 * build_music_score(Music_score *score)
 * 
 * Build music scores parameters allowing it to be played
 *
 */

void build_music_score (Music_score *score){
  uint16_t i= 3;
  score->voice = score->score[1];
  score->beat = score->score[0];
  score->tempo = score->score[2];
  score-> start[0] = i;
  writeDebugInit("Number of voice :");
  writeHexDebug(score->voice); 
  writeDebugInit("\r\n");
  for (uint8_t v =0; v < score->voice; v++){
    while (score->score[i+1] != 0){
	i+=2;
    }
    i+=2;
    writeDebugInit("Start at :");
    writeHex16Debug(i);
    writeDebugInit(" for ");
    writeHexDebug(v+1);
    writeDebugInit("\r\n");
    
    score->start[v+1] = i;
  }
  score->length = i;
}


/*
 * music_task()
 *
 * Play a music score when it's its turn
 *
 */
void music_task(){
  Music_score score;
  int8_t* me;
  score.score= play_list[1];
  build_music_score(&score);
  for(;;){
    if( (current_demo == MANAGER_DEMO9) && isLeader() && (getMembersInNetwork()>=7) ){
      me = get_block_me();
      if ((maxI-minI) > (maxJ-minJ))
	play_music(&score,(me[I]-minI)%score.voice);
      else
	play_music(&score,(me[J]-minJ)%score.voice);
    }
    else{
      vTaskDelay(200);
    }
    vTaskDelay(150); 
  }
}

