
    while(continua){
		if(!IniciaMenu(win, ren)){
			continua = false;
		}else{
			SDL_SetRenderDrawColor(ren, 100,100,100,0x00);//gcc main.c -lSDL2 -lSDL2_image -lSDL2_ttf -o main
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

void mudaCor(SDL_Renderer* ren,SDL_Surface* listaS[],SDL_Texture* listaT[],SDL_Color cor,int i,char nome[],TTF_Font *ourFont){
	listaS[i] = TTF_RenderText_Solid(ourFont, nome,cor);  
	listaT[i] = SDL_CreateTextureFromSurface(ren,listaS[i]);
}

bool IniciaMenu(SDL_Window* win, SDL_Renderer* ren){
	//SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    TTF_Init();
    SDL_Color padrao = { 255,231,55,255 };
    SDL_Color focus = { 237,139,0,255 };

    TTF_Font *ourFont = TTF_OpenFont("Pacmania.ttf",100);
    
    struct SDL_Surface* listaSurfaceText[3];
    listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",padrao);  
    listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "Quit",padrao); 
    listaSurfaceText[2] = TTF_RenderText_Solid(ourFont, "PACMAN",padrao); 
 
    struct SDL_Texture* listaTextureText[3];
	listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
    listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
	listaTextureText[2] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[2]);

	SDL_Surface* bertoneSur = TTF_RenderText_Solid(ourFont, "bertone",padrao);
    SDL_Texture* bertoneTex = SDL_CreateTextureFromSurface(ren,bertoneSur);
    int i;
    SDL_Point mouse = {0,0};
    SDL_Rect recPlay = {200,120,100,30};
    SDL_Rect recQuit = {200,200,100,30};
	SDL_Rect recTitle = {110, 50, 300,50};
    bool selecionado = false;
    bool gameIsRunning = true;


	while (gameIsRunning) {
    	SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);
		SDL_Event event;
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					gameIsRunning = false;
				break;
				case SDL_MOUSEMOTION:
						SDL_GetMouseState(&mouse.x,&mouse.y);
						if(SDL_PointInRect(&mouse,&recPlay)){
							//listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",focus);  
					   		//listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,0,"Play",ourFont);
					   	}
					   	else if(SDL_PointInRect(&mouse,&recQuit)){
					   		//listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "Quit",focus);  
					   		//listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,focus,1,"Quit",ourFont);
					   	}
					   	else{
							/*listaSurfaceText[0] = TTF_RenderText_Solid(ourFont, "Play",padrao);  
							listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
							listaSurfaceText[1] = TTF_RenderText_Solid(ourFont, "Quit",padrao);  
							listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);*/
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,0,"Play",ourFont);
					   		mudaCor(ren,listaSurfaceText,listaTextureText,padrao,1,"Quit",ourFont);
							selecionado = false;
					   	}
				break;
				case SDL_MOUSEBUTTONDOWN:	
					if(event.button.button==SDL_BUTTON_LEFT){
						if(SDL_PointInRect(&mouse,&recQuit)) selecionado = true;
					}
					if(event.button.button==SDL_BUTTON_LEFT){
						if(SDL_PointInRect(&mouse,&recPlay)) selecionado = true;
					}
				case SDL_MOUSEBUTTONUP:	
					if(event.button.button==SDL_BUTTON_LEFT){
						if(event.button.state==SDL_RELEASED){
							if(SDL_PointInRect(&mouse,&recQuit) && selecionado) {
								gameIsRunning = false;
								return false;
							}
							if(SDL_PointInRect(&mouse,&recPlay) && selecionado) {
								gameIsRunning = false;
								return true;
							}
						}
					}
				
				break;
			}
		}
		
		SDL_RenderCopy(ren,listaTextureText[0],NULL,&recPlay);
		SDL_RenderCopy(ren,listaTextureText[1],NULL,&recQuit);
		SDL_RenderCopy(ren,listaTextureText[2],NULL,&recTitle);
		SDL_RenderPresent(ren);
	}	

	for(i = 0; i < 3;i++){
		SDL_FreeSurface(listaSurfaceText[i]);
		SDL_DestroyTexture(listaTextureText[i]);
	}
	mouse.x = mouse.y = 0;
	TTF_CloseFont(ourFont);
	//SDL_DestroyRenderer(ren);
	//SDL_DestroyWindow(win);  
}

void IniciaJogo(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* sprite, SDL_Texture* maze){
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
						
						case SDLK_ESCAPE:
							IniciaMenu(win, ren);
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
			
			SDL_RenderPresent(ren);
			espera = 150;
		}
    }
    SDL_DestroyTexture(maze);
}




int main (int argc, char* args[])
{
     /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("PAC-MAN",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         500, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	SDL_Texture* maze = IMG_LoadTexture(ren, "labirinto.png");
	SDL_Texture* sprite = IMG_LoadTexture(ren, "pac-man-sprite.png");
	assert(maze != NULL);
	assert(sprite != NULL);

	bool continua = true;
	/* EXECUÇÃO */
    SDL_SetRenderDrawColor(ren, 0,0,0,0x00);
    SDL_RenderClear(ren);
	
    while(continua){
		if(IniciaMenu(win, ren)){
			IniciaJogo(win, ren, sprite, maze);
		}else{
			continua = false;
		}
		
	}
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

			SDL_Rect r1 = {50,50,50,50};
			SDL_RenderFillRect(ren, &r1);
			SDL_Delay(1000);
		}
		
	}
    /* FINALIZACAO */
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
