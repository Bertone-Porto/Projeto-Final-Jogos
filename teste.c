#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum Estado_Ghost{PARA_BAIXO, PARA_CIMA, PARA_DIREITA, PARA_ESQUERDA};


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
	int frame_red = 0, frame_pink = 0;
    int espera = 50;
    int yC=-10, wC=20, hC=20;
    int estado_atual_redGhost, estado_atual_pinkGhost;
    bool alcancou_topo = false, alcancou_fundo=false;
    int posX_red=20, posY_red=160;
    int posX_pink=120, posY_pink=50;
    while (continua) {
        SDL_SetRenderDrawColor(ren, 0,0,0,0);
        SDL_RenderClear(ren);
        SDL_Rect r_red_ghost = { posX_red,posY_red, 30,30 };
        SDL_Rect c_red_ghost;

		SDL_Rect r_pink_ghost = { posX_pink,posY_pink, 30,30 };
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
		switch(estado_atual_redGhost){

			//vermelho_cima
			case PARA_CIMA:

				posY_red -= 5;
				frame_red += 1;
				if(frame_red == 1){
					xR = 4; yR = 85;
				}else{
					xR = 24; yR = 85;
					frame_red=0;
				}
				c_red_ghost = (SDL_Rect) { xR, yR, wC,hC };
				

			break;
			
			case PARA_BAIXO:

				posY_red += 5;
				frame_red += 1;
				if(frame_red == 1){
					xR = 44; yR = 85;
				}else{
					xR = 64; yR = 85;
					frame_red=0;
				}
				c_red_ghost = (SDL_Rect) { xR, yR, wC,hC };

			break;


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
		switch(estado_atual_pinkGhost){
			case PARA_CIMA:

				frame_pink += 1;
				posY_pink -= 5;
				if(frame_pink == 1){
					xP = 4; yP = 105;
				}else{
					xP = 24; yP = 105;
					frame_pink =0;
				}
				c_pink_ghost = (SDL_Rect) { xP,yP, wC,hC };
			break;
			
			case PARA_BAIXO:

				posY_pink += 5;
				frame_pink += 1;
				if(frame_pink == 1){
					xP = 44; yP = 105;
				}else{
					xP = 64; yP = 105;
					frame_pink=0;
				}
				c_pink_ghost = (SDL_Rect) { xP,yP, wC,hC };
			break;
		}

		//MOVIMENTAÇÃO VERTICAL RED GHOST
		if(posY_red <= 165 && posY_red >= 50 && !alcancou_topo){
			estado_atual_redGhost = PARA_CIMA;
			if(posY_red == 50)
				alcancou_topo = true;
		}else if(alcancou_topo){
			estado_atual_redGhost = PARA_BAIXO;
			if(posY_red >= 160)
				alcancou_topo = false;
		}
		
		//MOVIMENTAÇÃO VERTICAL PINK GHOST
		if(posY_pink >= 40 && posY_pink <= 165 && !alcancou_fundo){
			estado_atual_pinkGhost = PARA_BAIXO;
			if(posY_pink == 165){
				alcancou_fundo = true;
				printf("alcançou fundo\n");
			}
		}else if(alcancou_fundo){
			estado_atual_pinkGhost = PARA_CIMA;
			if(posY_pink <= 50){
				alcancou_fundo = false;
				printf("alcançou topo\n");
			}
		}

	
	
	
		SDL_RenderCopy(ren, sprite, &c_red_ghost, &r_red_ghost);
		SDL_RenderCopy(ren, sprite, &c_pink_ghost, &r_pink_ghost);
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
