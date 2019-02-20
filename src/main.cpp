/*
 * My RaspberryPi 3 Synthesizer project
 *
 * the main file is based on:
 * http://alsamodular.sourceforge.net/alsa_programming_howto.html#sect06
 *
 * original comment:
 *
 *   miniFMsynth 1.1 by Matthias Nagorni
 *   This program uses callback-based audio
 *   playback as proposed by Paul Davis on
 *   the linux-audio-dev mailinglist.
 *
 * changes made:
 *   - changed C to C++
 *   - split off/changed playback_callback and midi_callback to class RP3Synth
 *   - added sig_handler and the lot for a clean ctrl-C exit
 *   - added capture_keyboard
 */

#include <stdio.h>   
#include <cstdlib>
#include <alsa/asoundlib.h>
#include <math.h>

#include <signal.h>
#include <string>
#include <iostream>

#include "rp3synth.h"

#define POLY    8
#define RATE    44100
#define BUFSIZE 512


using namespace std;

snd_seq_t *seq_handle;
snd_pcm_t *playback_handle;
short *buf;
double pitch, modulation;
unsigned int rate;
bool continue_playing = true;

snd_seq_t *open_seq(char *midi_name) {

    snd_seq_t *seq_handle;

    rate = RATE;
    if (snd_seq_open(&seq_handle, midi_name, SND_SEQ_OPEN_DUPLEX, 0) < 0) {
        cout << "Error opening ALSA sequencer.\n";
        exit(1);
    }
    snd_seq_set_client_name(seq_handle, "RP3Synth");
    if (snd_seq_create_simple_port(seq_handle, "RP3Synth",
        SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION) < 0) {
        cout << "Error creating sequencer port.\n";
        exit(1);
    }
    return(seq_handle);
}

void capture_keyboard(snd_seq_t *seq, int client, int port)
{
    snd_seq_addr_t sender, dest;
    snd_seq_port_subscribe_t *subs;

    sender.client = client;
    sender.port = port;
    dest.client = 128;
    dest.port = 0;
    snd_seq_port_subscribe_alloca(&subs);
    snd_seq_port_subscribe_set_sender(subs, &sender);
    snd_seq_port_subscribe_set_dest(subs, &dest);
    snd_seq_port_subscribe_set_queue(subs, 1);
    snd_seq_port_subscribe_set_time_update(subs, 1);
    snd_seq_port_subscribe_set_time_real(subs, 1);
    snd_seq_subscribe_port(seq, subs);
}

