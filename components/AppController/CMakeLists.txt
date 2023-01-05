
cmake_minimum_required(VERSION 3.5)

set(SRCS
   AppController.cpp
   AppControllerUI/AppControllerUI.c
   AppControllerUI/AppLoading.c  
)



set(INCLUDE
   AppControllerUI
   .
)



set(COMPONENT_REQUIRES
    Applications
    lvgl
    MYDriver
    mpu6050
)



idf_component_register(SRCS  "${SRCS}"
                    INCLUDE_DIRS "${INCLUDE}" 
                    REQUIRES "${COMPONENT_REQUIRES}" )
