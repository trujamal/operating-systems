/*******************************************************************************
*   course:       CS4328 
*   project:      Project 2 - Pair War 
*   programmer:   Jamal Rasool
*******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>         
#define NUM_THREADS 3 // Number of threads due to the 3 test cases.  
#define PTHREADM PTHREAD_MUTEX_INITIALIZER
#define PTHREADC PTHREAD_COND_INITIALIZER

// Deck Struct
struct Deck_init{               
   int card1, card2;
};

// Intialize an empy deck obj
Deck_init player_hand_1, player_hand_2, player_hand_3;  

// Pthread Initialization
pthread_mutex_t mutext_deck_handler = PTHREADM;
pthread_cond_t exclusion_variable = PTHREADC;
pthread_mutex_t mutext_exit_cond = PTHREADM;
pthread_cond_t win_condition = PTHREADC;

// player and dealer init
pthread_t player_threads_handler[NUM_THREADS];
pthread_t dealer_threads_handler;

// output file for the log data
FILE * output_file;   

// Declaration of Global Variables
int round_counter = 1; //init to 1       
int number_of_cases = 3;    
int turn = 0;              
const int NUM_CARDS = 52;  
int deck[NUM_CARDS];       
int *topOfDeck;            
int *bottomOfDeck;         
int seed = 0;              
bool win = false;   


void *dealer_thread(void *arg);
void *player_threads(void *playerId);
void useTheDeck(long, Deck_init);
void randSeed();
void buildDeck();
void shuffleDeck();
void dealCards();
void playRound();
void parseArgs(char *[]);
void print_output();








void parseArgs(char *argv[]){ 
   seed = atoi( argv[1] );
} 


void randSeed(){ 
   srand(seed);
} 


void buildDeck(){ 
   int cardVal = 0, numInSuit = 4, card = 0;
   while( (card < NUM_CARDS) && 
          (cardVal < (NUM_CARDS/numInSuit)) ){
      for( int i = 0; i < numInSuit; i++ ){
         deck[card] = cardVal;
         card++;
      }
      cardVal++;
   }    
   topOfDeck = deck;          
   bottomOfDeck = deck + 51;  
} 


void playRound(){ 
   printf("ROUND: %d ................\n", round_counter);
   fprintf(output_file, "ROUND: %d ................\n", round_counter);
           
   
   int retD = pthread_create(&dealer_threads_handler, NULL, &dealer_thread, NULL);
            
   
   int retP;
   for( long i = 1; i <= NUM_THREADS; i++ ){
      retP = pthread_create(&player_threads_handler[i], NULL, &player_threads, (void *)i);
   }

   
   pthread_join(dealer_threads_handler, NULL); 
   for( int j = 0; j < 3; j++ ){
      pthread_join(player_threads_handler[j], NULL); 
   }
} 


void *dealer_thread(void *arg){ 
   long pId = 0;       
   turn = 0;           
   Deck_init dealerHand;    
   useTheDeck(pId, dealerHand); 
   
   
   pthread_mutex_lock(&mutext_exit_cond);  
      while( !win ){
         pthread_cond_wait(&win_condition, &mutext_exit_cond);
      }      
   pthread_mutex_unlock(&mutext_exit_cond); 
   fprintf(output_file, "DEALER: exits round\n");  
   pthread_exit(NULL);
} 


void *player_threads(void *playerId){ 
   long pId = (long)playerId;
   
   
   Deck_init thisHand;
   if( round_counter == 1 ){
      if( pId == 1 ) thisHand = player_hand_1; 
      else if( pId == 2 ) thisHand = player_hand_2;
      else thisHand = player_hand_3;
   } 
   else if( round_counter == 2 ){
      if( pId == 2 ) thisHand = player_hand_1; 
      else if( pId == 3 ) thisHand = player_hand_2;
      else thisHand = player_hand_3;
   }    
   else if( round_counter == 3 ){
      if( pId == 3 ) thisHand = player_hand_1; 
      else if( pId == 1 ) thisHand = player_hand_2;
      else thisHand = player_hand_3;
   }   
   
   while( win == 0 ){
      pthread_mutex_lock(&mutext_deck_handler); 
         while( pId != turn && win == 0 ){ 
            pthread_cond_wait(&exclusion_variable, &mutext_deck_handler); 
         }
         if( win == 0 ){   
            useTheDeck(pId, thisHand); 
         }         
      pthread_mutex_unlock(&mutext_deck_handler); 
   }
   
   fprintf(output_file, "PLAYER %ld: exits round\n", pId); 
   pthread_exit(NULL);   
} 


void useTheDeck(long pId, Deck_init thisHand){
   if( pId == 0 ){ 
   
      fprintf(output_file, "DEALER: shuffle\n"); shuffleDeck(); 
      fprintf(output_file, "DEALER: deal\n"); dealCards();      
   }    
   else{ 
   
      
      fprintf(output_file, "PLAYER %ld: hand %d \n", pId, thisHand.card1);
      
      
      thisHand.card2 = *topOfDeck, 
      topOfDeck = topOfDeck + 1;
      fprintf(output_file, "PLAYER %ld: draws %d \n", pId,thisHand.card2); 
      
      
      printf("HAND %d %d\n", thisHand.card1, thisHand.card2);
      fprintf(output_file, "PLAYER %ld: hand %d %d\n", pId, thisHand.card1, thisHand.card2);        
            
      
      if( thisHand.card1 == thisHand.card2 ){
         
         printf("WIN yes\n");      
         fprintf(output_file, "PLAYER %ld: wins\n", pId);
         win = true;   
         pthread_cond_signal(&win_condition); 
      }
      else{
         
         printf("WIN no\n");    
         
         topOfDeck = topOfDeck - 1;       
         int *ptr = topOfDeck;
         while( ptr != bottomOfDeck ){
            *ptr = *(ptr + 1);
            ptr = ptr + 1;      
         }         
         
         int discard = rand()%2;
         if( discard == 0 ){
            fprintf(output_file, "PLAYER %ld: discards %d \n", pId, thisHand.card1);   
            *bottomOfDeck = thisHand.card1;  
            thisHand.card1 = thisHand.card2; 
         }     
         else{
            fprintf(output_file, "PLAYER %ld: discards %d \n", pId, thisHand.card2);             
            *bottomOfDeck = thisHand.card2;
         }   
         
         print_output();                 
      }      
   }  
   turn ++; 
   if( turn > NUM_THREADS ) turn = 1;      
   pthread_cond_broadcast(&exclusion_variable); 
} 


void shuffleDeck(){ 
   for( int i = 0; i < (NUM_CARDS - 1); i++ ){
      int randPos = i + (rand() % (NUM_CARDS - i));  
      int temp = deck[i];
      deck[i] = deck[randPos]; 
      deck[randPos] = temp;
   }
} 


void dealCards(){ 
   player_hand_1.card1 = *topOfDeck; topOfDeck = topOfDeck + 1; 
   player_hand_2.card1 = *topOfDeck; topOfDeck = topOfDeck + 1;  
   player_hand_3.card1 = *topOfDeck; topOfDeck = topOfDeck + 1;          
} 


void print_output(){ 
   printf("DECK: ");
   fprintf(output_file, "DECK: ");    
   int *ptr = topOfDeck;
   while( ptr != bottomOfDeck ){
      printf("%d ", *ptr);
      fprintf(output_file, "%d ", *ptr);
      ptr++;
      if( ptr == bottomOfDeck ){
         printf("%d \n", *ptr);
         fprintf(output_file, "%d \n", *ptr);     
      }      
   }
}


int main(int argc, char *argv[]){
   output_file = fopen("log.txt", "a"); 
   parseArgs(argv);               
   randSeed();                    
   buildDeck();                   
   
   while( round_counter <= 
          number_of_cases ){
      playRound();                
      round_counter++;                 
      win = false;                
   }
   
   fclose(output_file);                 
   exit(EXIT_SUCCESS);
} 