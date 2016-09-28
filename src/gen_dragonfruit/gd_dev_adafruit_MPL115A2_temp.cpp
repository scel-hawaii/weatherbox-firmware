#include "gd_dev_adafruit_MPL115A2_temp.h"
#include "Adafruit_MPL115A2.h"

static Adafruit_MPL115A2 mpl115a2t1;

void gd_dev_adafruit_MPL115A2_temp_open(void){
    mpl115a2t1.begin();
}

long gd_dev_adafruit_MPL115A2_temp_read(void){
  long value = 0;
  value = mpl115a2t1.getTemperature()*10;
  return value;
}
