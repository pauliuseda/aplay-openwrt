#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sched.h>
#include <errno.h>
#include <getopt.h>
#include <string.h> 
#include <math.h> 
#define ALSA_PCM_NEW_HW_PARAMS_API
#include <alsa/asoundlib.h>

#define SERVPORT 3333
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);
static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *params, snd_pcm_access_t access);
void udpstart(void  );
void patikrink(int  ,char* ,char*  );
static int write_loop(snd_pcm_t *handle, signed char *samples, snd_pcm_channel_area_t *areas);
static int xrun_recovery(snd_pcm_t *handle, int err);
float timedifference(time_t realtime ,time_t lasttime );


static char *device = "hw:1,0";                     /* playback device */
static snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE; /* sample format */
static unsigned int rate = 44100;                       /* stream rate */
static unsigned int channels = 2;                       /* count of channels */
static unsigned int buffer_size = 16384;               /* ring buffer length in us */
static unsigned int period_size = 256;               /* period time in us */
static double freq = 460;  
static int resample = 1;                                /* enable alsa-lib resampling */
static int period_event = 0;                            /* produce poll event after each period */

static snd_pcm_sframes_t buffer_time;
//static snd_pcm_sframes_t period_time;
static snd_output_t *output = NULL;
snd_pcm_sframes_t delayp;
snd_pcm_sframes_t availp;
  int klaidos;
	int sd, rc;
	struct sockaddr_in serveraddr, clientaddr;
	int clientaddrlen = sizeof(clientaddr);
	int serveraddrlen = sizeof(serveraddr);
	

long writebuf(snd_pcm_t *handle, char *buf, long len)
{
        long r;
        while (len > 0) {
                r = snd_pcm_writei(handle, buf, len);
                if (r == -EAGAIN)
                        continue;
                //printf("write = %li\n", r);
                if (r < 0)
                        return r;
                // showstat(handle, 0);
                buf += r * 4;
                len -= r;
              
        }
        return 0;
}


 	
int main(int argc, char *argv[])
	{
	//int a;
	
	
	//char buffer;
//	char *bufptr ;
//long loops;
 // int rc1;
  //int size;
  //unsigned int val;
  //int dir;

	//char mybuffer[44100*16*2];
	//char mybuffer2[940];

				snd_pcm_t *handle;
        			snd_pcm_hw_params_t *hwparams;
        			snd_pcm_sw_params_t *swparams;
        			signed char *samples;
        			unsigned int chn;
        			snd_pcm_channel_area_t *areas;

        			snd_pcm_hw_params_alloca(&hwparams);
        			snd_pcm_sw_params_alloca(&swparams);

        			snd_output_stdio_attach(&output, stdout, 0);

        			printf("Playback device is %s\n", device);
        			printf("Stream parameters are %iHz, %s, %i channels\n", rate, snd_pcm_format_name(format), channels);
        			//printf("Sine wave rate is %.4fHz\n", freq);

       				snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0);
       				set_hwparams(handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED);
        			set_swparams(handle, swparams);
        			samples = malloc((period_size * channels * snd_pcm_format_physical_width(format)) / 8);
        			areas = calloc(channels, sizeof(snd_pcm_channel_area_t));

        			

	udpstart();
  
	
 
/*loops = 39 ;
while (loops > 0) {
    	loops--; 
	sleep(1/(94));	*/
	
	//patikrink(rc, "UDP Server - recvfrom() error", "UDP Server - recvfrom() is OK..." );

	//printf("UDP Server received the following:\n \"%s\" message\n", bufptr);
	//printf("from port %d and address %s.\n", ntohs(clientaddr.sin_port),inet_ntoa(clientaddr.sin_addr));
	//sleep(1/(44100*2));
	//strcpy(mybuffer, bufptr);
	//}

/*printf("buferio dydis.1..%d\n",sizeof mybuffer);
 strncpy(mybuffer2,mybuffer,940);
printf("buferio dydis.1..%d\n",sizeof mybuffer);
 
printf("buferio  patikrinimasar netuscias %d\n",a );
sleep(5);*/
	//int a=0;
	/*while((a<sizeof mybuffer -sizeof mybuffer2))
	{	a++;
		mybuffer[a]=mybuffer[a +sizeof mybuffer2];

		
		} */
