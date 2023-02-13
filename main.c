
//gcc main.c -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -o main
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum Player{PARADO=0, MOVENDO, MORRENDO};
enum Player_Power{COM_PODER=0, SEM_PODER};

enum Ghost{MOVING=0, COLIDIDO};
enum Ghost_Vulnerability{VULNERAVEL=0, NAO_VULNERAVEL, MORTO};

enum Direction{MOVER_ESQUERDA=0, MOVER_DIREITA, MOVER_CIMA, MOVER_BAIXO};
enum Vida {ALIVE=0, DEAD};

struct dadosPlayer{
	SDL_Rect r;
	SDL_Rect c;
	int moveState;
	int lifeState;
	int state;
	int powerState;
	int vidas;
};

struct dadosGhost{
	SDL_Rect r;
	SDL_Rect c;
	int state;
	int lifeState;
	int vulnerabilityState;
	int directionState;
};

struct Coin{
	SDL_Rect r;
	SDL_Rect c;
	bool comido;
	bool contado;
};

struct Fruta{
	SDL_Rect r;
	SDL_Rect c;
	bool aparece;
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

bool Contato_Wall (SDL_Rect * r1, SDL_Rect * col) {
    int i; SDL_Rect temp;
	temp.x = r1->x-3;
	temp.y = r1->y-3;
	temp.w = r1->w-5;
	temp.h = r1->h-5;
	for (i = 0; i < 8; i++) {
		if (SDL_HasIntersection(&temp, &col[i])) return 1;
	} return 0; 
}

bool Tem_Contato (SDL_Rect * r1, SDL_Rect * r2){
	SDL_Rect temp, temp2;
	temp.x = r1->x-5;
	temp.y = r1->y-5;
	temp.w = r1->w-5;
	temp.h = r1->h-5;
	
	temp2.x = r2->x-5;
	temp2.y = r2->y-5;
	temp2.w = r2->w-5;
	temp2.h = r2->h-5;

	if(SDL_HasIntersection(&temp, &temp2)){
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
    listaSurfaceText[2] = TTF_RenderText_Solid(ourFont, "'PACMAN'",padrao); 
 
    struct SDL_Texture* listaTextureText[3];
	listaTextureText[0] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[0]);
    listaTextureText[1] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[1]);
	listaTextureText[2] = SDL_CreateTextureFromSurface(ren,listaSurfaceText[2]);

	//SDL_Surface* bertoneSur = TTF_RenderText_Solid(ourFont, "bertone",padrao);
    //SDL_Texture* bertoneTex = SDL_CreateTextureFromSurface(ren,bertoneSur);
    int i;
    SDL_Point mouse = {0,0};
    SDL_Rect recPlay = {250,120,100,30};
    SDL_Rect recQuit = {250,200,100,30};
	SDL_Rect recTitle = {160, 50, 300,50};
    bool selecionado = false;
    bool gameIsRunning = true;


