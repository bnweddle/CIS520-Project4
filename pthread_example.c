/* https://www.ibm.com/support/knowledgecenter/SSLTBW_2.4.0/com.ibm.zos.v2r4.bpxbd00/ptasetd.htm */
/* CELEBP11 */                                   
#define _OPEN_THREADS                                                           
#include <stdio.h>                                                              
#include <pthread.h>                                                            
                                                                                
void **stat;                                                                    
void *thread1(void *arg)                                                        
{                                                                               
   printf("hello from the thread\n");                                           
   pthread_exit((void *)0);                                                     
}                                                                               
                                                                                
int main()                                                                      
{                                                                               
   int            ds, rc;                                                       
   size_t         s1;                                                           
   pthread_attr_t attr;                                                         
   pthread_t      thid;                                                         
                                                                                
   rc = pthread_attr_init(&attr);                                               
   if (rc == -1) {                                                              
      perror("error in pthread_attr_init");                                     
      exit(1);                                                                  
   }                                                                            
                                                                                
   ds = 0;                                                                      
   rc = pthread_attr_setdetachstate(&attr, &ds);                                
   if (rc == -1) {                                                              
      perror("error in pthread_attr_setdetachstate");                           
      exit(2);                                                                  
   }                                                                            
                                                                                
   rc = pthread_create(&thid, &attr, thread1, NULL);                            
   if (rc == -1) {                                                              
      perror("error in pthread_create");                                        
      exit(3);                                                                  
   }                                                                            
                                                                                
   rc = pthread_join(thid, stat);                                               
   exit(0);                                                                     
}                                  
