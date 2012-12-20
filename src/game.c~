/*game part*/
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char ** ParseCmd(const char * cmd, int * argc)
{
	char buf[50];
	int i = 0;
	int j;
	char c;
	char * word;
	char ** argv = (char**)malloc(sizeof(char *));
	*argc = 0;
	while(*cmd != '\0') {
		c = *cmd;
		if(c!=' ') {
			if(i<49) {
				buf[i] = c;
				i++;
			}
		} else {
			if (i>0) {
				word = (char *) malloc(sizeof(char)*(i+1));
				for(j = 0; j<i; j++)
					word[j] = buf[j];
				word[i] = '\0';	
				argv[*argc] = word;
				(*argc)++;
				argv = realloc(argv, *argc+1);
				i = 0;
			}
		}
		cmd++;
	}
	if (i>0) {
		word = (char *) malloc(sizeof(char)*(i+1));
		for(j = 0; j<i; j++)
			word[j] = buf[j];
		word[i] = '\0';	
		argv[*argc] = word;
		(*argc)++;
		argv = realloc(argv, *argc+1);
		i = 0;
	}
	argv[*argc] = NULL;
	return argv;
}

void SHelp(struct ClientNode * cl)
{
	SendMessage("Available commands:",cl,1);
	SendMessage("/help: this help",cl,1);
	SendMessage("/info: number of players and their nicknames",cl,1);
	SendMessage("/start: starts a game",cl,1);
	SendMessage("/name <new_name> : changes name",cl,1);
	SendMessage("help: ingame help",cl,1);
}
void Start(int argc, char ** argv, struct ClientNode * cl)
{
	if(argc != 1) {
		SendMessage("Incorrect usage", cl, 1);
		return;
	}
	if(cl->game->state == 0) {
		Broadcast("Game started!", cl->game, 1);
		cl->game->state = 1;
	}
}
void Info(int argc, char ** argv, struct ClientNode * cl)
{
	struct Game * game = cl->game;
	struct ClientNode * list = game->cls;
	char buf[50];
	if(argc!=1) {
		SendMessage("Incorrect usage, no args needed", cl, 1);
		return;
	}
	SendMessage("Players: ", cl, 0);
	while(list) {
		SendMessage(list->name, cl, 0);
		SendMessage(" ", cl, 0);
		list = list->next;
	}
	SendMessage("\nPlayers count: ", cl, 0);
	itoa(game->playersCount, buf);
	SendMessage(buf, cl, 1);
	

}


void Name(int argc, char ** argv, struct ClientNode * cl)
{
	char found = 0;
	struct ClientNode * list = cl->game->cls;
	if (argc!=2) {
		SendMessage("Incorrect usage. Try /name <new_name>", cl, 1);
		return;
	}
	while(list) {
		found |= (!strcmp(list->name, argv[1]));
		list = list->next;
	}
	if(!found) {
		int i;
		for(i = 0; (argv[1])[i]!='\0'; i++)
			(cl->name)[i] = (argv[1])[i];
		(cl->name)[i] = '\0';
		SendMessage("Name: ", cl, 0);
		SendMessage(cl->name, cl, 1);
	} else {
		SendMessage("Player with this name already exists", cl, 1);
	}	
}


void Market(int argc, char ** argv, struct ClientNode * cl){}
void Player(int argc, char ** argv, struct ClientNode * cl){}
void Prod(int argc, char ** argv, struct ClientNode * cl){}
void Turn(int argc, char ** argv, struct ClientNode * cl){}
void Buy(int argc, char ** argv, struct ClientNode * cl){}
void Sell(int argc, char ** argv, struct ClientNode * cl){}
void Build(int argc, char ** argv, struct ClientNode * cl){}
void Help(struct ClientNode * cl)
{
	SendMessage("Available commands:",cl,1);
	SendMessage("market: market state",cl,1);
	SendMessage("player <name> : information about player",cl,1);
	SendMessage("prod <count> : to produce production",cl,1);
	SendMessage("build: to build a factory",cl,1);
	SendMessage("buy <count> <price> : to buy raw",cl,1);
	SendMessage("sell <count> <price> : to sell production",cl,1);
	SendMessage("turn : to finish your turn",cl,1);
}