/* rc1 = snd_pcm_writei(handle, mybuffer, sizeof mybuffer);
if (rc1 == -EPIPE) {
      /* EPIPE means underrun */
 /*     fprintf(stderr, "underrun occurred\n");
      snd_pcm_prepare(handle);
    } else if (rc1 < 0) {
      fprintf(stderr,
              "error from writei: %s\n",
              snd_strerror(rc));
    }  else if (rc1 != (int)frames) {
      fprintf(stderr,
              "short write, write %d frames\n", rc);
    }*/
 
	
	/**printf("UDP Server replying to the stupid UDP client...\n");
	rc = sendto(sd, bufptr, size, 0, (struct sockaddr *)&clientaddr, clientaddrlen);

 	patikrink(rc, "UDP server - sendto() error", "UDP Server - sendto() is OK..." );*/
	write_loop(handle, samples, areas);

 	free(areas);
        free(samples);		
	snd_pcm_drain(handle);
	snd_pcm_close(handle);
	//free(bufptr);
	close(sd);
	exit(0);
	}

static int write_loop(snd_pcm_t *handle, signed char *samples, snd_pcm_channel_area_t *areas)
{
       double phase = 0;
        signed char *ptr;
        int err;
       int klaidos;
	time_t last_time ;
	float time=(float)period_size*1000000000/(float)rate;
	float timedif=0;
	snd_pcm_status_t *status;
     //   printf("Period Size = %ld",period_size);
	snd_pcm_status_alloca(&status);
        while (1) {
        		fflush(stdout);
			rc = recvfrom(sd, samples, period_size*4, 0, (struct sockaddr *)&clientaddr, &clientaddrlen);
//printf("Period Size = %d",sizeof samples);
			if(rc < 0)
				{	printf("UDP server - erc error");
					close(sd);
					exit(-1);
					}
			else if(rc == 0)
				{	printf("Beda");
					close(sd);
					exit(-1);
					}
               		else {	//printf("Prieme:");
                   // err = snd_pcm_writei(handle, ptr, cptr);
 		   	 		err = writebuf(handle, samples, period_size);

					if (snd_pcm_status(handle, status) < 0) {
						fprintf(stderr, "snd_pcm_status failed.\n");
					}
					else {
						snd_htimestamp_t tstamp;
						snd_pcm_status_get_htstamp(status, &tstamp);
						if(last_time > 0)
							{//printf("time: %lu %lu skirtumas %lu : %lu \n :", tstamp.tv_sec, tstamp.tv_nsec,
								//tstamp.tv_nsec-last_time,(time_t)(time));
							timedif += timedifference(  tstamp.tv_nsec, last_time )-(float)time;
	//printf("Laiko Skirtumas: %f  bendras: %f \n :", (timedifference(  tstamp.tv_nsec, last_time )) / 1000000,  timedif/1000000);
						}
						last_time=tstamp.tv_nsec;
					}
                                  //  err = snd_pcm_writei(handle, samples, period_time);
                			//snd_pcm_avail_delay(handle,&availp,&delayp);
              // printf("available frames =%ld  delay = %ld  i = %d\n",availp,delayp,i);
                       			 if (err == -EAGAIN)
                              			  {	printf("Write error:");
							}
					 if (err == -EBADFD)
                              			  {	printf("Write error:EBADFD");
							}
					 if (err == -EPIPE)
                              			  {	printf("Write error:EPIPE");
							}
					if (err == -ESTRPIPE)
                              			  {	printf("Write error:ESTRPIPE");
							}

					if (err < 0) { printf("Laiko Skirtumas: %f  \n :", timedif/1000000);
							//printf("R<0: %d ",err);
							//timedif=0;
							//last_time=0;
                             		 		 if (xrun_recovery(handle, err) < 0) {
                                        				printf("Write error: %s\n", snd_strerror(err));
                                        				exit(EXIT_FAILURE);
                              			 			 }
                            				  } 
					if (err >0) {	//printf("Irase"); 
								rc=0;
								err=0;
							}
						rc=0;
						err=0;
                      			 }rc=0;
					  err=0;
       		}
}

