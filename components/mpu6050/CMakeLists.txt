idf_component_register(SRCS "MPU6050.cpp" "MPU.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES I2Cdev AppController
)