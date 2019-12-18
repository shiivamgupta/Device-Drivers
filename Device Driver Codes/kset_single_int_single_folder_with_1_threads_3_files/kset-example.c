/*
 * Sample kset and ktype implementation
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
#include <linux/slab.h>
/*
 * This module shows how to create a kset in sysfs called
 * /sys/kernel/kset-example
 * Then three kobjects are created and assigned to this kset, "foo", "baz",
 * and "bar".  In those kobjects, attributes of the same name are also
 * created and if an integer is written to these files, it can be later
 * read out of it.
 */


/*
 * This is our "object" that we will create a few of and register them with
 * sysfs.
 */

//a good example - kobject{} is always embedded in a container object,
//seldom used stand alone !!!
//typically kobject is part of a larger object - say, device object or 
//driver object 
//
//in this case, we say that the larger object containing the 
//kobject is known as a container object 
//during run-time, using kobject, we can access the container object 
//
//same way, during run-time, we can access kobject{}, 
//using the container object 
//

struct foo_obj {
	struct kobject kobj;
	int foo;
	int baz;
	int bar;
};

//following is a good example, for using "container_of" macro 
//we have a pointer to a kobject{}(x), in a foo_obj{} 
//in addition, we know the data type of foo_obj{}
//we also know the field name of kobj, in foo_obj{}
//
//based on the above, container_of() macro can be used, 
//in a Linux system, along with gcc compiler
// 
//ret = container_of(p1, p2, p3); will return pointer to 
//the corresponding foo_obj{} instance 
//in this context, 
//p1 is set to ptr to kobject{} instance, in a foo_obj{}
//p2 is set to data type of foo_obj{}
//p3 is set to field name of kobject{}, in the foo_obj{}
//
//ret will be the pointer returned to foo_obj{}/dummy 
//device object/container object, in this case 

#define to_foo_obj(x) container_of(x, struct foo_obj, kobj)

/* a custom attribute that works just for a struct foo_obj. */
//our abstract custom attribute uses abstract basic attribute !!
//
//we will create several instances of this abstract custom attribute -
//each such instance may represent an attribute of a device type !!!

struct foo_attribute {
	struct attribute attr;
	ssize_t (*show)(struct foo_obj *foo, struct foo_attribute *attr, char *buf);
	ssize_t (*store)(struct foo_obj *foo, struct foo_attribute *attr, const char *buf, size_t count);
};
//
//following is a good example, for using container_of 
//we have a pointer to a basic attribute, in a device 
//specific foo_attribute{} 
//in addition, we know the data type of foo_attribute{}
//we also know the field name of basic attribute, in 
//foo_attribute{}
//
//ret = container_of(p1, p2, p3); will return pointer to 
//the corresponding foo_attribute{} instance 
//in this context, 
//p1 is set to ptr to basic attribute{} instance, 
//in a foo_attribute{} instance
//p2 is set to data type of foo_attribute{}
//p3 is set to field name of attribute{}, 
//in the foo_attribute{}
//
//ret value will contain pointer to a foo_attribute{} 
//instance,which is a device specific attribute object 
#define to_foo_attr(x) container_of(x, struct foo_attribute, attr)

/*
 * The default show function that must be passed to sysfs.  This will be
 * called by sysfs for whenever a show function is called by the user on a
 * sysfs file associated with the kobjects we have registered.  We need to
 * transpose back from a "default" kobject to our custom struct foo_obj and
 * then call the show function for that specific object.
 */
//this is a sysfs_ops{} call back invoked, when read() system call API 
//is invoked - this call back is passed corresponding kobject(7) 
//and basic attribute object(8) - in addition, third parameter is pointer 
//to a kernel-space buffer/intermediate buffer, which will 
//be used to copy the 
//contents, from system space to user-space,eventually 
//in this context, we must copy data into this kernel buffer
//higher layers will do the job of copying this 
//kernel buffer's data into the user-space buffer
//
//the coding of this method/call back is based 
//on the frame-works and provided by core developers
//
//
//
//the parameters passed to call backs will be 
//low-level objects of the device model, but 
//driver code/call backs need device specific/
//driver specific objects/private objects - so,
//container_of techniques are used 
//
 
static ssize_t foo_attr_show(struct kobject *kobj,struct attribute *attr,char *buf)
{
	struct foo_attribute *attribute;
	struct foo_obj *foo;

        //dump_stack();
        //based on the basic attribute object(8), 
        //extract the device specific attribute object(9) of 
        //the device instance 
	attribute = to_foo_attr(attr);
        //based on the kobject{}(7), extract the specific device object/
        //container object(12)  
        //of the device instance 
	foo = to_foo_obj(kobj);

        //check, if there is a device specific attribute's show()  
	if (!attribute->show)
		return -EIO;
        //finally, invoke the call back of the device specific 
        //attribute object and 
        //pass private device object, 
        //device specific attribute object , and ptr to buffer,
        //as parameters 
	return attribute->show(foo, attribute, buf);
}

