#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
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
                         200, 200, SDL_WINDOW_SHOWN
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
	int pos = 0;
    int espera = 50;
    int yC=-10, wC=20, hC=20;
    while (continua) {
        SDL_SetRenderDrawColor(ren, 0,0,0,0);
        SDL_RenderClear(ren);
        SDL_Rect r_red_ghost = { x,y, 30,30 };
        SDL_Rect c_red_ghost;

		SDL_Rect r_pink_ghost = { 50,50, 30,30 };
        SDL_Rect c_pink_ghost;


      	Uint32 antes = SDL_GetTicks();
        int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);       
        if(isevt){       	
		switch (evt.type ) {
		    case SDL_QUIT:
		    	continua = false;
		    	break;		
		}
		//
	
	}
	
	else{   
		switch(isup){

			//vermelho_cima
			case 1:
				y -= 5;
				pos += 1;
				if(pos == 1){
					xR = 4; yR = 85;
				}else{
					xR = 24; yR = 85;
					pos=0;
				}
				c_red_ghost = (SDL_Rect) { xR, yR, wC,hC };
			break;
			
			case 2:
				y += 5;
				pos += 1;
				if(pos == 1){
					xR = 44; yR = 85;
				}else{
					xR = 64; yR = 85;
					pos=0;
				}
				c_red_ghost = (SDL_Rect) { xR, yR, wC,hC };
			break;

			/*case 1:
				c_red_ghost = (SDL_Rect) { 44,85, 20,20 };
			break;
			case 2:
				c_red_ghost = (SDL_Rect) { 64,85, wC,hC };
			break;*/ // vermelho_baixo

			/*case 1:
				c_red_ghost = (SDL_Rect) { 84,85, 20,20 };
			break;
			case 2:
				c_red_ghost = (SDL_Rect) { 104,85, wC,hC };
			break;*/ // vermelho_esquerda

			/*case 1:
				c_red_ghost = (SDL_Rect) { 124,85, 20,20 };
			break;
			case 2:
				c_red_ghost = (SDL_Rect) { 144,85, wC,hC };
			break;*/ // vermelho_direita


			


			/*case 1:
				c_pink_ghost = (SDL_Rect) { 4,105, wC,hC };
			break;
			case 2:
				c_pink_ghost = (SDL_Rect) { 24,105, wC,hC };
			break;*/ //rosa_cima
			
			/*case 1:
				c_pink_ghost = (SDL_Rect) { 44,105, wC,hC };
			break;
			case 2:
				c_pink_ghost = (SDL_Rect) { 64,105, wC,hC };
			break;*/ //rosa_baixo

			/*case 1:
				c_pink_ghost = (SDL_Rect) { 84,105, wC,hC };
			break;
			case 2:
				c_pink_ghost = (SDL_Rect) { 104,105, wC,hC };
			break;*/ //rosa_esquerda

			/*case 1:
				c_pink_ghost = (SDL_Rect) { 124,105, wC,hC };
			break;
			case 2:
				c_pink_ghost = (SDL_Rect) { 144,105, wC,hC };
			break;*/ //rosa_direita


		}
		isup++;
	
		SDL_RenderCopy(ren, sprite, &c_red_ghost, &r_red_ghost);
		SDL_RenderCopy(ren, sprite, &c_pink_ghost, &r_pink_ghost);
		SDL_RenderPresent(ren);
		espera = 220;
		if(y <= 160){
			isup = 1;
		}else if(y >= 50){
			isup = 2;
		}

		
		
	}
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(sprite);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
