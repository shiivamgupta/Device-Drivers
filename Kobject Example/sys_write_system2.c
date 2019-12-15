/*
 * Sample kobject implementation
 *
 * Copyright (C) 2004-2007 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (C) 2007 Novell Inc.
 *
 * Released under the GPL version 2 only.
 *
 */
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>

/*
 * This module shows how to create a simple subdirectory in sysfs called
 * /sys/kernel/kobject-example  In that directory, 3 files are created:
 * "foo", "baz", and "bar".  If an integer is written to these files, it can be
 * later read out of it.
 */
//these are dummy variable associated with 
//our attributes/device specific attributes(kobject specific)
//
//it may be abstract 
static char foo[32];
static char baz[32];
//atic char baz[32];
static char bar[32];


/*
 * The "foo" file where a static variable is read from and written to.
 */
//this is a typical xxx_show() call back - we can also 
//provide a similar xxx_store() call back 
//this xxx_show() call back will be invoked, 
//as part of a kernel execution path of a read() system 
//call on an active sysfs file 
//
//like any call back, this will be passed one  or more 
//system objects - these call backs are re-entrant 
//
//a kernel buffer is passed, for reading system data
//this kernel buffer must be filled, as per rules
//the same kernel buffer will be used to copy 
//data into the user-space buffer of the 
//read() system call API 
static ssize_t foo_show(struct kobject *kobj, 
                        struct kobj_attribute *attr,
			char *buf)
{
        dump_stack();
        //we need to pass an integer value, but format as a string 
        //and send it - this is the convention 
        //when the string is received, in the user-space, we must 
        //again extract the integer value, from the string  
	//printk("data string is %s\n", buf);
     int ret= sprintf(buf, "%s", foo);
        printk("data string is %s\n", buf);
        return ret;
       
}

  //this is a typical xxx_store() call back - we can also 
//provide a similar xxx_show() call back 
//this xxx_store() call back will be invoked, 
//as part of a kernel execution path of a write() system 
//call on an active sysfs file 
//
//like any call back, this will be passed one  or more 
//system objects - these call backs are re-entrant 
//
//a kernel buffer is passed, for writing system data
//this kernel buffer must be filled, as per rules
//the same kernel buffer will be used to copy 
//data from the user-space buffer of the 
//write() system call API

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{

        dump_stack();
	sscanf(buf,"%s", foo);
	printk("data  is %d\n", foo);
	return count;
}

//we associate our useful attribute with an abstract attribute object 
//we also associate a show method and store method with our 
//useful attribute object - together they are effective !!!
//
//in this context, following macros set-up  specific 
//struct kobj_attribute{ } instances
//
//as part of this, struct attribute{} fields are initialized
//name string  and multi-user access permissions are provided
//the name and access permissions are passed on to the 
//corresponding sysfs files
//  
//in addition, one show call back and one store call 
//back are registered, along with the specific attribute 
//object - we are supposed to provide appropriate 
//call backs, as per frame-works and rules
// 
//in the sysfs access permissions to attribute/sysfs regular files,
//the system may complain about 0666 and will support 
//0600 or 0644 or 0640 or 0604 - this could be for secure 
//access permissions   
static struct kobj_attribute foo_attribute =
	//__ATTR(foo, 0666, foo_show, foo_store);
//	__ATTR(foo, 0600, foo_show, foo_store);
	__ATTR(foo, 0644, foo_show, foo_store);

/*
 * More complex function where we determine which varible is being accessed by
 * looking at the attribute for the "baz" and "bar" files.
 */
static ssize_t b_show(struct kobject *kobj, struct kobj_attribute *attr,
		      char *buf)
{
	int ret;
        dump_stack();

	if (strcmp(attr->attr.name, "baz") == 0)
	ret=	sprintf(buf, "%s\n", baz);
	else
		
	ret= sprintf(buf, "%s\n", bar);

return ret;
}

static ssize_t b_store(struct kobject *kobj, struct kobj_attribute *attr,
		       const char *buf, size_t count)
{
	int var;
        dump_stack();

	
	if (strcmp(attr->attr.name, "baz") == 0)
		sscanf(buf, "%s", baz);

	else
		sscanf(buf, "%s", bar);

	return count;
}

static struct kobj_attribute baz_attribute =
	__ATTR(baz, 0664, b_show, b_store);
//	__ATTR(baz, 0666, b_show, b_store);
static struct kobj_attribute bar_attribute =
	__ATTR(bar, 0664, b_show, b_store);

