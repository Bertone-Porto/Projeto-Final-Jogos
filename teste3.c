#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>

enum Estado_Ghost{PARA_BAIXO, PARA_CIMA, PARA_DIREITA, PARA_ESQUERDA};
struct Coin{
	SDL_Rect r;
	SDL_Rect c;
	struct Coin* prox;
	struct Coin* ant;
};


int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms){
	Uint32 antes = SDL_GetTicks();
	int isevt = SDL_WaitEventTimeout(evt, *ms);
	if(isevt){
		Uint32 tempo = (SDL_GetTicks() - antes);
	if(*ms < tempo)
		tempo = *ms;
		*ms -= tempo;
	}
	else
		*ms = 500;
	return isevt;
}

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(0);
    SDL_Window* win = SDL_CreateWindow("Animação",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* sprite = IMG_LoadTexture(ren, "pac-man-sprite.png");
    assert(sprite != NULL);

    /* EXECUÇÃO */
    bool continua = true;
    SDL_Event evt;
    int isup = 1;
    int x = 50, y=130;
	int xR, yR;
	int xP, yP;
	int frame_red = 0, frame_pink = 0;
    int espera = 50;
    int yC=-10, wC=10, hC=10;
    int i;

    while (continua) {
        SDL_SetRenderDrawColor(ren, 0,0,0,0);
        SDL_RenderClear(ren);
		//SDL_Rect * coins = (SDL_Rect *) malloc(sizeof(SDL_Rect)*20);


		//MOEDAS
		struct Coin coins;
		
		SDL_Rect c_coin = (SDL_Rect) { 5,185, 10,10 };
		
		struct Coin * moedas = (struct Coin *) malloc(sizeof(struct Coin)*15);
		for(i=0;i<15;i++){
			moedas[i].r = (SDL_Rect) {(30)*i, 30, 10, 10};
			moedas[i].c = (SDL_Rect) { 5,185, 10,10 };
		}

      	Uint32 antes = SDL_GetTicks();
        int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);       
        if(isevt){       	
		switch (evt.type ) {
		    case SDL_QUIT:
		    	continua = false;
		    	break;		
		}
		//nada
	}
	else{   		
		//c_coin = (SDL_Rect) { 5,185, 10,10 };	
		for(i=3;i<15;i++){
			SDL_RenderCopy(ren, sprite, &moedas[i].c, &moedas[i].r);
		}
		

				
		//for (i=0; i<8; i++)
		//	SDL_RenderFillRect(ren, &walls[i]);
		//SDL_RenderCopy(ren, sprite, &pink.c, &pink.r);
		SDL_RenderPresent(ren);
		espera = 50;

		
		}
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
