/*
 Originally forked from: github.com/trilbywhite
 Thanks to: github.com/unia for sound query and mpd.
 Modified by: illusionist: github.com/gk7 . I did some tweakings here and there and optimized cpu and free memory calculation.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <X11/Xlib.h>
#include <alsa/asoundlib.h>
#include <mpd/client.h>
/* #include <sys/sysinfo.h> */

/* Sleeps for INTERVAL seconds between updates */
#define INTERVAL	1

/* Files read for system info: */
#define CPU_FILE		"/proc/stat"                        /* File to query for cpu information */
#define MEM_FILE		"/proc/meminfo"                     /* File to query for memory information */
#define VOL_CH          "Master"                            /* Channel to watch for volume */
#define AC		       "/sys/class/power_supply/AC/online"

#define CPU_STR			" \x08Ñ\x07 %d%%\x07"				/* CPU percentage */
#define MEM_STR			" \x08Î\x07 %ld MB\x07"	            /* Free memory */
#define VOL_STR			" \x08í\x07 %d%%\x07"			    /* volume */
#define VOL_MUTE_STR	" \x08ë\x07 Mute\x07"				/* volume when muted */
#define AC_STR			" \x08Â\x07 AC\x07"				    /* Ac status */
#define BAT_STR		    " \x03ï\x07 Battery\x07"			/* Battery status */
#define DATE_TIME_STR	" \x08É\x07 %a %I:%M\x07"	        /* This is a strftime format string which is passed localtime */
#define NO_MPD_STR      " "                                 /* MPD, not playing */
#define MPD_STR         "\x08æ\x07 %s\x07"                  /* MPD, playing */
#define MPD_P_STR       "\x03ç\x07 Paused\x07"              /* MPD, paused */