	while (gameIsRunning) {
    	SDL_SetRenderDrawColor(ren,0,0,0,255);
		SDL_RenderClear(ren);
		SDL_Event event;
		int espera = 250;
		int isevt = AUX_WaitEventTimeoutCount(&event, &espera);
		if(isevt){
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
}

void IniciaJogo(SDL_Window* win, SDL_Renderer* ren, SDL_Texture* sprite, SDL_Texture* maze){
	/* EXECUÇÃO */
	bool continua = true;
	SDL_Event evt;
	int isup = 1, movimento;
	int x =0, y=20;
	int espera = 150;
	int i=0, boca=0, j;
	int posicao_x = 1;
	int posicao_y = 1;
	int conta_tempo1 = 0, conta_tempo2=0, conta_tempo3=0, fruta_aleatoria;

	struct dadosPlayer p;
	p.r = (SDL_Rect) {150,200,30,30};
	p.state = PARADO;
	p.lifeState = ALIVE;
	p.powerState = SEM_PODER;
	p.vidas = 3;

	int xR, yR;
	int xP, yP;
	int yC=-10, wC=20, hC=20;
	int frame_red = 0, frame_pink = 0;

	int posX_red=60, posY_red=100; //posição red ghost
	int posX_pink=400, posY_pink=350; //posição pink ghost
	int count = 0;
	int totalComidos = 0;
	
	//MOEDAS
	struct Coin * fileira_moedas1 = (struct Coin *) malloc(sizeof(struct Coin)*13);
	struct Coin * fileira_moedas2 = (struct Coin *) malloc(sizeof(struct Coin)*13);
	struct Coin * fileira_moedas3 = (struct Coin *) malloc(sizeof(struct Coin)*13);
	struct Coin * fileira_moedas4 = (struct Coin *) malloc(sizeof(struct Coin)*13);
	for(i=3;i<13;i++){
		//fileira_moedas1 parte de cima
		fileira_moedas1[i].r = (SDL_Rect) {(30)*i, 30, 10, 10};
		fileira_moedas1[i].c = (SDL_Rect) { 5,185, 10,10 };
		fileira_moedas1[i].contado = false;
		fileira_moedas1[i].comido = false;
		
		//fileira_moedas2 parte de cima
		fileira_moedas2[i].r = (SDL_Rect) {(30)*i, 430, 10, 10};
		fileira_moedas2[i].c = (SDL_Rect) { 5,185, 10,10 };
		fileira_moedas2[i].contado = false;
		fileira_moedas2[i].comido = false;
		
		//fileira_moedas3 parte de cima
		fileira_moedas3[i].r = (SDL_Rect) {45, 30*i, 10, 10};
		fileira_moedas3[i].c = (SDL_Rect) { 5,185, 10,10 };
		fileira_moedas3[i].contado = false;
		fileira_moedas3[i].comido = false;
		
		//fileira_moedas4 parte de cima
		fileira_moedas4[i].r = (SDL_Rect) {430, 30*i, 10, 10};
		fileira_moedas4[i].c = (SDL_Rect) { 5,185, 10,10 };
		fileira_moedas4[i].contado = false;
		fileira_moedas4[i].comido = false;	
	}
	
	//frutas
	struct Fruta * frutas = (struct Fruta *) malloc(sizeof(struct Fruta)*4);
	frutas[0].c = (SDL_Rect) {165,158,25,25}; frutas[0].r = (SDL_Rect) {150,100,38,38}; frutas[0].aparece = false;
	frutas[1].c = (SDL_Rect) {165,180,25,25}; frutas[1].r = (SDL_Rect) {300,250,38,38}; frutas[1].aparece = false;
	frutas[2].c = (SDL_Rect) {165,200,25,25}; frutas[2].r = (SDL_Rect) {450,300,38,38}; frutas[2].aparece = false;
	frutas[3].c = (SDL_Rect) {165,220,25,25}; frutas[3].r = (SDL_Rect) {100,300,38,38}; frutas[3].aparece = false;
	
	//FANTASMAS
	struct dadosGhost red;
	red.lifeState = ALIVE;
	red.vulnerabilityState = NAO_VULNERAVEL;	
	red.directionState = MOVER_CIMA;
	red.state = MOVING;
	
	struct dadosGhost pink;
	pink.lifeState = ALIVE;
	pink.vulnerabilityState = NAO_VULNERAVEL;
	pink.directionState = MOVER_BAIXO;
	pink.state = MOVING;
	
	
	while (continua) {
		SDL_SetRenderDrawColor(ren, 0,0,0,0);
   		SDL_RenderClear(ren);
   		
   		SDL_Rect vidas_r1 = {560,50,20,20};
   		SDL_Rect vidas_r2 = {540,50,20,20};
   		SDL_Rect vidas_r3 = {520,50,20,20};
		SDL_Rect vidas_c = { 84,162, 20, 20};
		
		red.r = (SDL_Rect) {posX_red,posY_red, 30,30};
		pink.r = (SDL_Rect) { posX_pink,posY_pink, 30,30 };

		SDL_SetRenderDrawColor(ren, 0,0,250,0);
		SDL_Rect * walls = (SDL_Rect *) malloc(sizeof(SDL_Rect)*8);
		walls[0] = (SDL_Rect) {0, 0, 30,80};
		walls[1] = (SDL_Rect) {470, 0, 30,80};
		walls[2] = (SDL_Rect) {0, 420, 30,80};
		walls[3] = (SDL_Rect) {470, 420, 30,80};
		walls[4] = (SDL_Rect) {220, 210, 30,100};
		walls[5] = (SDL_Rect) {180, 240, 100,30};
		walls[6] = (SDL_Rect) {160, 50, 150, 30};
		walls[7] = (SDL_Rect) {160, 450, 150, 30};
		
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
			    if (evt.type == SDL_KEYDOWN && (p.state != MORRENDO)) {
			    	switch (evt.key.keysym.sym) {
			            case SDLK_UP:	
					       	p.moveState = MOVER_CIMA;
					       	p.state= MOVENDO;
							break;
								   
						case SDLK_DOWN:
							p.moveState = MOVER_BAIXO;
							p.state= MOVENDO;
							break;
							
						case SDLK_LEFT:
							p.moveState = MOVER_ESQUERDA;
							p.state= MOVENDO;
							break;
								   
						case SDLK_RIGHT:
							p.moveState = MOVER_DIREITA;
							p.state= MOVENDO;
							break;
					}
				}		
		} else{   
			if(p.state == MOVENDO){
				switch(p.moveState){
					case MOVER_CIMA: //para cima
						if(!Contato_Wall(&p.r, walls) && p.lifeState == ALIVE){
							p.c = (SDL_Rect) {x, y, 20, 20}; 
							p.r.y -= 7;
							if(p.r.y <= -20) p.r.y = 520;
							boca++;
							if(boca % 2 == 0){
								x=0; y=40;
							} else{
								x=40; y=0;
							}
							if(boca > 11){
								boca=0;
							}
							//CONTATO COM MOEDAS
							for(i=3;i<13;i++){
								if(Tem_Contato(&p.r, &fileira_moedas1[i].r)){
									fileira_moedas1[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas1[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas2[i].r)){
									fileira_moedas2[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas2[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas3[i].r)){
									fileira_moedas3[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas3[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas4[i].r)){
									fileira_moedas4[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas4[i].comido = true;
								}
							}
							
							//CONTATO COM FANTASMAS
							if(Tem_Contato(&p.r, &red.r) || Tem_Contato(&p.r, &pink.r)){
								if(p.powerState == SEM_PODER){
									p.lifeState = DEAD;
									p.state = MORRENDO;
									red.state = COLIDIDO;
									pink.state = COLIDIDO;
								}else {
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &red.r)){
										red.lifeState = DEAD;
										posX_red=0, posY_red=0; 
									}
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &pink.r)){
										pink.lifeState = DEAD;
										posX_pink=0, posY_pink=0;
									}
								}
							}
							
							//CONTATO COM FRUTAS
							for(i=0;i<4;i++){
								if(Tem_Contato(&p.r, &frutas[i].r) && frutas[i].aparece){
									frutas[i].r = (SDL_Rect) {0,0,0,0};
									p.powerState = COM_PODER;	
									red.vulnerabilityState = VULNERAVEL;
									pink.vulnerabilityState = VULNERAVEL;						
								}
							}
						}else{
							p.r.y +=5;
							p.moveState = MOVER_BAIXO;
						}
						break;
						
					case MOVER_BAIXO: //para baixo
						if(!Contato_Wall(&p.r, walls) && p.lifeState == ALIVE){
							p.c = (SDL_Rect) {x, y, 20, 20}; 
							p.r.y += 7;
							if(p.r.y >= 520) p.r.y = -20;
							boca++;
							if(boca % 2 == 0){
								x=0; y=60;
							} else{
								x=40; y=0;
							}
							if(boca > 11){
								boca=0;
							}
							//CONTATO COM MOEDAS
							for(i=3;i<13;i++){
								if(Tem_Contato(&p.r, &fileira_moedas1[i].r)){
									fileira_moedas1[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas1[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas2[i].r)){
									fileira_moedas2[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas2[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas3[i].r)){
									fileira_moedas3[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas3[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas4[i].r)){
									fileira_moedas4[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas4[i].comido = true;
								}
							}
							//CONTADO COM FANSTASMAS
							if(Tem_Contato(&p.r, &red.r) || Tem_Contato(&p.r, &pink.r)){
								if(p.powerState == SEM_PODER){
									p.lifeState = MORTO;
									p.state = MORRENDO;
									red.state = COLIDIDO;
									pink.state = COLIDIDO;
								}else {
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &red.r)){
										red.lifeState = DEAD;
										posX_red=0, posY_red=0; 
									}
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &pink.r)){
										pink.lifeState = DEAD;
										posX_pink=0, posY_pink=0;
									}
								}
							}
							//CONTATO COM FRUTAS
							for(i=0;i<4;i++){
								if(Tem_Contato(&p.r, &frutas[i].r) && frutas[i].aparece){
									frutas[i].r = (SDL_Rect) {0,0,0,0};
									p.powerState = COM_PODER;
									red.vulnerabilityState = VULNERAVEL;
									pink.vulnerabilityState = VULNERAVEL;							
								}
							}
						}else{
							p.r.y -=5;
							p.moveState = MOVER_CIMA;
						}
						break;
						
					case MOVER_ESQUERDA: //para esquerda
						if(!Contato_Wall(&p.r, walls) && p.lifeState == ALIVE){
							p.c = (SDL_Rect) {x, y, 20, 20}; 
							p.r.x -= 7;
							if(p.r.x <= -10) p.r.x = 480;
							boca++;
							if(boca % 2 == 0){
								x=0; y=0;
							} else{
								x=40; y=0;
							}
							if(boca > 11){
								boca=0;
							}
							//CONTATO COM MOEDAS
							for(i=3;i<13;i++){
								if(Tem_Contato(&p.r, &fileira_moedas1[i].r)){
									fileira_moedas1[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas1[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas2[i].r)){
									fileira_moedas2[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas2[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas3[i].r)){
									fileira_moedas3[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas3[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas4[i].r)){
									fileira_moedas4[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas4[i].comido = true;
								}
							}
							//CONTADO COM FANSTAMAS
							if(Tem_Contato(&p.r, &red.r) || Tem_Contato(&p.r, &pink.r)){
								if(p.powerState == SEM_PODER){
									p.lifeState = MORTO;
									p.state = MORRENDO;
									red.state = COLIDIDO;
									pink.state = COLIDIDO;
								}else {
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &red.r)){
										red.lifeState = DEAD;
										posX_red=0, posY_red=0; 
									}
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &pink.r)){
										pink.lifeState = DEAD;
										posX_pink=0, posY_pink=0;
									}
								}
							}
							//CONTATO COM FRUTAS
							for(i=0;i<4;i++){
								if(Tem_Contato(&p.r, &frutas[i].r) && frutas[i].aparece){
									frutas[i].r = (SDL_Rect) {0,0,0,0};
									p.powerState = COM_PODER;
									red.vulnerabilityState = VULNERAVEL;
									pink.vulnerabilityState = VULNERAVEL;								
								}
							}
						}else{
							p.r.x +=5;
							p.moveState = MOVER_DIREITA;
						}
						break;
						
					case MOVER_DIREITA: //para direita
						if(!Contato_Wall(&p.r, walls) && p.lifeState == ALIVE){
							p.c = (SDL_Rect) {x, y, 20, 20}; 
							p.r.x += 7;
							if(p.r.x >= 480) p.r.x = -40;
							boca++;
							if(boca % 2 == 0){
								x=0; y=20;
							} else{
								x=40; y=0;
							}
							if(boca > 11){
								boca=0;
							}
							//CONTATO COM MOEDAS
							for(i=3;i<13;i++){
								if(Tem_Contato(&p.r, &fileira_moedas1[i].r)){
									fileira_moedas1[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas1[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas2[i].r)){
									fileira_moedas2[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas2[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas3[i].r)){
									fileira_moedas3[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas3[i].comido = true;
								}
								if(Tem_Contato(&p.r, &fileira_moedas4[i].r)){
									fileira_moedas4[i].c = (SDL_Rect) { 0,0, 0,0 };
									fileira_moedas4[i].comido = true;
								}
							}
							//CONTATO COM FANTASMAS
							if(Tem_Contato(&p.r, &red.r) || Tem_Contato(&p.r, &pink.r)){
								if(p.powerState == SEM_PODER){
									p.lifeState = MORTO;
									p.state = MORRENDO;
									red.state = COLIDIDO;
									pink.state = COLIDIDO;
								}else {
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &red.r)){
										red.lifeState = DEAD;
										posX_red=0, posY_red=0; 

									}
									if(p.powerState == COM_PODER && Tem_Contato(&p.r, &pink.r)){
										pink.lifeState = DEAD;
										posX_pink=0, posY_pink=0; 
									}
								}
							}
							//CONTATO COM FRUTAS
							for(i=0;i<4;i++){
								if(Tem_Contato(&p.r, &frutas[i].r) && frutas[i].aparece){
									frutas[i].r = (SDL_Rect) {0,0,0,0};
									p.powerState = COM_PODER;
									red.vulnerabilityState = VULNERAVEL;
									pink.vulnerabilityState = VULNERAVEL;								
								}
							}
						}else{
							p.r.x -= 5;
							p.moveState = MOVER_ESQUERDA;
						}
						break;
					
				}
			//ANIMAÇÃO INICIAL
			} else if(p.state == PARADO){
					p.c = (SDL_Rect) {0, 0, 20, 20}; 	
			}
			//ANIMAÇÃO AO MORRER
			if(p.state == MORRENDO){
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
					p.state = PARADO;
					p.lifeState = ALIVE;
					isup = 1;
					p.r.x = 150;
					p.r.y = 200;
					red.state = MOVING;
					pink.state = MOVING;
					red.directionState = MOVER_CIMA;
					pink.directionState = MOVER_BAIXO;
					p.vidas -= 1;
					if(p.vidas == 0) continua = false;
				}
			}
			
			
			//ANIMAÇÃO RED GHOST
			switch(red.directionState){
				//vermelho_cima
				case MOVER_CIMA:				
					posY_red -= 5;
					frame_red += 1;
					if(red.vulnerabilityState == VULNERAVEL){
						if(frame_red == 1){
							xR = 4; yR = 162;
						}
						if(frame_red == 2){
							xR = 24; yR = 162;
						}
						if(frame_red == 3){
							xR = 44; yR = 162;
						}
						if(frame_red == 3){
							xR = 64; yR = 162;
							frame_red = 0;
						}
					} else{
						if(frame_red == 1){
							xR = 4; yR = 85;
						}else{
							xR = 24; yR = 85;
							frame_red=0;
						}
					}
					red.c = (SDL_Rect) { xR, yR, wC,hC };
					break;					
			//vemelho_baixo
			case MOVER_BAIXO:
				posY_red += 5;
				frame_red += 1;
				if(red.vulnerabilityState == VULNERAVEL){
					if(frame_red == 1){
						xR = 4; yR = 162;
					}
					if(frame_red == 2){
						xR = 24; yR = 162;
					}
					if(frame_red == 3){
						xR = 44; yR = 162;
					}
					if(frame_red == 3){
						xR = 64; yR = 162;
						frame_red = 0;
					}
				}else{
					if(frame_red == 1){
						xR = 44; yR = 85;
					}else{
						xR = 64; yR = 85;
						frame_red=0;
					}
				}
				red.c = (SDL_Rect) { xR, yR, wC,hC };
				break;
			}
		
			//ANIMAÇÃO PINK GHOST
			switch(pink.directionState){
				//rosa_cima
				case MOVER_CIMA: 
					frame_pink += 1;
					posY_pink -= 5;
					if(pink.vulnerabilityState == VULNERAVEL){
						if(frame_pink == 1){
							xP = 4; yP = 162;
						}
						if(frame_pink == 2){
							xP = 24; yP = 162;
						}
						if(frame_pink == 3){
							xP = 44; yP = 162;
						}
						if(frame_pink == 3){
							xP = 64; yP = 162;
							frame_pink = 0;
						}
					}else{
						if(frame_pink == 1){
							xP = 4; yP = 105;
						}else{
							xP = 24; yP = 105;
							frame_pink =0;
						}
					}
					pink.c = (SDL_Rect) { xP,yP, wC,hC };
					break;
			
				//rosa_baixo
				case MOVER_BAIXO:
					posY_pink += 5;
					frame_pink += 1;
					if(pink.vulnerabilityState == VULNERAVEL){
						if(frame_pink == 1){
							xP = 4; yP = 162;
						}
						if(frame_pink == 2){
							xP = 24; yP = 162;
						}
						if(frame_pink == 3){
							xP = 44; yP = 162;
						}
						if(frame_pink == 3){
							xP = 64; yP = 162;
							frame_pink = 0;
						}
					}else{
						if(frame_pink == 1){
							xP = 44; yP = 105;
						}else{
							xP = 64; yP = 105;
							frame_pink=0;
						}
					}
					pink.c = (SDL_Rect) { xP,yP, wC,hC };
				break;
				
			}

			//MOVIMENTAÇÃO VERTICAL RED GHOST
			if(red.state != COLIDIDO){
				if(posY_red <= -20) posY_red = 520;
			}else{
				red.directionState = PARADO;
			}
			
			
			//MOVIMENTAÇÃO VERTICAL PINK GHOST
			if(pink.state != COLIDIDO){
				if(posY_pink > 500) posY_pink = -10;
			}else{
				pink.directionState = PARADO;
			}
			
			if(red.lifeState != DEAD) SDL_RenderCopy(ren, sprite, &red.c, &red.r);
			if(pink.lifeState != DEAD) SDL_RenderCopy(ren, sprite, &pink.c, &pink.r);
			SDL_RenderPresent(ren);
			SDL_RenderCopy(ren, sprite, &p.c, &p.r); //player	
			
			//VIDAS		
			if(p.vidas == 3){
				SDL_RenderCopy(ren, sprite, &vidas_c, &vidas_r1);
				SDL_RenderCopy(ren, sprite, &vidas_c, &vidas_r2);
				SDL_RenderCopy(ren, sprite, &vidas_c, &vidas_r3);
			}else if(p.vidas == 2){
				SDL_RenderCopy(ren, sprite, &vidas_c, &vidas_r2);
				SDL_RenderCopy(ren, sprite, &vidas_c, &vidas_r3);
			}else if(p.vidas == 1){
				SDL_RenderCopy(ren, sprite, &vidas_c, &vidas_r3);
			}
			stringRGBA(ren, 510, 10, "Quantidade", 250,0,0,255);
			stringRGBA(ren, 510, 20, "de", 250,0,0,255);
			stringRGBA(ren, 510, 30, "vidas", 250,0,0,255);
			
			//MOEDAS
			for(i=3;i<13;i++){
				if(!(fileira_moedas1[i].comido)){
					SDL_RenderCopy(ren, sprite, &fileira_moedas1[i].c, &fileira_moedas1[i].r);
				}
				if(!(fileira_moedas2[i].comido)){
					SDL_RenderCopy(ren, sprite, &fileira_moedas2[i].c, &fileira_moedas2[i].r);
				}
				if(!(fileira_moedas3[i].comido)){
					SDL_RenderCopy(ren, sprite, &fileira_moedas3[i].c, &fileira_moedas3[i].r);
				}
				if(!(fileira_moedas4[i].comido)){
					SDL_RenderCopy(ren, sprite, &fileira_moedas4[i].c, &fileira_moedas4[i].r);
				}
			}
			for(i=3;i<13;i++){
				if(fileira_moedas1[i].comido && !fileira_moedas1[i].contado){
					fileira_moedas1[i].contado = true;
					totalComidos++;
				}
				if(fileira_moedas2[i].comido && !fileira_moedas2[i].contado){
					fileira_moedas2[i].contado = true;
					totalComidos++;
				}
				if(fileira_moedas3[i].comido && !fileira_moedas3[i].contado){
					fileira_moedas3[i].contado = true;
					totalComidos++;
				}
				if(fileira_moedas4[i].comido && !fileira_moedas4[i].contado){
					fileira_moedas4[i].contado = true;
					totalComidos++;
				}
			}
			if(totalComidos == 40){
				continua = false;
			}
			
			//FRUTAS
			for(i=0;i<4;i++){
				if(frutas[i].aparece){
					SDL_RenderCopy(ren, sprite, &frutas[i].c, &frutas[i].r);
				}
			}
			
			//WALLS
			for (i=0; i<8; i++) 
				SDL_RenderFillRect(ren, &walls[i]);

			SDL_RenderPresent(ren);
			espera = 150;
			
			//
			conta_tempo1 += espera;
			if(conta_tempo1 > 15000){ //tempo para fruta aleatória aparecer
				fruta_aleatoria = random()%4;
				frutas[fruta_aleatoria].aparece = true;
				conta_tempo1 = 0;
			}
			conta_tempo2 += espera;
			if(conta_tempo2 > 20000){ //tempo para fruta desaparecer após ter aparecido
				frutas[fruta_aleatoria].aparece = false;
				conta_tempo2 = 0;
			}
			if(p.powerState == COM_PODER) conta_tempo3 += espera; //tempo do player com poder

			if(conta_tempo3 > 6000){ //tempo para acabar o poder do player
				p.powerState= SEM_PODER;
				red.vulnerabilityState = NAO_VULNERAVEL;
				pink.vulnerabilityState = NAO_VULNERAVEL;
				conta_tempo3 = 0;
			}
			free(walls);
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
                         600, 500, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	SDL_Texture* maze = IMG_LoadTexture(ren, "labirinto.png");
	SDL_Texture* sprite = IMG_LoadTexture(ren, "pac-man-sprite.png");
	assert(maze != NULL);
	assert(sprite != NULL);

	bool continua = true;
	
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
