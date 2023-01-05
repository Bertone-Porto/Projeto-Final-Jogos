#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

struct Moedas{
	SDL_Rect moeda[6];
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
                         1000, 800, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
    SDL_Texture* stack = IMG_LoadTexture(ren, "stack.png");
    SDL_Texture* moeda = IMG_LoadTexture(ren, "moeda.png");
    
    assert(stack != NULL);
    assert(moeda != NULL);

    /* EXECUÇÃO */
    bool continua = true;
    SDL_Event evt;
    int isup = 1;
    int x = 400, y=200;
    int espera = 50;
    int yC=-10, wC=40, hC=80;
    int i=0;
    while (continua) {
        SDL_SetRenderDrawColor(ren, 100,100,100,0);
        SDL_RenderClear(ren);
        
        SDL_Rect r_stack = { 900,50, 50,50 };
        SDL_Rect c_stack;
        
        SDL_Rect r_moeda = { x,y, 20,30 };
        SDL_Rect c_moeda;
        struct Moedas moedas[5];
        
      	Uint32 antes = SDL_GetTicks();
        int isevt = AUX_WaitEventTimeoutCount(&evt,&espera);       
        if(isevt){       	
		switch (evt.type ) {
		    case SDL_QUIT:
		    	continua = false;
		    	break;		
		}
	
	}
	
	else{   
		switch(isup){
			case 1:
				c_moeda = (SDL_Rect) {   0, 0, 180,165 };
				moedas[i].moeda[0] = c_moeda;
			break;
			case 2:
				c_moeda = (SDL_Rect) {   167, 0, 180,165 };
				moedas[i].moeda[1] = c_moeda;
			break;
			case 3:
				c_moeda = (SDL_Rect) {   337, 0, 180,165 };
				moedas[i].moeda[2] = c_moeda;
			break;
			case 4:
				c_moeda = (SDL_Rect) {   507, 0, 180,165 };
				moedas[i].moeda[3] = c_moeda;
			break;
			case 5:
				c_moeda = (SDL_Rect) {   737, 0, 180,165 };
				moedas[i].moeda[4] = c_moeda;
			break;
			case 6:
				c_moeda = (SDL_Rect) {   957, 0, 180,165 };
				moedas[i].moeda[5] = c_moeda;
			break;
			

		}
		isup++;
		
		c_stack = (SDL_Rect) {   150, 50, 3100,3600 }; //stack de moedas
		
		//c_moeda = (SDL_Rect) {   170, 0, 180,165 };
		
		SDL_RenderCopy(ren, stack, &c_stack, &r_stack);
		for(i=0;i<isup;i++){
			SDL_RenderCopy(ren, moeda, &moedas->moeda[i], &r_moeda);
			SDL_RenderPresent(ren);
		}
		SDL_RenderCopy(ren, moeda, &moedas->moeda[i], &r_moeda);
		SDL_RenderPresent(ren);
		i++;
		espera = 150;
		x += 50;

		if(isup > 6){
			isup = 1;
			i=0;
			x = 50;
		}
		
	}
    }

    /* FINALIZACAO */
    SDL_DestroyTexture(stack);
    SDL_DestroyTexture(moeda);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