int main() {
	Display *dpy;
	Window root;
	int num,mute = 0,realvol = 0;
	long jif1,jif2,jif3,jif4,jif5,jif6,jif7,min = 0,max = 0,vol = 0;
	long lnum1,lnum2,lnum3,lnum4,lnum5,lnum6,lnum7;
    float tjifs1,tjifs2,wjifs1,wjifs2;
	char statnext[30], status[100];
    snd_mixer_t *handle;
    snd_mixer_elem_t *pcm_mixer, *mas_mixer;
    snd_mixer_selem_id_t *vol_info, *mute_info;
#ifdef CLK
	time_t current;
#endif
#ifdef MPD
    char musicstring[100];
    struct mpd_song *song = NULL;
    const char *title = NULL;//, *artist = NULL;
#endif
	FILE *infile;
	/* get initial jiffies */
	infile = fopen(CPU_FILE,"r");
	fscanf(infile,"cpu %ld %ld %ld %ld %ld %ld %ld",&jif1,&jif2,&jif3,&jif4,&jif5,&jif6,&jif7);
	fclose(infile);
        tjifs1 = jif1+jif2+jif3+jif4+jif5+jif6+jif7;
        wjifs1 = jif1+jif2+jif3;
	/* Setup X display and root window id: */
	dpy=XOpenDisplay(NULL);
	if ( dpy == NULL) {
		fprintf(stderr, "ERROR: could not open display\n");
		exit(1);
	}
	root = XRootWindow(dpy,DefaultScreen(dpy));
    /* Main loop starts here */
	for (;;) {
		status[0]='\0';
    /* Music player daemon */
#ifdef MPD
			struct mpd_connection *conn = mpd_connection_new(NULL, 0, 30000);
			if (mpd_connection_get_error(conn))
				sprintf(musicstring, NO_MPD_STR);
			mpd_command_list_begin(conn, true);
			mpd_send_status(conn);
			mpd_send_current_song(conn);
			mpd_command_list_end(conn);
			struct mpd_status* theStatus = mpd_recv_status(conn);
				if (!theStatus)
					sprintf(musicstring, NO_MPD_STR);
				else {
					if (mpd_status_get_state(theStatus) == MPD_STATE_PLAY) {
						mpd_response_next(conn);
						song = mpd_recv_song(conn);
						title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
						/* artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0); */
						sprintf(musicstring,MPD_STR, title);
						mpd_song_free(song);
					} else if (mpd_status_get_state(theStatus) == MPD_STATE_PAUSE) {
						mpd_response_next(conn);
						song = mpd_recv_song(conn);
						title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
						/* artist = mpd_song_get_tag(song, MPD_TAG_ARTIST, 0); */
						sprintf(musicstring, MPD_P_STR);
						mpd_song_free(song);
					} else if (mpd_status_get_state(theStatus) == MPD_STATE_STOP)
						sprintf(musicstring, NO_MPD_STR);
				}
			mpd_response_finish(conn);
			mpd_connection_free(conn);
		strcat(status, musicstring);
#endif
	    /* CPU use: */
		infile = fopen(CPU_FILE,"r");
		fscanf(infile,"cpu %ld %ld %ld %ld %ld %ld %ld",&lnum1,&lnum2,&lnum3,&lnum4,&lnum5,&lnum6,&lnum7);
		fclose(infile);
        tjifs2 = lnum1+lnum2+lnum3+lnum4+lnum5+lnum6+lnum7;
        wjifs2 = lnum1+lnum2+lnum3;
		num = (int) ((100*(wjifs2-wjifs1))/(tjifs2-tjifs1));
		jif1=lnum1; jif2=lnum2; jif3=lnum3; jif4=lnum4; jif5=lnum5; jif6=lnum6; jif7=lnum7; tjifs1=tjifs2; wjifs1=wjifs2;
		sprintf(statnext,CPU_STR,num);
		strcat(status,statnext);
	    /* Memory use: */
		infile = fopen(MEM_FILE,"r");
		fscanf(infile,"MemTotal: %ld kB\nMemFree: %ld kB\nBuffers: %ld kB\nCached: %ld kB\n",
			&lnum1,&lnum2,&lnum3,&lnum4);
		fclose(infile);
		sprintf(statnext,MEM_STR,((lnum1-(lnum2+lnum3+lnum4))/1024));
        /*sprintf(statnext,MEM_STR,((info.totalram-info.freeram)/1024)*info.mem_unit);*/
		strcat(status,statnext);
	    /* Audio volume */
		snd_mixer_open(&handle, 0);
		snd_mixer_attach(handle, "default");
		snd_mixer_selem_register(handle, NULL, NULL);
		snd_mixer_load(handle);
		snd_mixer_selem_id_malloc(&vol_info);
		snd_mixer_selem_id_set_name(vol_info, VOL_CH);
		pcm_mixer = snd_mixer_find_selem(handle, vol_info);
		snd_mixer_selem_get_playback_volume_range(pcm_mixer, &min, &max);           /* get volume */
		snd_mixer_selem_get_playback_volume(pcm_mixer, SND_MIXER_SCHN_MONO, &vol);
		snd_mixer_selem_id_malloc(&mute_info);
		snd_mixer_selem_id_set_name(mute_info, VOL_CH);
		mas_mixer = snd_mixer_find_selem(handle, mute_info);
		snd_mixer_selem_get_playback_switch(mas_mixer, SND_MIXER_SCHN_MONO, &mute); /* get mute state */
		if(mute == 0)
			sprintf(statnext, VOL_MUTE_STR);
		else {
			realvol = (vol * 100) / max;
			sprintf(statnext, VOL_STR, realvol);
		}
		if(vol_info)
			snd_mixer_selem_id_free(vol_info);
		if (mute_info)
			snd_mixer_selem_id_free(mute_info);
		if (handle)
			snd_mixer_close(handle);
		strcat(status,statnext);
	    /* Date & Time: */
#ifdef CLK
		time(&current);
		strftime(statnext,38,DATE_TIME_STR,localtime(&current));
		strcat(status,statnext);
#endif
	    /* Power/Battery: */
		infile = fopen(AC,"r");
			fscanf(infile,"%ld\n",&lnum1);fclose(infile);
        num = 0;
		if (lnum1 == 0) {
			sprintf(statnext,BAT_STR);
		}
		else {
				sprintf(statnext,AC_STR);
		}
		strcat(status,statnext);
	    /* Set root name */
		XStoreName(dpy,root,status);
		XFlush(dpy);
		sleep(INTERVAL);
	}
	XCloseDisplay(dpy);
	return 0;
}
