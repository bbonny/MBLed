/*
 * util.h
 *
 * ( descripts the module here ) 
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

#ifndef H_UTIL
#define H_UTIL

//////////////////////////////////////////////////////////
//  void short_to_string( char* res, unsigned short s ) //
//  convert a short in a string                         //
////////////////////////////////////////////////////////// 
void short_to_string( char* res, unsigned short s );

////////////////////////////////////////////////////////////
// unsigned short is_Number(char * s, unsigned short end) //
// return 1 if the string represent a decimal number      //
////////////////////////////////////////////////////////////
unsigned short is_Number(char * s, unsigned short end);

///////////////////////////////////////////////////////////
// unsigned long string_dec_to_int(char *s)              //
// transform a decimal number into a ascii string        //
/////////////////////////////////////////////////////////// 
unsigned long string_dec_to_int(char *s);


unsigned long string_hex_to_int(char *s);
unsigned short string_33_to_int(char c);
#endif
