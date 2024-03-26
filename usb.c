#include <fcntl.h>
#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *usbidx = "/usr/share/hwdata/usb.ids";

int main(void) {
  libusb_device **list_device;
  size_t device_num;
  char vendorName[64] = "", productName[64] = "";
  char cmd[128] = "";
  FILE *usbids, *productids;
  int rc;

  if ((rc = libusb_init(NULL)) < 0) {
    printf("Failed initializing libusb\n");
    return -1;
  }

  if ((device_num = libusb_get_device_list(NULL, &list_device)) < 0) {
    printf("Failed getting device list\n");
    return -1;
  }

  for (int i = device_num - 1; i >= 0; i--) {
    libusb_device *device = list_device[i];
    struct libusb_device_descriptor desc;
    if ((rc = libusb_get_device_descriptor(device, &desc)) < 0) {
      printf("Failed getting descriptor\n");
      return -1;
    }

    sprintf(cmd, "grep \"^%04x\" %s  | sed -r s/%04x// | tr -d '\t|\n'",
            desc.idVendor, usbidx, desc.idVendor);
    usbids = popen(cmd, "r");

    while (fgets(vendorName, 64, usbids) != NULL)
      ;

    sprintf(cmd,
            "grep -A999 -P \"^%04x\" %s|grep -P \"\t%04x\" -m1|sed "
            "s/%04x//|tr -d "
            "'\t|\n'",
            desc.idVendor, usbidx, desc.idProduct, desc.idProduct);
    productids = popen(cmd, "r");

    while (fgets(productName, 64, productids) != NULL)
      ;

    pclose(productids);
    pclose(usbids);

    printf("Bus %03d Dev %03d: ID %04x:%04x%s%s\n",
           libusb_get_bus_number(device), libusb_get_device_address(device),
           desc.idVendor, desc.idProduct, vendorName, productName);

    memset(vendorName, 0, 64);
    memset(productName, 0, 64);
  }

  libusb_free_device_list(list_device, 1);
  return 0;
}