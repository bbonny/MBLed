/*
 * manager.h
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
#ifndef H_MANAGER
#define H_MANAGER

#define MANAGER_DEMO0  0
#define MANAGER_DEMO1 1
#define MANAGER_DEMO2 2
#define MANAGER_DEMO3 3
#define MANAGER_DEMO4 4
#define MANAGER_DEMO5 5
#define MANAGER_DEMO6 6
#define MANAGER_DEMO7 7
#define MANAGER_DEMO8 8
#define MANAGER_DEMO9 9
#define MANAGER_DEMO10 10
#define MANAGER_DEMO11 11
#define MANAGER_DEMO12 12
#define MANAGER_DEMO13 13


#define MANAGER_TASKS 20

#define MANAGER_COMMAND_GIVE_TASK 0
#define MANAGER_COMMAND_ASK_TASK  1


void manager_init();
void manager_task();
void switching_task();
void launcher_task();
void manager_receive_command(Packet *packet);

#endif
