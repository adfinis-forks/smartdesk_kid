#include "stm32f10x.h"
#include "main.h"
#include <stdio.h>
#include "AddonBoard.h"
#include "stm32f10x_rcc.h"

int main(){
  AddonBoard.Init();
  while (1){
    AddonBoard.Update();
  }
}

