#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
enum Estado_Ghost{PARA_BAIXO, PARA_CIMA, PARA_DIREITA, PARA_ESQUERDA};
struct Coin{
	SDL_Rect r;
	SDL_Rect c;
	struct Coin* prox;
	struct Coin* ant;
};

struct Fruta{
	SDL_Rect r;
	SDL_Rect c;
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
    int isup = 0;
    int x = 50, y=130;
	int xR, yR;
	int xP, yP;
	int frame_red = 0, frame_pink = 0;
    int espera = 50;
    int yC=-10, wC=10, hC=10;
    int i;
	//frutas
	struct Fruta * frutas = (struct Fruta *) malloc(sizeof(struct Fruta)*4);
	frutas[0].c = (SDL_Rect) {165,158,25,25}; frutas[0].r = (SDL_Rect) {random()%200,random()%100,38,38};
	frutas[1].c = (SDL_Rect) {165,180,25,25}; frutas[1].r = (SDL_Rect) {random()%300,random()%250,38,38};
	frutas[2].c = (SDL_Rect) {165,200,25,25}; frutas[2].r = (SDL_Rect) {random()%400,random()%300,38,38};
	frutas[3].c = (SDL_Rect) {165,220,25,25}; frutas[3].r = (SDL_Rect) {random()%100,random()%300,38,38};

    while (continua) {
        SDL_SetRenderDrawColor(ren, 0,0,0,0);
        SDL_RenderClear(ren);
		
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
		int kk = random()%4;
		printf("valor: %d\n", kk);
		//SDL_RenderFillRect(ren, &walls[i]);
		for(i=0;i<4;i++){
			SDL_RenderCopy(ren, sprite, &frutas[i].c, &frutas[i].r);
		}

		SDL_RenderPresent(ren);
		espera = 50;

		isup++;
		}
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
