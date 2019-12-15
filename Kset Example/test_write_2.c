#include<unistd.h>
#include<sys/types.h>
#include<linux/fcntl.h>
#include<pthread.h>





int f,b1,b2,ret1,ret2,ret3,g,c1,c2,h,d1,d2,ret4,ret5,ret6,ret7,ret8,ret9;
char buf1[32];
char buf2[32];
char buf3[32];

char buf4[32];
char buf5[32];
char buf6[32];

char buf7[32];
char buf8[32];
char buf9[32];

void *device1(int arr[]){


        write(arr[0], "0", 1);
        write(arr[1], "1",1 );   //We need binary sys file to pass binary data.
        write(arr[2], "2", 1);


lseek(arr[0],0,SEEK_SET); //active files offset will be reset to zero
lseek(arr[1],0,SEEK_SET);
lseek(arr[2],0,SEEK_SET);





        ret1 = read(arr[0],buf1,sizeof(buf1));
   if(ret1<0) { perror("error in read"); exit(6); }
 //  printf(" Value of ret1 is %d\n",ret1);

   printf("The value of dev_param1 is %s\n", buf1);

  ret2 = read(arr[1],buf2,sizeof(buf2));
   if(ret1<0) { perror("error in read"); exit(6); }
   //printf(" Value of ret1 is %d\n",ret2);
   printf("The value of dev_param2 is %s\n", buf2);
ret3 = read(arr[2],buf3,sizeof(buf3));
   if(ret1<0) { perror("error in read"); exit(6); }


//printf(" Value of ret1 is %d\n",ret3);

   printf("The value of dev_param3 is %s\n", buf3);

return NULL;

}


void *device2(int arr[]){


        write(arr[0], "3", 1);
        write(arr[1], "4",1 );   //We need binary sys file to pass binary data.
        write(arr[2], "5", 1);


lseek(arr[0],0,SEEK_SET); //active files offset will be reset to zero
lseek(arr[1],0,SEEK_SET);
lseek(arr[2],0,SEEK_SET);





        ret4 = read(arr[0],buf4,sizeof(buf4));
   if(ret4<0) { perror("error in read"); exit(6); }
 //  printf(" Value of ret1 is %d\n",ret1);

   printf("The value of dev_param1 is %s\n", buf4);

  ret5 = read(arr[1],buf5,sizeof(buf5));
   if(ret1<0) { perror("error in read"); exit(6); }
   //printf(" Value of ret1 is %d\n",ret2);
   printf("The value of dev_param2 is %s\n", buf5);
ret6 = read(arr[2],buf6,sizeof(buf6));
   if(ret6<0) { perror("error in read"); exit(6); }


//printf(" Value of ret1 is %d\n",ret3);

   printf("The value of dev_param3 is %s\n", buf6);

return NULL;

}




void *device3(int arr[]){


        write(arr[0], "6", 1);
        write(arr[1], "7",1 );   //We need binary sys file to pass binary data.
        write(arr[2], "8", 1);


lseek(arr[0],0,SEEK_SET); //active files offset will be reset to zero
lseek(arr[1],0,SEEK_SET);
lseek(arr[2],0,SEEK_SET);





        ret7 = read(arr[0],buf7,sizeof(buf7));
   if(ret7<0) { perror("error in read"); exit(6); }
 //  printf(" Value of ret1 is %d\n",ret1);

   printf("The value of dev_param1 is %s\n", buf7);

  ret8 = read(arr[1],buf8,sizeof(buf8));
   if(ret8<0) { perror("error in read"); exit(6); }
   //printf(" Value of ret1 is %d\n",ret2);
   printf("The value of dev_param2 is %s\n", buf8);
ret8 = read(arr[2],buf9,sizeof(buf9));
   if(ret9<0) { perror("error in read"); exit(6); }


//printf(" Value of ret1 is %d\n",ret3);

   printf("The value of dev_param3 is %s\n", buf9);

return NULL;

}







int dev1[3];
int dev2[3];
int dev3[3];
int main()
{

  f= open("/sys/kernel/kset_devices_typeA/device0/dev_param1", O_RDWR);
   
   	//pdfd0  = open("/sys/bus/platform/devices/custom_leds/leds/led1", O_RDWR); 
   	//pdfd0  = open("/dev/pseudo0", O_RDWR|O_NONBLOCK); //device file accessed in non-blocking mode
   	if(f<0) { 
		perror("error in opening dev_param1"); 
		exit(1);
	}
   	dev1[0] = f;

	
	
	b1  = open("/sys/kernel/kset_devices_typeA/device0/dev_param2", O_RDWR); 
   
   	if(b1<0) { 
		perror("error in opening param2"); 
		exit(1);
	}
dev1[1] = b1;

   	b2  = open("/sys/kernel/kset_devices_typeA/device0/dev_param3", O_RDWR); 
   	
   	if(b2<0) { 
		perror("error in opening param3"); 
		exit(1);
       }		
	dev1[2] = b2;




 g= open("/sys/kernel/kset_devices_typeA/device1/dev_param1", O_RDWR);

        //pdfd0  = open("/sys/bus/platform/devices/custom_leds/leds/led1", O_RDWR); 
        //pdfd0  = open("/dev/pseudo0", O_RDWR|O_NONBLOCK); //device file accessed in non-blocking mode
        if(g<0) {
                perror("error in opening dev_param1");
                exit(1);
        }
        dev2[0] = g;



        c1  = open("/sys/kernel/kset_devices_typeA/device1/dev_param2", O_RDWR);

        if(c1<0) {
                perror("error in opening param2");
                exit(1);
        }
dev2[1] = c1;

        c2  = open("/sys/kernel/kset_devices_typeA/device1/dev_param3", O_RDWR);

        if(c2<0) {
                perror("error in opening param3");
                exit(1);
       }
        dev2[2] = c2;




	 h= open("/sys/kernel/kset_devices_typeA/device2/dev_param1", O_RDWR);

        //pdfd0  = open("/sys/bus/platform/devices/custom_leds/leds/led1", O_RDWR);
        //pdfd0  = open("/dev/pseudo0", O_RDWR|O_NONBLOCK); //device file accessed in non-blocking mode
        if(h<0) {
                perror("error in opening dev_param1");
                exit(1);
        }
        dev3[0] = h;



        d1  = open("/sys/kernel/kset_devices_typeA/device1/dev_param2", O_RDWR);

        if(d1<0) {
                perror("error in opening param2");
                exit(1);
        }
dev3[1] = d1;

        d2  = open("/sys/kernel/kset_devices_typeA/device2/dev_param3", O_RDWR);

        if(d2<0) {
                perror("error in opening param3");
                exit(1);
       }
        dev3[2] = d2;




	pthread_t thread_pid1,thread_pid2,thread_pid3;

	pthread_create(&thread_pid1,NULL,device1,dev1);
        pthread_create(&thread_pid2,NULL,device2,dev2);
        pthread_create(&thread_pid3,NULL,device3,dev3);

       
pthread_join(thread_pid1,NULL);
pthread_join(thread_pid2,NULL);
pthread_join(thread_pid3,NULL);



}