snd_pcm_t *open_pcm(const char *pcm_name) {

    snd_pcm_t *playback_handle;
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_sw_params_t *sw_params;
            
    if (snd_pcm_open (&playback_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        cout << "cannot open audio device" << pcm_name << "\n";
        exit (1);
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(playback_handle, hw_params);
    snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2);
    snd_pcm_hw_params_set_periods(playback_handle, hw_params, 2, 0);
    snd_pcm_hw_params_set_period_size(playback_handle, hw_params, BUFSIZE, 0);
    snd_pcm_hw_params(playback_handle, hw_params);
    snd_pcm_sw_params_alloca(&sw_params);
    snd_pcm_sw_params_current(playback_handle, sw_params);
    snd_pcm_sw_params_set_avail_min(playback_handle, sw_params, BUFSIZE);
    snd_pcm_sw_params(playback_handle, sw_params);
    return playback_handle;
}

int midi_callback() {
/*
    snd_seq_event_t *ev;
    int l1;
  
    do {
        snd_seq_event_input(seq_handle, &ev);


      if ((ev->type == SND_SEQ_EVENT_NOTEON) && (ev->data.note.velocity == 0)) ev->type = SND_SEQ_EVENT_NOTEOFF;

      switch (ev->type) {
            case SND_SEQ_EVENT_PITCHBEND:
                pitch = (double)ev->data.control.value / 8192.0;
                break;
            case SND_SEQ_EVENT_CONTROLLER:
                if (ev->data.control.param == 1) {
                    modulation = (double)ev->data.control.value / 10.0;
                } 
                break;
            case SND_SEQ_EVENT_NOTEON:
                for (l1 = 0; l1 < POLY; l1++) {
                    if (!note_active[l1]) {
                        note[l1] = ev->data.note.note;
                        velocity[l1] = ev->data.note.velocity / 127.0;
                        env_time[l1] = 0;
                        gate[l1] = 1;
                        note_active[l1] = 1;
                        break;
                    }
                }
                break;        
            case SND_SEQ_EVENT_NOTEOFF:
                for (l1 = 0; l1 < POLY; l1++) {
                    if (gate[l1] && note_active[l1] && (note[l1] == ev->data.note.note)) {
                        env_time[l1] = 0;
                        gate[l1] = 0;
                    }
                }
                break;        
        }
        snd_seq_free_event(ev);
    } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
*/
    return (0);
}

int playback_callback (snd_pcm_sframes_t nframes) {
/*
    int l1, l2;
    double dphi, dphi_mod, f1, f2, f3, freq_note, sound;
      
    memset(buf, 0, nframes * 4);
    for (l2 = 0; l2 < POLY; l2++) {
        if (note_active[l2]) {
            f1 = 8.176 * exp((double)(transpose+note[l2]-2)*log(2.0)/12.0);
            f2 = 8.176 * exp((double)(transpose+note[l2])*log(2.0)/12.0);
            f3 = 8.176 * exp((double)(transpose+note[l2]+2)*log(2.0)/12.0);
            freq_note = (pitch > 0) ? f2 + (f3-f2)*pitch : f2 + (f2-f1)*pitch;
            dphi = M_PI * freq_note / 22050.0;                                    
            dphi_mod = dphi * (double)harmonic / (double)subharmonic;
            for (l1 = 0; l1 < nframes; l1++) {
                phi[l2] += dphi;
                phi_mod[l2] += dphi_mod;
                if (phi[l2] > 2.0 * M_PI) phi[l2] -= 2.0 * M_PI;
                if (phi_mod[l2] > 2.0 * M_PI) phi_mod[l2] -= 2.0 * M_PI;
                sound = GAIN * 1.0
                             * velocity[l2] * sin(phi[l2] + modulation * sin(phi_mod[l2]));
                env_time[l2] += 1.0 / 44100.0;
                buf[2 * l1] += sound;
                buf[2 * l1 + 1] += sound;
            }
        }    
    }
    return snd_pcm_writei (playback_handle, buf, nframes); 
*/
    return 0;
}

void sig_handler(int s){
    continue_playing = false;
}

void usage()
{
    const std::string msg = "\n"
"Usage:\n\n"
"  rp3synth  pcm_device  midi_client  midi_port  settings_file\n\n"
"    pcm_device:\n"
"      something like: hw:0\n"
"      number can be found with: aplay -l\n\n"
"    midi_client  midi_port:\n"
"      something like: 20 1\n"
"      numbers can be found with: aconnect -l\n\n"
"    settings_file:\n"
"      path to a rp3synth settings file (might be not existing,\n"
"                                        will be overwritten at exit)\n\n"
"";
      std::cout << msg << std::endl;
      exit(-1);
}
      
int main (int argc, char *argv[]) {

    std::string pcm_device;
    int midi_client;
    int midi_port;
    std::string settings_file;


    // parse arguments
    if (argc !=5) {
        usage();
    }
    pcm_device    = std::string(argv[1]);
    try {
        midi_client = std::stoi(argv[2]);
    } catch (const std::exception& e) {
        std::cout << "\nError: Could not convert 2nd argument to a midi client number\n";
        usage();
    }
    try {
        midi_port = std::stoi(argv[3]);
    } catch (const std::exception& e) {
        std::cout << "\nError: Could not convert 3nd argument to a midi port number\n" ;
        usage();
    }
    settings_file = std::string(argv[4]);


    RP3Synth synth = RP3Synth(POLY, RATE, BUFSIZE, settings_file);

    int nfds, seq_nfds, l1;
    struct pollfd *pfds;

    buf = (short *) malloc (2 * sizeof (short) * BUFSIZE);

    playback_handle = open_pcm(pcm_device.c_str());
    seq_handle = open_seq((char*)"default");
    capture_keyboard(seq_handle, midi_client, midi_port);

    seq_nfds = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
    nfds = snd_pcm_poll_descriptors_count (playback_handle);
    pfds = (struct pollfd *)alloca(sizeof(struct pollfd) * (seq_nfds + nfds));
    snd_seq_poll_descriptors(seq_handle, pfds, seq_nfds, POLLIN);
    snd_pcm_poll_descriptors (playback_handle, pfds+seq_nfds, nfds);

    continue_playing = true;
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = sig_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    while (continue_playing) {
        if (poll (pfds, seq_nfds + nfds, 1000) > 0) {
            for (l1 = 0; l1 < seq_nfds; l1++) {
               //if (pfds[l1].revents > 0) midi_callback();
               if (pfds[l1].revents > 0)
               {
                    snd_seq_event_t *ev;
                    do {
                        snd_seq_event_input(seq_handle, &ev);
                        if (pfds[l1].revents > 0) {
                            synth.MidiCallback(ev);
                        }
                        snd_seq_free_event(ev);
                    } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
               }
            }
            for (l1 = seq_nfds; l1 < seq_nfds + nfds; l1++) {    
                if (pfds[l1].revents > 0) { 
                    // if (playback_callback(BUFSIZE) < BUFSIZE) {
                    // return snd_pcm_writei (playback_handle, buf, nframes);
                    synth.PlaybackCallback(buf);
                    int size = snd_pcm_writei (playback_handle, buf, BUFSIZE);
                    if (size < BUFSIZE) {
                        cout << "xrun !\n";
                        snd_pcm_prepare(playback_handle);
                    }
                }
            }        
        }
    }

    snd_pcm_close (playback_handle);
    snd_seq_close (seq_handle);
    free(buf);
    cout << "\nBye...\n\n";
    return 0;
}
     
