#include <stdio.h>
#include <systemd/sd-device.h>

#define FOREACH_DEVICE(enumerator, device)                                     \
  for (device = sd_device_enumerator_get_device_first(enumerator); device;     \
       device = sd_device_enumerator_get_device_next(enumerator))

int main(void) {
  sd_device_enumerator *enumerator = NULL;
  sd_device *device = NULL;
  size_t device_num;

  if (sd_device_enumerator_new(&enumerator) < 0) {
    printf("Failed to create device enumerator\n");
    return -1;
  }

  if (sd_device_enumerator_add_match_subsystem(enumerator, "block", 1) < 0) {
    printf("Failed to add match subsystem\n");
    return -1;
  }

  if (sd_device_enumerator_add_match_property(enumerator, "DEVTYPE",
                                              "partition") < 0) {
    printf("Failed to add match property\n");
    return -1;
  }

  FOREACH_DEVICE(enumerator, device) {
    const char *devname = NULL;
    const char *UUID = NULL;
    const char *FS = NULL;

    if (sd_device_get_property_value(device, "DEVNAME", &devname) < 0) {
      printf("Failed to get devname\n");
    }

    if (sd_device_get_property_value(device, "ID_FS_UUID", &UUID) < 0) {
      printf("Failed to get UUID\n");
    }

    if (sd_device_get_property_value(device, "ID_FS_TYPE", &FS) < 0) {
      printf("Failed to get FS\n");
    }

    printf("Device: %s, FS: %s\tUUID: %s\n", devname, FS, UUID);
  }
  sd_device_unref(device);
  sd_device_enumerator_unref(enumerator);

  return 0;
}