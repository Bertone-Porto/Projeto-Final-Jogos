
//gcc main.c -lSDL2 -lSDL2_image -lSDL2_ttf -o main
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum Personagem{PARADO, MOVER_ESQUERDA, MOVER_DIREITA, MOVER_CIMA, MOVER_BAIXO};
enum Vida {VIVO, MORTO};
enum Estado_Ghost{COLIDIDO, PARA_BAIXO, PARA_CIMA, PARA_DIREITA, PARA_ESQUERDA};

struct dadosPlayer{
	SDL_Rect r;
	SDL_Rect c;
	int moveState;
	int lifeState;
};

struct dadosGhost{
	SDL_Rect r;
	SDL_Rect c;
	int state;
	int lifeState;
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

bool Tem_Contato (SDL_Rect * r1, SDL_Rect * col) {
    int i; SDL_Rect temp;
	temp.x = r1->x-3;
	temp.y = r1->y-3;
	temp.w = r1->w+3;
	temp.h = r1->h+3;
	for (i = 0; i < 8; i++) {
		if (SDL_HasIntersection(&temp, &col[i])) return 1;
	} return 0; 
}

bool Contato_Ghost (SDL_Rect * r1, SDL_Rect * r2){
	int i; SDL_Rect temp;

	if(SDL_HasIntersection(r1, r2)){
		return true;
	} return false;

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
	/* EXECUÇÃO */
    bool continua = true;
	SDL_Event evt;
	int isup = 1, movimento;
	int x =0, y=20;
	int espera = 50;
	int i=0, boca=0, j;
	int posicao_x = 1;
	int posicao_y = 1;
	struct dadosPlayer p;
	p.r = (SDL_Rect) {60,63,30,30};
	p.moveState = PARADO;
	p.lifeState = VIVO;
   
	
	int xR, yR;
	int xP, yP;
	int yC=-10, wC=20, hC=20;
	int frame_red = 0, frame_pink = 0;

	bool alcancou_topo = false, alcancou_fundo=false;
	bool started = false;
	int posX_red=100, posY_red=100; //posição red ghost
	int posX_pink=400, posY_pink=350; //posição pink ghost		
    
    while (continua) {
    	
		SDL_SetRenderDrawColor(ren, 0,0,0,0);
	   	SDL_RenderClear(ren);
	   		
	   	struct dadosGhost red;
	   	red.r = (SDL_Rect) {posX_red,posY_red, 30,30};
	   	red.lifeState = VIVO;
	   		
		struct dadosGhost pink;
		pink.r = (SDL_Rect) { posX_pink,posY_pink, 30,30 };
		pink.lifeState = VIVO;
			
		if(!started){
	   		red.state = PARA_CIMA;
	   		pink.state = PARA_CIMA;
	   	}


		SDL_SetRenderDrawColor(ren, 0,0,250,0);
		SDL_Rect * walls = (SDL_Rect *) malloc(sizeof(SDL_Rect)*8);
		walls[0] = (SDL_Rect) {0, 0, 30,80};
		walls[1] = (SDL_Rect) {470, 0, 30,80};	
		walls[2] = (SDL_Rect) {0, 420, 30,80};
		walls[3] = (SDL_Rect) {470, 420, 30,80};
		walls[4] = (SDL_Rect) {220, 210, 30,100};
		walls[5] = (SDL_Rect) {180, 240, 100,30};
		walls[6] = (SDL_Rect) {160, 50, 150, 30};
		walls[7] = (SDL_Rect) {160, 420, 150, 30};
		
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
						       	p.moveState = MOVER_CIMA;
								break;
									   
							case SDLK_DOWN:
								p.moveState = MOVER_BAIXO;
								break;
								
							case SDLK_LEFT:
								p.moveState = MOVER_ESQUERDA;
								break;
									   
							case SDLK_RIGHT:
								p.moveState = MOVER_DIREITA;
								break;
						}
					}	
			
	
		} else{   
			switch(p.moveState){
				case PARADO:
					p.c = (SDL_Rect) {0, 0, 20, 20}; 
					break;
				case MOVER_CIMA: //para cima
					if(!Tem_Contato(&p.r, walls) && p.lifeState == VIVO){
						p.c = (SDL_Rect) {x, y, 20, 20}; 
						p.r.y -= 7;
						boca++;
						if(boca % 2 == 0){
							x=0; y=40;
						} else{
							x=40; y=0;
						}
						if(boca > 11){
							boca=0;
						}
						if(Contato_Ghost(&p.r, &red.r) || Contato_Ghost(&p.r, &pink.r)){
							p.lifeState = MORTO;
							red.state = COLIDIDO;
							pink.state = COLIDIDO;
						}
						}else{
							p.r.y +=5;
							p.moveState = MOVER_BAIXO;
						}
					break;
				case MOVER_BAIXO: //para baixo
					if(!Tem_Contato(&p.r, walls) && p.lifeState == VIVO){
						p.c = (SDL_Rect) {x, y, 20, 20}; 
						p.r.y += 7;
						boca++;
						if(boca % 2 == 0){
							x=0; y=60;
						} else{
							x=40; y=0;
						}
						if(boca > 11){
							boca=0;
						}
						if(Contato_Ghost(&p.r, &red.r) || Contato_Ghost(&p.r, &pink.r)){
							p.lifeState = MORTO;
							red.state = COLIDIDO;
							pink.state = COLIDIDO;
						}
						}else{
							p.r.y -=5;
							p.moveState = MOVER_CIMA;
						}
					break;
				case MOVER_ESQUERDA: //para esquerda
					if(!Tem_Contato(&p.r, walls) && p.lifeState == VIVO){
						p.c = (SDL_Rect) {x, y, 20, 20}; 
						p.r.x -= 7;
						boca++;
						if(boca % 2 == 0){
							x=0; y=0;
						} else{
							x=40; y=0;
						}
						if(boca > 11){
							boca=0;
						}
						if(Contato_Ghost(&p.r, &red.r) || Contato_Ghost(&p.r, &pink.r)){
							p.lifeState = MORTO;
							red.state = COLIDIDO;
							pink.state = COLIDIDO;
						}
					}else{
						p.r.x +=5;
						p.moveState = MOVER_DIREITA;
					}
					break;
				case MOVER_DIREITA: //para direita
					if(!Tem_Contato(&p.r, walls) && p.lifeState == VIVO){
						p.c = (SDL_Rect) {x, y, 20, 20}; 
						p.r.x += 7;
						boca++;
						if(boca % 2 == 0){
							x=0; y=20;
						} else{
							x=40; y=0;
						}
						if(boca > 11){
							boca=0;
						}
						if(Contato_Ghost(&p.r, &red.r) || Contato_Ghost(&p.r, &pink.r)){
							p.lifeState = MORTO;
							red.state = COLIDIDO;
							pink.state = COLIDIDO;
						}
					}else{
						p.r.x -= 5;
						p.moveState = MOVER_ESQUERDA;
					}
					break;
				}
				
				//ANIMAÇÃO MORTE PLAYER
				if(p.lifeState == MORTO){
					switch(isup){
						case 1:
							p.c = (SDL_Rect) { 3, 245, 20,20 };
							break;
						case 2:
							p.c = (SDL_Rect) { 23, 245, 20,20 };
							break;
						case 3:
							p.c = (SDL_Rect) { 43, 245, 20,20 };
							break;
						case 4:
							p.c = (SDL_Rect) { 63, 245, 20,20 };
							break;
						case 5:
							p.c = (SDL_Rect) { 83, 245, 20,20 };
							break;
						case 6:
							p.c = (SDL_Rect) { 103, 245, 20,20 };
							break;
						case 7:
							p.c = (SDL_Rect) { 123, 245, 20,20 };
							break;
						case 8:
							p.c = (SDL_Rect) { 143, 245, 20,20 };
							break;
						case 9:
							p.c = (SDL_Rect) { 163, 245, 20,20 };
							break;
						case 10:
							p.c = (SDL_Rect) { 183, 245, 20,20 };
							break;
						case 11:
							p.c = (SDL_Rect) { 203, 245, 20,20 };
							break;
					}
					isup++;
					if(isup > 11){
						p.lifeState = VIVO;
						isup = 1;
						p.r.x = 60;
						p.r.y = 63;
						red.state = PARA_CIMA;
						pink.state = PARA_BAIXO;
					}
				}
			
				//ESTADOS RED GHOST
				switch(red.state){
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
						red.c = (SDL_Rect) { xR, yR, wC,hC };
					break;
				
					//vemelho_baixo
					case PARA_BAIXO:
						posY_red += 5;
						frame_red += 1;
						if(frame_red == 1){
							xR = 44; yR = 85;
						}else{
							xR = 64; yR = 85;
							frame_red=0;
						}
						red.c = (SDL_Rect) { xR, yR, wC,hC };
					break;
					}
			
				//ESTADOS PINK GHOST
				switch(pink.state){
					//rosa_cima
					case PARA_CIMA: 
						frame_pink += 1;
						posY_pink -= 5;
						if(frame_pink == 1){
							xP = 4; yP = 105;
						}else{
							xP = 24; yP = 105;
							frame_pink =0;
						}
						pink.c = (SDL_Rect) { xP,yP, wC,hC };
					break;
				
					//rosa_baixo
					case PARA_BAIXO:
						posY_pink += 5;
						frame_pink += 1;
						if(frame_pink == 1){
							xP = 44; yP = 105;
						}else{
							xP = 64; yP = 105;
							frame_pink=0;
						}
						pink.c = (SDL_Rect) { xP,yP, wC,hC };
					break;
			}

				//MOVIMENTAÇÃO VERTICAL RED GHOST
				if(posY_red <= 365 && posY_red >= 50 && !alcancou_topo && red.state != COLIDIDO){
					red.state = PARA_CIMA;
					if(posY_red == 50)
						alcancou_topo = true;
				}else if(alcancou_topo){
					red.state = PARA_BAIXO;
					if(posY_red >= 350){
						alcancou_topo = false;
						//printf("alcançou fundo\n");
						}
				}
				
				//MOVIMENTAÇÃO VERTICAL PINK GHOST
				if(posY_pink >= 40 && posY_pink <= 350 && !alcancou_fundo && pink.state != COLIDIDO){
					pink.state = PARA_BAIXO;
					if(posY_pink == 350){
						alcancou_fundo = true;
						printf("alcançou fundo\n");
					}
				}else if(alcancou_fundo){
					pink.state = PARA_CIMA;
					if(posY_pink <= 50){
						alcancou_fundo = false;
						//printf("alcançou topo\n");
					}
				}
			
			SDL_RenderCopy(ren, sprite, &red.c, &red.r);
			SDL_RenderCopy(ren, sprite, &pink.c, &pink.r);
			SDL_RenderPresent(ren);	
				
			SDL_RenderCopy(ren, sprite, &p.c, &p.r); //player
					
			for (i=0; i<8; i++)
				SDL_RenderFillRect(ren, &walls[i]);
			SDL_RenderPresent(ren);
			started=true;
			espera = 150;
			
		}
    }

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
