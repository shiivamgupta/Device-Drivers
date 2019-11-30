#include <linux/init.h>
#include <linux/module.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include<linux/kernel.h>
/*Kernel Module Object contain code,data,other sections*/

/*When insmod is invoked  to load your kernel module, insmod invokes a system call API,which inturns requests memory from system's physical memory manager a*/

/*you may declare the global variables for a module the memory is allocated using non-contiguous memory allocator of the kernel*/

int new_symbol=5;//this will be allocated in the data region of the memory
char*str1=NULL;
int *ptr1=0x0000000a;
char *p2=NULL;
struct page*p1=NULL;


MODULE_LICENSE("GPL");//Special macro which is also function internally is used to tell the  kernel that this modulebears free licence
MODULE_AUTHOR("Dishant Patel");
static int __init load(void){

int local_var5=0;//This variable will be stored in the system stack

dump_stack(); // function which will give you a call trace of system stack for the debug purpose

printk(KERN_ALERT " External Dynamic Module Is loaded"); //printk() function is defined in the linux  kernel and made available to modules,it behaves similarly
                                          //to the printf()..Kernel needs it own printing function because it cannot acess c library  

p1=alloc_page(GFP_KERNEL);                //The string KERN_ALERT is priority of the message.We have specified a high priority ,because a message with default
                                          //priority might not  show up

if(p1!=NULL){
p2=kmap(p1);
if(p2!=NULL){
printk("KVAS address is %x\n",p2);
}

} 

return 0;
}

static void unload(void){
dump_stack();
printk(KERN_ALERT"kernel is unloaded\n");//Use dmesd is used to check the message stored in the temporary buffer while syslog file can also be used to check the message 
                                         //but it is permament storage.  
}
EXPORT_SYMBOL_GPL(new_symbol);
module_init(load); //The module_init() macro defines which function is to be called at module insertion time (if the file is compiled as a module), or at boot time:
module_exit(unload);//This macro defines the function to be called at module removal time (or never, in the case of the file compiled into the kernel). It will only be called if the module usage count has reached zero
//Both are basically functions only

MODULE_LICENSE("GPL");