//the following objects are used to manage the attributes declared above 
//this is one way of setting up attributes - we will see another 
//method, in the next example !!!
/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
//
//we are collecting all the abstract attribute objects of specific
//attribute objects, before registering
//
//most of the coding and data types are pre-defined, by 
//kernel frame-works 
//
static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval=0;

	/*
	 * Create a simple(dummy) kobject with the name of "kobject_example",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	
         */
        //we may use kobjects directly or we may use kobjects indirectly - 
        //we may use kobjects indirectly, using other objects of device model ??


        //here, a kobject{} is created, initialized , registered with 
        //unified device(driver) model(a subsystem) and also registered with 
        //sysfs(another subsystem) 
        //initially, understand the relationship with unified device model
        //and once clear, try to understand the relationship with 
        //sysfs pseudo fs !!!

        //in this example, we are not using a kset(another low-level 
        //kernel object, but another kobject{} is
        //used as the parent - in this case, kernel_kobj is ptr to 
        //"a system's kobject, which may be used for experimental and 
        //testing purposes" - this is one of the scenarios - in the 
        //next example(kset_example.c), 
        //we will see use of a kset{} along with our 
        //kobject !!
        //in further code samples/scenarios, we will use other kobject{}s
        //, as parents  

        //refer to <ksrc>/lib/kobject.c and <ksrc>/include/linux/kobject.h 
        //in this system API, p1 is the name of the new kobject to be
        //created - p2 will be ptr to a parent kobject - the parent 
        //kobject can be a standalone kobject or a kobject inside 
        //another kobject,like kset - there are different possible 
        //designs
        //in this context, we are passing ptr to a system's  kobject's 
        //pointer 
        //in this context, parent of our kobject will be a system kobject - 
        //second parameter ??
        //this system API creates a new kobject(1) - 
        //initializes the kobject(2) - 
        //registers with device model(3) - 
        //registers with sysfs(4) ??
        //if this system API is successful, a new kobject is created
        //and registered with device model and sysfs - "we will get 
        //a ptr to the newly created kobject" - this will be used
        //as a handle to other system APIs  

	example_kobj = kobject_create_and_add("kobject_example2", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM; //kobject is typically allocated from 
                                //a slab allocator and due to low physical 
                                //memory scenario, there may be a failure 
                                

	/* Create the files associated with this kobject */
        //with the help of sysfs_create_group() system API, system will
        //create one sysfs regular file per attribute passed via 
        //attr_group{}(param2) under sysfs directory file /entry of 
        //kobject {} ( param1) !!!

        //there will be  sysfs_dirent{} objects corresponding 
        //to regular files of attributes maintained, in the sysfs  ??
        //hint: they will be maintained under a specific 
        //sysfs_dirent{} object of kobject, in the sysfs !! which one ???
        	
        //in this case, regular sysfs files are created for one or 
        //more attributes - these are created under kobject's sysfs directory 
        //what is the use of these regular sysfs files ??

        //when we are registering our attributes of our specific 
        //attributes, with device model/sysfs. 
        //we are also registering our specific 
        //attributes' call backs - show()/store()
        //
        //when we access our sysfs regular file of our specific
        // attributes, we are 
        //eventually accessing our attributes/kobject/call backs - 
        //this is what enables us to actually take actions, when a read call 
        //and a write call are invoked on an attribute's sysfs file ??
        //as part of this set-up and access, we still need to 
        //set-up an active file of a sysfs regular file, using 
        //open() system call API 

        //these call backs have strict prototypes - these call backs are 
        //passed certain system objects/private objects - 
        //these objects help us do  productive 
        //work - these objects enable us to write re-entrant call backs, which 
        //can be effective for several device instances,concurrently, 
        //in several kernel execution paths  ?? these kernel executions
        //paths correspond to several threads of execution of 
        //one or more processes - much depends on the user-space 
        //application/code, as well  
        //
        //assignment requires following changes :
        //
        //you need to add 2 more attributes - in this case, you must 
        //use sysfs_create_file() API for registering attributes
        //
        //we need to create 2 more attributes with appropriate names/show/store
        //you need to use a single set of show/store between the 2 attributes 
        //
        //code, build, and test the attributes - check call trace,using dump stack ??
        //p1 will be ptr. to the kobject of this device instance
        //p2 will be containing multiple attributes
        //this is a sysfs API  
        retval = sysfs_create_group(example_kobj, &attr_group);
	//if (retval)
	//	kobject_put(example_kobj);


         //we may use sysfs_create_file(), if we just need to manage 
        //few attributes, under a kobject 
         //sysfs_create_file(example_kobj, &new_attribute1.attr);
         //sysfs_create_file(example_kobj, &new_attribute2.attr);


	return retval;
}

static void __exit example_exit(void)
{

        //this decreases the usage count of the respective kobject {} and
        //if the count drops to 0, free the container object and its kobject,
        //using destructor  !!!
        //in this case, destructor is provided by the core device 
        //model/layer - implicitly, taken care  !!!

	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
