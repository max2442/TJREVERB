#include <libserialport.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BAUD_RATE   19200
#define BUFF_SIZE   512

unsigned char replay_array[] =
{0x54, 0x45, 0x53, 0x54, 0x0D};

struct sp_port *port;
char *serial_port_name;

FILE *file;



void
write_array(char *message)
{
    int ret = sp_nonblocking_write(port, message, strlen(message));
    printf("Number of bytes sent: %d\n", ret);
	  fprintf(file, "Sent: ");
    for(unsigned int i=0 ; i <  strlen(message) ; i++){
		fprintf(file, "%c", message[i]);
		printf("%c", message[i]);
    }

    if (ret < 0) {
        fprintf(stderr, "Unable to write to serial port %s\n", serial_port_name);
		    fprintf(file, "\nFAILED: unable to write to serial port");
	  }
	fprintf(file, "\n");
}

void
print_buffer(unsigned char *byte_buff, int num_read) {
    fprintf(file, "Received: ");
    for (int i = 0; i < num_read; i++){
        //printf("%c" , byte_buff[i]);
		fprintf(file, "%c", byte_buff[i]);
    }
    fprintf(file, "\n");
}

void
print_usage(char *command)
{
    fprintf(stderr, "Usage: %s <serial device>\n", command);
}

void
print_banner()
{
    puts("       TJREVERB CubeSat 2018              ");
    puts(" ______   _____  _____   _________  ___   ");
    puts("/_  __/_ / / _ \\/ __/ | / / __/ _ \\/ _ )");
    puts(" / / / // / , _/ _/ | |/ / _// , _/ _  |  ");
    puts("/_/  \\___/_/|_/___/ |___/___/_/|_/____/  ");
    puts("");
    puts("Thomas Jefferson HS & George Mason Univ.  ");
    printf("Version 0.1 compiled %s %s\n", __DATE__, __TIME__);
}
int main(int argc, char **argv)
{
    file = fopen("src/log.txt", "a");
    if(file == NULL){
		printf("error opening file \n");
    }
    unsigned char byte_buff[BUFF_SIZE] = {0};

    int bytes_waiting = 0;
    int num_read = 0;

    print_banner();

    if (argc < 2) {
        print_usage(argv[0]);
        return -1;
    }
    serial_port_name = argv[1]; // WARNING, No error checking here

    enum sp_return ret = sp_get_port_by_name(serial_port_name, &port);
    if (ret != SP_OK) {
        fprintf(stderr, "Unable to get serial port %s\n", serial_port_name);
        return -1;
    }

    ret = sp_open(port,SP_MODE_READ_WRITE);
    if (ret != SP_OK) {
        fprintf(stderr, "Unable to open serial port %s\n", serial_port_name);
        return -1;
      }
    /*ret = sp_open(port,SP_MODE_WRITE);
    if (ret != SP_OK) {
        fprintf(stderr, "Unable to open serial port %s\n", serial_port_name);
        return -1;
    }*/

    ret = sp_set_baudrate(port,BAUD_RATE);
    if (ret != SP_OK) {
        fprintf(stderr, "Unable to set the baud rate to: %d\n", BAUD_RATE);
        return -1;
    }
    //MODE CHOOSING HERE
    if (strcmp(argv[2],"heartbeat")==0)
    {
      printf("HEARTBEAT MODE\n");
      while(1) {
          bytes_waiting = sp_input_waiting(port);
          if (bytes_waiting > 0) {
              num_read = sp_nonblocking_read(port,byte_buff, sizeof byte_buff);
              print_buffer(byte_buff,num_read);
              printf("Sending message \n");
              write_array(("heartbeat response %ld \n" __TIME__));
              sleep(2);
          }
 	  //sp_flush(port, bytes_waiting);

      }
    }
    if (strcmp(argv[2],"beacon")==0)
    {
      time_t rawtime;
      //struct tm * timeinfo;
      time ( &rawtime );

      long int oldtime = rawtime;

      printf("BEACON MODE\n");
      while(1) {

        ///char testnum[5]
        //
        time ( &rawtime );
        printf("%ld",rawtime);
        printf("\n");
        //long int newtime = rawtime + 5;
        printf("%ld", oldtime);
        printf("\n");
        //printf("%ld", newtime - 5);
        //timeinfo = localtime ( &rawtime );
        //printf ( "Current local time and date: %s", asctime (timeinfo) );
          bytes_waiting = sp_input_waiting(port);

          if (bytes_waiting > 0) {
              num_read = sp_nonblocking_read(port,byte_buff, sizeof byte_buff);
              print_buffer(byte_buff,num_read);
          }
          printf("Sending message \n");
          write_array(("beacon pulse %s \n" __TIME__));
          sleep(5);
          //printf("Waiting\n");
      }
    }
    if (strcmp(argv[2],"command")==0)
    {
      printf("COMMAND MODE\n");
      while(1) {
          bytes_waiting = sp_input_waiting(port);
          if (bytes_waiting > 0) {
              num_read = sp_nonblocking_read(port,byte_buff, sizeof byte_buff);
              print_buffer(byte_buff,num_read);
          }
          printf("Sending message \n");
          char s1[500];
          printf("Enter command: ");
          int i = scanf("%s",s1);
          printf("Sending command: %s\n",s1);
          printf("%i\n",i);
          printf("%s\n",s1);
          strcat(s1,"\n");
          write_array(s1);
          sleep(10);
          //printf("Waiting\n");
      }
    }
    if (strcmp(argv[2],"time")==0)
    {
          FILE *fp;
          FILE *fw;
          char str[255];
          char* filename = "time.txt";

          fp = fopen(filename,"r");
          fw = fopen("timewrite.txt","w");
          //while(1){
          fgets(str,255,fp);

          printf("%s",str);
          printf("%lu\n",(unsigned long) strtoul(str,NULL,10));
          fprintf(fw,"%lu\n",(unsigned long) strtoul(str,NULL,10));
          fclose(fp);
          while(1){


            time_t t;

            t = time(NULL);
            if((unsigned long)t == (unsigned long) strtoul(str,NULL,10)){

              //send message
              return 1;
            }
            fprintf(stdout, "%lu\n", (unsigned long)t);
            //fprintf(fw, "%lu\n", (unsigned long)t);
            sleep(1);
          }
          fclose(fw);
	}
  //fclose(fw);
        //while(1) {

          //printf("Waiting\n");
      }

    while (0) {

        sleep(2);
    }

    return 0;
}
