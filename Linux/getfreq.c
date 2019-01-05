// de SQ3SWF 2018

#include <stdio.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 31337 // UDP


float buf[22050];
int len = 300;

void readAudioThread() {

	// for socket communication
	struct sockaddr_in si_other;
	int slen=sizeof(si_other);
	int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);

	// pulseaudio samples capture
	static const pa_sample_spec ss = { .format = PA_SAMPLE_FLOAT32LE, .rate = 22050, .channels = 1 };

	pa_simple *sr;
	sr = pa_simple_new(NULL, "QCXF", PA_STREAM_RECORD, NULL, "", &ss, NULL, NULL, NULL);
	pa_simple_read(sr, buf, len*sizeof(float), NULL);

	// variables for audio processing
	int i=1, rec = 0, reject=0, empty=0, symbol = 0, newqrg = 0;
	double last = -1;
	double diff=0, first=0;
	double freq=0, lastfreq=0;
	char message[10];


	while(1) {

		// read samples to the buffer
		pa_simple_read(sr, buf, len*sizeof(float), NULL);
		rec = 0;
		i=1;
		last = -1;
		empty = 0;

		for(int x=0;x<len;x++) {
			// count empty samples, 20 samples ~< 1ms @ 22050 Hz sr
			if(buf[x] < 0.001 && buf[x] > -0.001) { empty++; }
			else { empty = 0; }
			if(empty > 20) { 	// 20 empty frames? send "Z" command
				reject = 4;
				symbol=0;
				first = 0;
				sprintf(message, "Z");
				sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
				break; }
		}

		for(i=0; i<len; i++) {		// for every sample in the buffer

	 		if(buf[i-1] < 0.0 && buf[i] >= 0.0) { // look for zero-crossing

				if(last >= 0) {
					rec++;
					diff = (i + (-buf[i-1]/(-buf[i-1]+buf[i])))-last; // interpolate
					freq = 22050/diff;	// calculate frequency
					if(abs(freq-lastfreq) > 1) {
						reject = 2; 	// wait two more 0-crossings before final measure (=~5ms @ 200 Hz)
						lastfreq = freq;
						newqrg = 1;
					}

					// check if difference is above 7 samples
					if(diff > 7 && reject == 0 && newqrg) {
						if(first==0) { first = freq; }
						printf("%d: %.3f %.3f %.3f\n", symbol, diff, freq, (freq-first)/6.25);
						sprintf(message, "A%05dX", (int)((freq*10)+0.5));	// send via socket
						sendto(s, message, strlen(message) , 0 , (struct sockaddr *) &si_other, slen);
						symbol++;
						newqrg = 0;
					}

					if(reject > 0) { reject--; }
	 			}
				last = i + (-buf[i-1]/(-buf[i-1]+buf[i]));
			}
		}

	}

}

int main(int argc, char **argv){

	readAudioThread();

	return 0;

}