static int xrun_recovery(snd_pcm_t *handle, int err)
{   klaidos +=1;
printf("Klaidu skaicius: %d \n",klaidos);
        if (0)
                printf("stream recovery\n");
        if (err == -EPIPE) {    /* under-run */
                err = snd_pcm_prepare(handle);
                if (err < 0)
                        printf("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));
                return 0;
        } else if (err == -ESTRPIPE) {
                while ((err = snd_pcm_resume(handle)) == -EAGAIN)
                        sleep(1);       /* wait until the suspend flag is released */
                if (err < 0) {
                        err = snd_pcm_prepare(handle);
                        if (err < 0)
                                printf("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));
                }
                return 0;
        }
 
        return err;
}
  
 void udpstart(void )
	{ 
	patikrink((sd = socket(AF_INET, SOCK_DGRAM, 0)), "UDP server - socket() error", "UDP server - socket() is OK" );
 
	printf("UDP server - try to bind...\n");
 
	memset(&serveraddr, 0x00, serveraddrlen);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

 	patikrink((rc = bind(sd, (struct sockaddr *)&serveraddr, serveraddrlen)), "UDP server - bind() error", "UDP server - bind() is OK" );
 
	printf("Using IP %s and port %d\n", inet_ntoa(serveraddr.sin_addr), SERVPORT);
	printf("UDP server - Listening...\n");
		
		}


 void patikrink(int rc ,char* blogai,char* gerai )
	{ 
	if(rc < 0)
		{
			printf("%s \n",blogai);
			//close(sd);
			exit(-1);
			}
		else	printf("%s \n",gerai);
		
		}

static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *params, snd_pcm_access_t access)
{	snd_pcm_uframes_t val;	
        unsigned int rrate;
        snd_pcm_uframes_t size;
        int dir;
        snd_pcm_hw_params_any(handle, params);                        /* choose all parameters */
        snd_pcm_hw_params_set_rate_resample(handle, params, resample);/* set hardware resampling */
        snd_pcm_hw_params_set_access(handle, params, access);         /* set the interleaved read/write format */
        snd_pcm_hw_params_set_format(handle, params, format);         /* set the sample format */
        snd_pcm_hw_params_set_channels(handle, params, channels);     /* set the count of channels */
        rrate = rate;  

	//snd_pcm_hw_params_get_buffer_size_max(params,&size);
	// printf("max period %li \n", size);
	size=buffer_size;                                               /* set the stream rate */
        snd_pcm_hw_params_set_rate_near(handle, params, &rrate, 0);
        snd_pcm_hw_params_set_buffer_size_near(handle, params, &size);/* set the buffer time */
      //  snd_pcm_hw_params_get_buffer_time(params, &size);
       // buffer_time = size;
	size=period_size;
        snd_pcm_hw_params_set_period_size_near(handle, params, &size,&dir);/* set the period time */
	 snd_pcm_hw_params_get_period_size(params, &val, NULL);
		printf("Periodo dydis  : %lu \n", val);
	//size=buffer_size; 
      //  snd_pcm_hw_params_get_period_time(params, &size, &dir);
       // period_time = size;
        snd_pcm_hw_params(handle, params);                            /* write the parameters to device */
        return 0;
}

static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams)
{
        snd_pcm_sw_params_current(handle, swparams);                /* get the current swparams */

                  
       if (snd_pcm_sw_params_set_start_threshold(handle, swparams, 0x0fffffff) < 0) {
               printf("Unable to set start threshold mode for : ");
                //return err;
        }                           /* start the transfer when the buffer is almost full: */
                                            /* (buffer_time / avail_min) * avail_min */
        snd_pcm_sw_params_set_start_threshold(handle, swparams,  2*100000*period_size/rate);
	 // snd_pcm_sw_params_set_start_threshold(handle, swparams, 0x00004000);
                                     /* allow the transfer when at least period_time samples can be processed */
                                            /* or disable this mechanism when period event is enabled (aka interrupt like style processing) */
        snd_pcm_sw_params_set_avail_min(handle, swparams,period_size);
        snd_pcm_sw_params(handle, swparams);/* write the parameters to the playback device */
        return 0;
}



float timedifference(time_t realtime ,time_t lasttime )
{
    float time=0;
	if(lasttime >realtime)
		{  time= 999999999-(float)lasttime +(float)realtime;

	}
	else
		{ time= (float)realtime-(float)lasttime;
		
	}	
        return time;
}



