/*gcc teste.c -lSDL2 -lSDL2_gfx -lSDL2_image -o teste*/
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum Personagem{PARADO, MOVER_ESQUERDA, MOVER_DIREITA, MOVER_CIMA, MOVER_BAIXO};

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


int RectInRect (SDL_Rect* r1, SDL_Rect* r2) {
    return !( r1->x+r2->w < r1->x ||
              r1->y+r2->h < r1->y ||
              r1->x > r1->x+r1->w ||
              r1->y > r1->y+r1->h );
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
    int isup = 1, movimento;
    int x =0, y=20;
    int espera = 50;
    int yC=-10, wC=40, hC=80;
    int i=0, boca=0, j;
    bool ready = false;
    int posicao_x = 1;
	int posicao_y = 1;
	int estado_atual_p = PARADO;
    
    SDL_Rect r_personagem = {60, 63, 30, 30};
    SDL_Rect c_personagem;    
    
    while (continua) {
    	
		SDL_SetRenderDrawColor(ren, 0,0,0,0);
   		SDL_RenderClear(ren);

		SDL_SetRenderDrawColor(ren, 0,0,250,0);
		SDL_Rect obj1 = {40, 100, 30,80};
		SDL_Rect obj2 = {380, 100, 30,80};
		SDL_Rect obj3 = {380, 400, 30,80};
		SDL_Rect obj4 = {250, 250, 30,80};
		SDL_Rect obj5 = {380, 20, 30,80};
		SDL_Rect obj6 = {250, 250, 80,30};
		
		
        switch (evt.type) {
				case SDL_QUIT:
					continua = false;
					break;	
		}
        
      	Uint32 antes = SDL_GetTicks();
        int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);       
        if(isevt){    
        	espera -= (SDL_GetTicks() - antes);
            if (espera < 0) {
                espera = 0;
            }   	
		        if (evt.type == SDL_KEYDOWN) {
		        	switch (evt.key.keysym.sym) {
		                case SDLK_UP:	
				            if(estado_atual_p == PARADO || !SDL_HasIntersection(&r_personagem, &obj1)){
								estado_atual_p = MOVER_CIMA;						
							} else{
								estado_atual_p = PARADO;
							}
							break;
								   
						case SDLK_DOWN:
							if(estado_atual_p == PARADO || !SDL_HasIntersection(&r_personagem, &obj1)){
								estado_atual_p = MOVER_BAIXO;								
							} else{
								estado_atual_p = PARADO;
							}
							break;
							
						case SDLK_LEFT:
							if(estado_atual_p == PARADO || !SDL_HasIntersection(&r_personagem, &obj1) ){
								estado_atual_p = MOVER_ESQUERDA;					
							} else{
								estado_atual_p = PARADO;
							}
							break;
								   
						case SDLK_RIGHT:
							if(estado_atual_p == PARADO || !SDL_HasIntersection(&r_personagem, &obj1)) {
								estado_atual_p = MOVER_DIREITA;
							} else{
								estado_atual_p = PARADO;
							}
							break;
					}
				}	
			
	
		} else{   
			
			switch(estado_atual_p){
				case MOVER_CIMA: //para cima
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					r_personagem.y -= 5;
					ready = true;
					boca++;
					if(boca % 2 == 0){
						x=0; y=40;
					} else{
						x=40; y=0;
					}
					if(boca > 11){
						boca=0;
					}
					break;
				case MOVER_BAIXO: //para baixo
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					r_personagem.y += 5;
					ready = true;
					boca++;
					if(boca % 2 == 0){
						x=0; y=60;
					} else{
						x=40; y=0;
					}
					if(boca > 11){
						boca=0;
					}
					break;
				case MOVER_ESQUERDA: //para esquerda
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					r_personagem.x -= 5;
					ready = true;
					boca++;
					if(boca % 2 == 0){
						x=0; y=0;
					} else{
						x=40; y=0;
					}
					if(boca > 11){
						boca=0;
					}
					break;
				case MOVER_DIREITA: //para direita
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					r_personagem.x += 5;
					ready = true;
					boca++;
					if(boca % 2 == 0){
						x=0; y=20;
					} else{
						x=40; y=0;
					}
					if(boca > 11){
						boca=0;
					}
					break;
			}
				
			if(!ready){
				c_personagem = (SDL_Rect) {0, 0, 20, 20}; 
			}
			SDL_RenderCopy(ren, sprite, &c_personagem, &r_personagem); //player
					
			SDL_RenderFillRect(ren, &obj1);
			SDL_RenderFillRect(ren, &obj2);
			SDL_RenderFillRect(ren, &obj3);
			SDL_RenderFillRect(ren, &obj4);
			SDL_RenderFillRect(ren, &obj5);
			SDL_RenderFillRect(ren, &obj6);
			
			SDL_RenderPresent(ren);
			
			espera = 150;
			
		}
    }

    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
