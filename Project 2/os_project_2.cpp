/*******************************************************************************
*   course:       CS4328 
*   project:      Project 2 - Pair War 
*   programmer:   Jamal Rasool
*   due:          12/03/2015  
*   submitted:    12/05/2015
*   description:  Pair War Card Game
*******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>         
#define NUM_THREADS 3 // Number of whitepsace threads.  

pthread_mutex_t mutex_useDeck = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex_dealerExit = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_win1 = PTHREAD_COND_INITIALIZER;
pthread_t playerThreads[NUM_THREADS];
pthread_t dealerThread;

FILE * pFile;              
int roundNum = 1;          
int numberOfRounds = 3;    
int some_value = 1;        
int turn = 0;              
const int NUM_CARDS = 52;  
int deck[NUM_CARDS];       
int *topOfDeck;            
int *bottomOfDeck;         
int seed = 0;              
bool win = false;          
struct hand{               
   int card1, card2;
};
hand hand1, hand2, hand3;  


void *dealer_thread(void *arg);
void *player_threads(void *playerId);
void useTheDeck(long, hand);
void randSeed();
void buildDeck();
void shuffleDeck();
void dealCards();
void playRound();
void parseArgs(char *[]);
void printDeck();



int main(int argc, char *argv[]){
   pFile = fopen("log.txt", "a"); 
   parseArgs(argv);               
   randSeed();                    
   buildDeck();                   
   
   while( roundNum <= 
          numberOfRounds ){
      playRound();                
      roundNum++;                 
      win = false;                
   }
   
   fclose(pFile);                 
   exit(EXIT_SUCCESS);
} 




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
   printf("ROUND: %d ................\n", roundNum);
   fprintf(pFile, "ROUND: %d ................\n", roundNum);
           
   
   int retD = pthread_create(&dealerThread, NULL, &dealer_thread, NULL);
            
   
   int retP;
   for( long i = 1; i <= NUM_THREADS; i++ ){
      retP = pthread_create(&playerThreads[i], NULL, &player_threads, (void *)i);
   }

   
   pthread_join(dealerThread, NULL); 
   for( int j = 0; j < 3; j++ ){
      pthread_join(playerThreads[j], NULL); 
   }
} 


void *dealer_thread(void *arg){ 
   long pId = 0;       
   turn = 0;           
   hand dealerHand;    
   useTheDeck(pId, dealerHand); 
   
   
   pthread_mutex_lock(&mutex_dealerExit);  
      while( !win ){
         pthread_cond_wait(&cond_win1, &mutex_dealerExit);
      }      
   pthread_mutex_unlock(&mutex_dealerExit); 
   fprintf(pFile, "DEALER: exits round\n");  
   pthread_exit(NULL);
} 


void *player_threads(void *playerId){ 
   long pId = (long)playerId;
   
   
   hand thisHand;
   if( roundNum == 1 ){
      if( pId == 1 ) thisHand = hand1; 
      else if( pId == 2 ) thisHand = hand2;
      else thisHand = hand3;
   } 
   else if( roundNum == 2 ){
      if( pId == 2 ) thisHand = hand1; 
      else if( pId == 3 ) thisHand = hand2;
      else thisHand = hand3;
   }    
   else if( roundNum == 3 ){
      if( pId == 3 ) thisHand = hand1; 
      else if( pId == 1 ) thisHand = hand2;
      else thisHand = hand3;
   }   
   
   while( win == 0 ){
      pthread_mutex_lock(&mutex_useDeck); 
         while( pId != turn && win == 0 ){ 
            pthread_cond_wait(&condition_var, &mutex_useDeck); 
         }
         if( win == 0 ){   
            useTheDeck(pId, thisHand); 
         }         
      pthread_mutex_unlock(&mutex_useDeck); 
   }
   
   fprintf(pFile, "PLAYER %ld: exits round\n", pId); 
   pthread_exit(NULL);   
} 


void useTheDeck(long pId, hand thisHand){
   if( pId == 0 ){ 
   
      fprintf(pFile, "DEALER: shuffle\n"); shuffleDeck(); 
      fprintf(pFile, "DEALER: deal\n"); dealCards();      
   }    
   else{ 
   
      
      fprintf(pFile, "PLAYER %ld: hand %d \n", pId, thisHand.card1);
      
      
      thisHand.card2 = *topOfDeck, 
      topOfDeck = topOfDeck + 1;
      fprintf(pFile, "PLAYER %ld: draws %d \n", pId,thisHand.card2); 
      
      
      printf("HAND %d %d\n", thisHand.card1, thisHand.card2);
      fprintf(pFile, "PLAYER %ld: hand %d %d\n", pId, thisHand.card1, thisHand.card2);        
            
      
      if( thisHand.card1 == thisHand.card2 ){
         
         printf("WIN yes\n");      
         fprintf(pFile, "PLAYER %ld: wins\n", pId);
         win = true;   
         pthread_cond_signal(&cond_win1); 
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
            fprintf(pFile, "PLAYER %ld: discards %d \n", pId, thisHand.card1);   
            *bottomOfDeck = thisHand.card1;  
            thisHand.card1 = thisHand.card2; 
         }     
         else{
            fprintf(pFile, "PLAYER %ld: discards %d \n", pId, thisHand.card2);             
            *bottomOfDeck = thisHand.card2;
         }   
         
         printDeck();                 
      }      
   }  
   turn ++; 
   if( turn > NUM_THREADS ) turn = 1;      
   pthread_cond_broadcast(&condition_var); 
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
   hand1.card1 = *topOfDeck; topOfDeck = topOfDeck + 1; 
   hand2.card1 = *topOfDeck; topOfDeck = topOfDeck + 1;  
   hand3.card1 = *topOfDeck; topOfDeck = topOfDeck + 1;          
} 


void printDeck(){ 
   printf("DECK: ");
   fprintf(pFile, "DECK: ");    
   int *ptr = topOfDeck;
   while( ptr != bottomOfDeck ){
      printf("%d ", *ptr);
      fprintf(pFile, "%d ", *ptr);
      ptr++;
      if( ptr == bottomOfDeck ){
         printf("%d \n", *ptr);
         fprintf(pFile, "%d \n", *ptr);     
      }      
   }
}
