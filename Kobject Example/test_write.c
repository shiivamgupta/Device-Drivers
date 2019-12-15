#include<unistd.h>
#include<sys/types.h>
#include<linux/fcntl.h>


int main()
{
int f,b1,b2,ret1,ret2,ret3;0
char buf1[32];
char buf2[32];
char buf3[32];
  f= open("/sys/kernel/kobject_example2/foo", O_RDWR); 
   	//pdfd0  = open("/sys/bus/platform/devices/custom_leds/leds/led1", O_RDWR); 
   	//pdfd0  = open("/dev/pseudo0", O_RDWR|O_NONBLOCK); //device file accessed in non-blocking mode
   	if(f<0) { 
		perror("error in opening led1"); 
		exit(1);
	}
   	b1  = open("/sys/kernel/kobject_example2/bar", O_RDWR); 
   
   	if(b1<0) { 
		perror("error in opening led2"); 
		exit(1);
	}
   	b2  = open("/sys/kernel/kobject_example2/baz", O_RDWR); 
   	
   	if(b2<0) { 
		perror("error in opening led3"); 
		exit(1);
       }		


        write(f, "dishant", 7); 
        write(b1, "patel",5 );   //We need binary sys file to pass binary data.
        write(b2, "sachin", 6);	


lseek(f,0,SEEK_SET); //active files offset will be reset to zero
lseek(b1,2,SEEK_SET);
lseek(b2,3,SEEK_SET);





	ret1 = read(f,buf1,sizeof(buf1));
   if(ret1<0) { perror("error in read"); exit(6); }
 //  printf(" Value of ret1 is %d\n",ret1);

   printf("The value of foo is %s\n", buf1);

  ret2 = read(b1,buf2,sizeof(buf2));
   if(ret1<0) { perror("error in read"); exit(6); }
   //printf(" Value of ret1 is %d\n",ret2);
   printf("The value of baz is %s\n", buf2);
ret3 = read(b2,buf3,sizeof(buf3));
   if(ret1<0) { perror("error in read"); exit(6); }
   

//printf(" Value of ret1 is %d\n",ret3);

   printf("The value of bar is %s\n", buf3);

}
