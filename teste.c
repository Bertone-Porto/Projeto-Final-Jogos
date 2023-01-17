/*gcc teste.c -lSDL2 -lSDL2_gfx -lSDL2_image -o teste*/
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

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
    SDL_Texture* maze = IMG_LoadTexture(ren, "labirinto.png");
	SDL_Texture* sprite = IMG_LoadTexture(ren, "pac-man-sprite.png");
    
    assert(maze != NULL);
	assert(sprite != NULL);
	
	int mazeVet[10] = [00000000000,
			  	       01111111110,
			  	       01000100010,
			 	       01011111010,
			           01110101110,
				       01010101010,
				       01110101110,
				       01011111010,
				       01110101110,
				       00000000000];

    /* EXECUÇÃO */
    bool continua = true;
    SDL_Event evt;
    int isup = 1, movimento;
    int x =0, y=20;
    int espera = 50;
    int yC=-10, wC=40, hC=80;
    int i=0, boca=0;
    bool para_cima=false, para_baixo=false, para_esquerda = false, para_direita = true;
    bool ready = false;
    
    
    SDL_Rect r_personagem = {100, 150, 40, 40};
    SDL_Rect c_personagem;
    
    SDL_Rect r_maze = {-40,50, 618,404 };
    SDL_Rect c_maze;
    
    //roundedBoxRGBA(ren, 200, 200, 800, 800, 20, 250,250,250,0);

    
    while (continua) {
    	
		SDL_SetRenderDrawColor(ren, 0,0,0,0);
   		SDL_RenderClear(ren);
        
        switch (evt.type ) {
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
				            if(r_personagem.y-5 > 63){
								r_personagem.y -= 5;
								para_cima = true;
								movimento = 1;
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
							}
							break;
								   
						case SDLK_DOWN:
							if(r_personagem.y < 315){
								r_personagem.y += 5;
								para_baixo = true;
								movimento = 2;
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
							}

							break;
							
						case SDLK_LEFT:
							if(r_personagem.x > 68 ){
								r_personagem.x -= 5;
								para_esquerda = true;
								movimento = 3;
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
							}
							break;
								   
						case SDLK_RIGHT:
							if(r_personagem.x < 400) {
								r_personagem.x += 5;
								para_direita = true;
								movimento = 4;
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
							}
							break;
					}
				}	
			
	
		} else{   
			
			switch(movimento){
				case 1: //para cima
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					break;
				case 2: //para baixo
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					break;
				case 3: //para esquerda
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					break;
				case 4: //para direita
					c_personagem = (SDL_Rect) {x, y, 20, 20}; 
					break;
			}
				
			if(!ready){
				c_personagem = (SDL_Rect) {0, 0, 20, 20}; 
			}
			SDL_RenderCopy(ren, sprite, &c_personagem, &r_personagem); //player
			
			c_maze = (SDL_Rect) {   0, 0, 600,500 }; //labirinto
			SDL_RenderCopy(ren, maze, &c_maze, &r_maze);
			
			/*SDL_SetRenderDrawColor(ren,250,250,250,0);
			SDL_RenderDrawLine(ren,50,75,450,75); //limite topo
			SDL_RenderDrawLine(ren,68,60,68,400); //limite lado esquerdo
			SDL_RenderDrawLine(ren,441,60,441,400); //limite lado direito
			SDL_RenderDrawLine(ren,50,360,450,360); //limite parte de baixo/*
			
			/*SDL_SetRenderDrawColor(ren,250,250,250,10);//branco
			SDL_Rect cubos = {165,170, 20,20};
			SDL_RenderFillRect(ren, &cubos);
			
			SDL_SetRenderDrawColor(ren,250,0,0,10);//vermelho
			SDL_Rect cubos2 = {215,170, 20, 20};
			SDL_RenderFillRect(ren, &cubos2);
			
			SDL_SetRenderDrawColor(ren,0,250,0,10);//verde
			SDL_Rect cubos3 = {165,200, 20,20};
			SDL_RenderFillRect(ren, &cubos3);
			
			SDL_SetRenderDrawColor(ren,100,100,100,10);//cinza
			SDL_Rect cubos4 = {215,200, 20, 20};
			SDL_RenderFillRect(ren, &cubos4);*/
			
			
			
			
			SDL_RenderPresent(ren);

			espera = 150;
			
		}
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(maze);

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
