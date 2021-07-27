 #ifndef H_SP_GPIO
 #define H_SP_GPIO

int spxmit_io_request(const char* dev, int pin); 
int spxmit_io_setpin(int fd, int state);
void spxmit_io_close(int fd);

 #endif