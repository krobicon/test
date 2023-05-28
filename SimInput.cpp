#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <linux/uinput.h>

class SimInput
{
   private:
       struct uinput_setup usetup;
       int fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
   
   public:
       void setup(){
          ioctl(fd, UI_SET_EVBIT, EV_KEY);
          ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);

          ioctl(fd, UI_SET_EVBIT, EV_REL);
          ioctl(fd, UI_SET_RELBIT, REL_X);
          ioctl(fd, UI_SET_RELBIT, REL_Y);
          
          memset(&usetup, 0, sizeof(usetup));
          usetup.id.bustype = BUS_USB;
          usetup.id.vendor = 0x1234; /* sample vendor */
          usetup.id.product = 0x5678; /* sample product */
          strcpy(usetup.name, "Example device");

          ioctl(fd, UI_DEV_SETUP, &usetup);
          ioctl(fd, UI_DEV_CREATE);
       }
      void emit(int type, int code, int val)
      {
         struct input_event ie;

         ie.type = type;
         ie.code = code;
         ie.value = val;
         /* timestamp values below are ignored */
         ie.time.tv_sec = 0;
         ie.time.tv_usec = 0;

         write(fd, &ie, sizeof(ie));
      }
   
      void click() {
         emit(fd, EV_KEY, BTN_LEFT, 1);
         emit(fd, EV_SYN, SYN_REPORT, 0);
         emit(fd, EV_KEY, BTN_LEFT, 0);
         emit(fd, EV_SYN, SYN_REPORT, 0);
      }
};

// input to angle values for 1.9 sens at 110 fov
// MOVING 100 PIXELS DOWN IN Y DIRECTION -> pitch += 4.2405
// MOVING 100 PIXELS RIGHT IN x DIRECTION -> yaw -= 4.2404
// 1 pixel ~ .0424

