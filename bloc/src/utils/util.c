/*
 * util.c
 *
 * Util functions to convert numbers.
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


//////////////////////////////////////////////////////////
//  void short_to_string( char* res, unsigned short s ) //
//  convert a short in a string                         //
////////////////////////////////////////////////////////// 
void short_to_string( char* res, unsigned short s ){
  unsigned char c;
  unsigned short r = 0;

  /* 10 000 digit*/
  c = (( s - r ) / 10000 );
  r = r + ( c * 10000 );
  *res++ = c + 0x30;

  /* 1 000 digit*/
  c = (( s - r ) / 1000 );
  r = r + ( c * 1000 );
  *res++ = c + 0x30;

  /* 100 digit*/
  c = (( s - r ) / 100 );
  r = r + ( c * 100 );
  *res++ = c + 0x30;

  /* 10 digit*/
  c = (( s - r ) / 10 );
  r = r + ( c * 10 );
  *res++ = c + 0x30;

  /* 1 digit*/
  c = (( s - r ) );
  r = r + ( c  );
  *res++ = c + 0x30;

  *res++ = '\0';
}


////////////////////////////////////////////////////////////
// unsigned short is_Number(char * s, unsigned short end) //
// return 1 if the string represent a decimal number      //
////////////////////////////////////////////////////////////
unsigned short is_Number(char * s, unsigned short end){
  unsigned short i;
  for(i=0; i<end; i++)
    if(s[i]>57 || s[i]<48)
      return 0;
  return 1;
}

///////////////////////////////////////////////////////////
// unsigned long string_dec_to_int(char *s)              //
// transform a decimal number into a ascii string        //
/////////////////////////////////////////////////////////// 
unsigned long string_dec_to_int(char *s){
  unsigned long cpt=0;
  unsigned short i = 0;

  while(s[i]!='\0'){
    cpt = cpt*10 + s[i] - '0';
    i++;
  }
  return cpt;
}


///////////////////////////////////////////////////////////
// unsigned long string_hex_to_int(char *s)              //
// transform a decimal number into a ascii string        //
/////////////////////////////////////////////////////////// 
unsigned long string_hex_to_int(char *s){
  unsigned long cpt = 0;
  unsigned short i = 0;

  while(s[i]!='\0'){
    if((s[i] >= 65) && (s[i] <= 70))
      cpt = cpt*16 + s[i] - 'A' +10;
    else
      cpt = cpt*16 + s[i] - '0';
    i++;
  }
  return cpt;
}


unsigned short char_to_int(char c){
  if((c<='9') && (c>='0'))
    return c - '0';
  
  if((c<='A') && (c>='F'))
    return c - 'A' + 10;
  
  return 0;
}

 
unsigned short string_33_to_int(char c){
  if((c<='9') && (c>='0'))
    return c - '0';
  
  if((c<='A') && (c>='Z'))
    return c - 'A' + 10;
  
  return 0;
}
