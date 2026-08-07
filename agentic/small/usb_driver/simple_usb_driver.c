/*
 * Simple USB Device Driver - Basic USB Storage Driver
 * This is a minimal USB driver that demonstrates USB device detection
 * and basic file operations over USB mass storage
 *
 * Compile: make
 * Load: sudo modprobe simple_usb_storage
 * Remove: sudo rmmod simple_usb_storage
 */

#define _GNU_SOURCE
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/skbuff.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/seq_file.h>

#define DRIVER_NAME "simple_usb_storage"
#define DRIVER_VERSION "1.0"

/* Device ID matching */
static const struct usb_device_id simple_usb_id[] = {
    {USB_DEVICE(0x0b05, 0x9614)}, /* Silicon Labs CP210x */
    {USB_DEVICE(0x1a86, 0x7523)}, /* Prolific PL2303 */
    {USB_DEVICE(0x1a86, 0x7525)}, /* Prolific PL2303H */
    { },
};
MODULE_DEVICE_TABLE(usb, simple_usb_id);

static int simple_usb_probe(struct usb_interface *intf, 
                            const struct usb_device_id *id);
static void simple_usb_disconnect(struct usb_interface *intf);
static int simple_usb_suspend(struct usb_interface *intf, 
                              pm_message_t status);
static int simple_usb_resume(struct usb_interface *intf);

static struct usb_driver simple_usb_driver = {
    .name    = DRIVER_NAME,
    .id_table = simple_usb_id,
    .probe   = simple_usb_probe,
    .disconnect = simple_usb_disconnect,
    .suspend = simple_usb_suspend,
    .resume  = simple_usb_resume,
};

static int simple_usb_probe(struct usb_interface *intf, 
                            const struct usb_device_id *id)
{
    int ret = -ENOMEM;
    
    pr_info("%s: Probe called\n", DRIVER_NAME);
    
    /* Iterate over all altsettings to find bulk endpoints */
    int i;
    for (i = 0; i < intf->cur_altsetting->desc.bNumEndpoints; i++) {
        /* For now, just check the current active altsetting */
        struct usb_host_interface *altsetting = intf->cur_altsetting;
        
        pr_info("%s: altsetting, bNumEndpoints=%d\n", 
                DRIVER_NAME, altsetting->desc.bNumEndpoints);
        
        /* Find bulk endpoint for storage */
        for (int j = 0; j < altsetting->desc.bNumEndpoints; j++) {
            struct usb_host_endpoint *ep = &altsetting->endpoint[j];
            
            if (ep->bEndpointType == USB_ENDPOINT_XFER_BULK) {
                pr_info("%s: Found bulk endpoint %d for storage\n", DRIVER_NAME, j);
                ret = 0;
                break;
            }
        }
        if (ret == 0)
            break;
    }
    
    if (ret) {
        pr_err("%s: No bulk endpoint found\n", DRIVER_NAME);
        return -ENODEV;
    }
    
    pr_info("%s: Device successfully probed\n", DRIVER_NAME);
    return 0;
}

static void simple_usb_disconnect(struct usb_interface *intf)
{
    pr_info("%s: Disconnect called\n", DRIVER_NAME);
}

static int simple_usb_suspend(struct usb_interface *intf, 
                             pm_message_t status)
{
    pr_info("%s: Suspend called\n", DRIVER_NAME);
    return 0;
}

static int simple_usb_resume(struct usb_interface *intf)
{
    pr_info("%s: Resume called\n", DRIVER_NAME);
    return 0;
}

static int __init simple_usb_init(void)
{
    pr_info("%s: Module loaded\n", DRIVER_NAME);
    return usb_register(&simple_usb_driver);
}

static void __exit simple_usb_exit(void)
{
    pr_info("%s: Module unloaded\n", DRIVER_NAME);
    usb_deregister(&simple_usb_driver);
}

module_init(simple_usb_init);
module_exit(simple_usb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simple USB Driver");
MODULE_DESCRIPTION("Simple USB Device Driver");
MODULE_VERSION(DRIVER_VERSION);