/*
 * Just like the default show function above, but this one is for when the
 * sysfs "store" is requested (when a value is written to a file.)
 */
static ssize_t foo_attr_store(struct kobject *kobj,struct attribute *attr,const char *buf, size_t len)
{
	struct foo_attribute *attribute;
	struct foo_obj *foo;

        //dump_stack();
	attribute = to_foo_attr(attr);
	foo = to_foo_obj(kobj);

	if (!attribute->store)
		return -EIO;

	return attribute->store(foo, attribute, buf, len);
}

/* Our custom sysfs_ops that we will associate with our ktype later on */
static struct sysfs_ops foo_sysfs_ops = {
	.show = foo_attr_show,
	.store = foo_attr_store,
};

/*
 * The release function for our object.  This is REQUIRED by the kernel to
 * have.  We free the memory held in our object here.
 *
 * NEVER try to get away with just a "blank" release function to try to be
 * smarter than the kernel.  Turns out, no one ever is...
 */
static void foo_release(struct kobject *kobj)
{
        dump_stack();

	struct foo_obj *foo;
        //using ptr to a kobject, we are extracting pointer
        //to a private object
	foo = to_foo_obj(kobj);
	kfree(foo);
        //we may free other resources, if any allocated, for this 
        //object
}

/*
 * The "foo" file where the .foo variable is read from and written to.
 */
static ssize_t foo_show(struct foo_obj *foo_obj, struct foo_attribute *attr,char *buf)
{
        dump_stack(); 
	return sprintf(buf, "%d\n", foo_obj->foo);
}

static ssize_t foo_store(struct foo_obj *foo_obj, struct foo_attribute *attr,const char *buf, size_t count)
{

        dump_stack(); 
	sscanf(buf, "%du", &foo_obj->foo);
	return count;
}
//
//typically, struct attribute{} is an abstract object 
//struct attribute{} is not used as a stand-alone object, 
//but built into another custom/specific attribute object
//in this case, built into foo_attribute{} object 
//
//foo_attribute{} contains certain call backs, in addition
//to struct attribute{} object - it may contain other
//fields, if needed
//
//typically struct attribute{} is what is registered 
//with the system and based on it pointer, which may 
//be passed to one or more call backs, we can use 
//container_of() to extract the pointer of the actual 
//container object 
//
//in addition, struct attribute{} contains a name field, 
//which is used to set-up the name of the corresponding 
//sysfs regular file 
//
//in addition, struct attribute{} contains permissions 
//field, which is used to set the access permissions of 
//corresponding sysfs regular file - based on this 
//and based on the credentials of the process/pd/td
//accessing, the actual access permissions of the 
//sysfs regular file/struct attribute{} will be 
//utilized /applied  
//
static struct foo_attribute foo_attribute = __ATTR(dev_param1, 0664, foo_show, foo_store);

/*
 * More complex function where we determine which varible is being accessed by
 * looking at the attribute for the "baz" and "bar" files.
 */
static ssize_t b_show(struct foo_obj *foo_obj, struct foo_attribute *attr,char *buf)
{
	int var;
        dump_stack();

	if (strcmp(attr->attr.name, "dev_param2") == 0)
		var = foo_obj->baz;
	else
		var = foo_obj->bar;
	return sprintf(buf, "%d\n", var);
}

static ssize_t b_store(struct foo_obj *foo_obj, struct foo_attribute *attr,const char *buf, size_t count)
{
	int var;

        dump_stack();
	sscanf(buf, "%du", &var);
	if (strcmp(attr->attr.name, "dev_param2") == 0)
		foo_obj->baz = var;
	else
		foo_obj->bar = var;
	return count;
}

static struct foo_attribute baz_attribute = __ATTR(dev_param2, 0664, b_show, b_store);
static struct foo_attribute bar_attribute = __ATTR(dev_param3, 0664, b_show, b_store);

/*
 * Create a group of attributes so that we can create and destory them all
 * at once.
 */
static struct attribute *foo_default_attrs[] = {
	&foo_attribute.attr,
	&baz_attribute.attr,
	&bar_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * Our own ktype for our kobjects.  Here we specify our sysfs ops, the
 * release function, and the set of default attributes we want created
 * whenever a kobject of this type is registered with the kernel.
 */
static struct kobj_type foo_ktype = {
	.sysfs_ops = &foo_sysfs_ops,    //these are the methods 
                                        //invoked when our sysfs regular files
	.release = foo_release,         //are accessed 
	.default_attrs = foo_default_attrs,
};

static struct kset *example_kset;
static struct foo_obj *foo_obj;
static struct foo_obj *bar_obj;
static struct foo_obj *baz_obj;

//here we create several devices/objects and register their corresponding 
//kobjects under kset - this will register our devices' kobjects under
//device model and sysfs ??? 

static struct foo_obj *create_foo_obj(const char *name)
{
	struct foo_obj *foo;
	int retval;

