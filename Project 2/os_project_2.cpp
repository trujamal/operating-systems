/*
* Card Matching Fight - a simple card game with one dealer, 3 players, and a single deck of cards.
* Copyright (C) 2019 Jamal Rasool
* Class: CS4328 Operating Systems
* Date: Monday April 22nd 2019
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>         

#define NUM_THREADS 3 // Number of threads due to the 3 test cases.  
#define NUM_CARDS_IN_DECK 52 // Cards in deck
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
int turn_val = 0;    

// Other Intit
int deck[NUM_CARDS_IN_DECK];       
int *beginning_of_deck;            
int *end_of_deck;         
int seed_value = 0;              
bool win_flag = false;   

// Generating functions
void parseArgs(char *[]);
void generate_random_seed();
void generate_deck();
void print_output();

// Player thread functions
void *dealer_thread(void *arg);
void *player_threads(void *current_player_id);

// Deck Logic Handler
void deck_handler(long, Deck_init);
void shuffle_handler();
void deal_handler();
void round_handler();

// Take in command Arguments
void parseArgs(char *argv[]){ 
   seed_value = atoi( argv[1] );
} 

// Generate random value and set to global variable
void generate_random_seed(){ 
   srand(seed_value);
} 

// Card Logic
void generate_deck(){ 
   int card_counter = 0, suit_count = 4, card_at_hand = 0;
   while( (card_at_hand < NUM_CARDS_IN_DECK) && 
          (card_counter < (NUM_CARDS_IN_DECK/suit_count)) ){
      for( int i = 0; i < suit_count; i++ ){
         deck[card_at_hand] = card_counter;
         card_at_hand++;
      }
      card_counter++;
   }    
   beginning_of_deck = deck;          
   end_of_deck = deck + 51;  
} 

// Printing output data for rounds
void round_header(){
   printf("---------------------------------------------------------------------------------------------------------------\n");
   printf("ROUND: %d ................\n", round_counter);
   printf("---------------------------------------------------------------------------------------------------------------\n");
   fprintf(output_file,"---------------------------------------------------------------------------------------------------------------\n");
   fprintf(output_file, "ROUND: %d ................\n", round_counter);
   fprintf(output_file,"---------------------------------------------------------------------------------------------------------------\n");
}

void round_handler(){ 
   // printing out the data 
   round_header();

   int round_Dealer = pthread_create(&dealer_threads_handler, NULL, &dealer_thread, NULL);
   int round_Player;   

   for( long a = 1; a <= NUM_THREADS; a++)
      round_Player = pthread_create(&player_threads_handler[a], NULL, &player_threads, (void *)a);

   // J\joinning threads
   pthread_join(dealer_threads_handler, NULL); 

   for(int b = 0; b < 3; b++)
      pthread_join(player_threads_handler[b], NULL); 
} 

// Dealer logic
void *dealer_thread(void *arg){ 
   Deck_init dealerHand;    
   long process_id = 0;       
   turn_val = 0;           
   deck_handler(process_id, dealerHand); 
   
   // Waiting conditions
   pthread_mutex_lock(&mutext_exit_cond);  
      while(!win_flag ){
         pthread_cond_wait(&win_condition, &mutext_exit_cond);
      }      
   pthread_mutex_unlock(&mutext_exit_cond); 
   fprintf(output_file, "DEALER: stops round\n");  

   // Exit condition
   pthread_exit(NULL);
} 


void *player_threads(void *current_player_id){ 
   long pId = (long)current_player_id;
   
   
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
   
   while(win_flag == 0 ){
      pthread_mutex_lock(&mutext_deck_handler); 
         while( pId != turn_val && win_flag == 0 ){ 
            pthread_cond_wait(&exclusion_variable, &mutext_deck_handler); 
         }
         if(win_flag == 0 ){   
            deck_handler(pId, thisHand); 
         }         
      pthread_mutex_unlock(&mutext_deck_handler); 
   }
   
   fprintf(output_file, "PLAYER %ld: exits round\n", pId); 
   pthread_exit(NULL);   
} 


void deck_handler(long pId, Deck_init thisHand){
   if( pId == 0 ){ 
   
      fprintf(output_file, "DEALER: shuffle\n"); shuffle_handler(); 
      fprintf(output_file, "DEALER: deal\n"); deal_handler();      
   }    
   else{ 
      fprintf(output_file, "PLAYER %ld: hand %d \n", pId, thisHand.card1);

      thisHand.card2 = *beginning_of_deck, 
      beginning_of_deck = beginning_of_deck + 1;
      fprintf(output_file, "PLAYER %ld: draws %d \n", pId,thisHand.card2); 
      
      
      printf("HAND %d %d\n", thisHand.card1, thisHand.card2);
      fprintf(output_file, "PLAYER %ld: hand %d %d\n", pId, thisHand.card1, thisHand.card2);        
            
      
      if( thisHand.card1 == thisHand.card2 ){
         
         printf("WIN yes\n");      
         fprintf(output_file, "PLAYER %ld: wins\n", pId);
         win_flag = true;   
         pthread_cond_signal(&win_condition); 
      }
      else{
         
         printf("WIN no\n");    
         
         beginning_of_deck = beginning_of_deck - 1;       
         int *ptr = beginning_of_deck;
         while( ptr != end_of_deck ){
            *ptr = *(ptr + 1);
            ptr = ptr + 1;      
         }         
         
         int discard = rand()%2;
         if( discard == 0 ){
            fprintf(output_file, "PLAYER %ld: discards %d \n", pId, thisHand.card1);   
            *end_of_deck = thisHand.card1;  
            thisHand.card1 = thisHand.card2; 
         }     
         else{
            fprintf(output_file, "PLAYER %ld: discards %d \n", pId, thisHand.card2);             
            *end_of_deck = thisHand.card2;
         }   
         
         print_output();                 
      }      
   }  
   turn_val ++; 
   if( turn_val > NUM_THREADS ) turn_val = 1;      
   pthread_cond_broadcast(&exclusion_variable); 
} 


void shuffle_handler(){ 
   for( int i = 0; i < (NUM_CARDS_IN_DECK - 1); i++ ){
      int randPos = i + (rand() % (NUM_CARDS_IN_DECK - i));  
      int temp = deck[i];
      deck[i] = deck[randPos]; 
      deck[randPos] = temp;
   }
} 


void deal_handler(){ 
   player_hand_1.card1 = *beginning_of_deck; beginning_of_deck = beginning_of_deck + 1; 
   player_hand_2.card1 = *beginning_of_deck; beginning_of_deck = beginning_of_deck + 1;  
   player_hand_3.card1 = *beginning_of_deck; beginning_of_deck = beginning_of_deck + 1;          
} 


void print_output(){ 
   printf("DECK: ");
   fprintf(output_file, "DECK: ");    
   int *ptr = beginning_of_deck;
   while( ptr != end_of_deck ){
      printf("%d ", *ptr);
      fprintf(output_file, "%d ", *ptr);
      ptr++;
      if( ptr == end_of_deck ){
         printf("%d \n", *ptr);
         fprintf(output_file, "%d \n", *ptr);     
      }      
   }
}


int main(int argc, char *argv[]){
   output_file = fopen("report.txt", "a"); 
   parseArgs(argv);               
   generate_random_seed();                    
   generate_deck();                   
   
   while(round_counter <= number_of_cases ){
      round_handler();                
      round_counter++;                 
      win_flag = false;                
   }

   fclose(output_file);                 
   exit(EXIT_SUCCESS);
} 