	/* allocate the memory for the whole object */
	//in this line, we are creating a private device object/container
        //object  with an embedded kobject
        //this is how, we manage device objects in device model - subsystems
        //do this creation and initialization 
        foo = kzalloc(sizeof(*foo), GFP_KERNEL);
	if (!foo)
		return NULL;

	/*
	 * As we have a kset for this kobject, we need to set it before calling
	 * the kobject core.
	 */

        //if we are using a kset for managing our kobjects of our devices, 
        //we must point our kobjects to corresponding kset

        //most of these steps are done, as per the rules of 
        //frame-works  
	foo->kobj.kset = example_kset;

	/*
	 * Initialize and add the kobject to the kernel.  All the default files
	 * will be created here.  As we have already specified a kset for this
	 * kobject, we don't have to set a parent for the kobject, the kobject
	 * will be placed beneath that kobject of the kset automatically.
	 */
        //
        //in this context, kobject{} instance is part of our
        //private device object/container object - in this case, 
        //we not allocating a kobject, explicitly, but need to 
        //initialize, before registration
        //
        //in this case, kobject does not have a parent - however, 
        //kset's kobject will serve as the parent  - in addition, 
        //we also add a kobj_type {}  to our kobjects 
        //
        //
        //kobj_type{} provides methods for sysfs regular file access - 
        //use dump_stack to understand how our kobj_type{} methods are
        //invoked ??
        //
        //using kobj_type{}, we can also register default attributes 
        //for our device under kobject{} - corresponding sysfs files 
        //will be created 
        //when we access our attributes' sysfs files, the first call backs that 
        //are invoked belong to kobj_type{} 
        //
        //it is the responsibility of kobj_type{}'s sysfs_ops to invoke 
        //show/store methods of our attributes 
        //
        //in this context, we must understand sysfs_ops, in order to code 
        //our show()/store methods of our attributes 
        //
	retval = kobject_init_and_add(&foo->kobj, &foo_ktype, NULL, "%s", name);
	if (retval) {
		kfree(foo);
		return NULL;
	}

	/*
	 * We are always responsible for sending the uevent that the kobject
	 * was added to the system.
	 */
	//here, the uevent may not have any impact on the system 
        //kobject_uevent(&foo->kobj, KOBJ_ADD);
        //we are ignoring certain notifications to user-space
        //however, we may enable such notifications, in real-world
        //code 

        //for the assignment ??

        //in this context, create our own new attribute and show/store methods
        //
        //use sysfs_create_file() to create a new attribute under each device - however, 
        //show/store must be common ???
        //
        //test the sysfs regular files of new attribute - understand the call trace ?? 
        //what is the role of kobj_type{} xxx_sysfs_ops ???

        //sysfs_create_file(&foo->kobj, &new_attribute.attr);
	return foo;
}

static void destroy_foo_obj(struct foo_obj *foo)
{
	kobject_put(&foo->kobj);
}

static int example_init(void)
{
	/*
	 * Create a kset with the name of "kset_example",
	 * located under /sys/kernel/
	 */

        //in this case, our kset is created and registered with 
        //LDDM and sysfs - however, kobject of the kset is actually 
        //used for such registrations and other fields of kset will 
        //be used for managing kobjects grouped under this kset and 
        //other run-time activities !!!

        //internally, a kset is managed using its kobject - most of th e
        //rules of kobject apply - in addition, we can register other kobjects
        //under this kset - this is a form of grouping and hierarchy
        //
        //p1 is just the name assigned to the kobject of the kset 
        //p3 is the system kobject, which will be the parent kobject
        //of the kset's kobject  
        //we are not using p2, in this case
	example_kset = kset_create_and_add("kset_devices_typeA", 
                                            NULL, kernel_kobj);
	if (!example_kset)
		return -ENOMEM;

	/*
	 * Create three device objects(representing devices) and 
           register them with our kset,using respective kobjects !!!
	 */
        //
        //we are creating three dummy device objects/container
        //objects,  containing 
        //kobjects and registering these kobjects, with the 
        //kset of this module/component 
        //since a kobject{} is part of the dummy device object, 
        //effectively, the device object is registered
        //
        //many of these design/frame-work issues will be clear,
        //in the call backs of attributes 
        //
        //we can modify similar code to support, 
        //as many device instances, for our 
        //embedded requirements
        // 
	foo_obj = create_foo_obj("device0");
	if (!foo_obj)
		goto foo_error;

	bar_obj = create_foo_obj("device1");
	if (!bar_obj)
		goto bar_error;

	baz_obj = create_foo_obj("device2");
	if (!baz_obj)
		goto baz_error;

	return 0;

baz_error:
	destroy_foo_obj(bar_obj);
bar_error:
	destroy_foo_obj(foo_obj);
foo_error:
	return -EINVAL;
}

static void example_exit(void)
{
	destroy_foo_obj(baz_obj);
	destroy_foo_obj(bar_obj);
	destroy_foo_obj(foo_obj);
	kset_unregister(example_kset);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("cdac");
