#include "voicesettings.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <fstream>
#include <iostream>  // TODO: moet weer weg

#include "utils.h"



VoiceSettings::VoiceSettings(const std::string& in_settings_file)
{
    settings_file = in_settings_file;
    Load();
}

VoiceSettings::~VoiceSettings()
{
    Save();
}


void VoiceSettings::Load()
{
    boost::property_tree::ptree pt;

    // check if file exists, otherwise create an emtpy json file
    std::ifstream fin(settings_file);
    if (!fin) {
        std::ofstream outfile(settings_file);
        outfile.write("{}\n", 3);
        outfile.close();
    }

    // read setting values from file or apply defaults, if value not present
    read_json(settings_file, pt);

    waveform_1 = StringToWaveform(pt.get("waveform_1",  "square"));
    detune_1   = pt.get("detune_2",    0.5);
    attack_1   = pt.get("attack_1",    0.05);
    decay_1    = pt.get("decay_1",     0.05);
    sustain_1  = pt.get("sustain_1",   0.9 );
    release_1  = pt.get("release_1",   0.05);

    waveform_2 = StringToWaveform(pt.get("waveform_2",  "noise"));
    detune_2   = pt.get("detune_2",    0.5);
    attack_2   = pt.get("attack_2",    0.05);
    decay_2    = pt.get("decay_2",     0.05);
    sustain_2  = pt.get("sustain_2",   0.9 );
    release_2  = pt.get("release_2",   0.2 );

    chorus_gain      = pt.get("chorus_gain",      0.5);
    chorus_depth     = pt.get("chorus_depth",     0.5);
    chorus_frequency = pt.get("chorus_frequency", 0.5);

    master_volume = pt.get("master_volume", 0.5);
}

void VoiceSettings::Save()
{
    boost::property_tree::ptree pt;

    pt.put("waveform_1", WaveformToString(waveform_1));
    pt.put("attack_1",   attack_1);
    pt.put("decay_1",    decay_1);
    pt.put("sustain_1",  sustain_1);
    pt.put("release_1",  release_1);

    pt.put("waveform_2", WaveformToString(waveform_2));
    pt.put("detune_2",   detune_2);
    pt.put("attack_2",   attack_2);
    pt.put("decay_2",    decay_2);
    pt.put("sustain_2",  sustain_2);
    pt.put("release_2",  release_2);

    pt.put("chorus_gain",      chorus_gain);
    pt.put("chorus_depth",     chorus_depth);
    pt.put("chorus_frequency", chorus_frequency);

    pt.put("master_volume", master_volume);

    write_json(settings_file, pt);
}

void VoiceSettings::HandleKnob(unsigned int param, int value)
{
    // This handles (for now) modulation events from my Akai MIDI-Mix device

    double normal_value = bounds_limit(value / 127.0, 0.0, 1.0);

    switch (param)
    {
    case 18: detune_1         =       normal_value; break;
    case 19: attack_1         = 0.5 * normal_value; break;
    case 23: decay_1          = 0.5 * normal_value; break;
    case 27: sustain_1        =       normal_value; break;
    case 31: release_1        = 0.5 * normal_value; break;

    case 48: detune_2         =       normal_value; break;
    case 49: attack_2         = 0.5 * normal_value; break;
    case 53: decay_2          = 0.5 * normal_value; break;
    case 57: sustain_2        =       normal_value; break;
    case 61: release_2        = 0.5 * normal_value; break;

    case 50: chorus_gain      = normal_value; break;
    case 54: chorus_depth     = normal_value; break;
    case 58: chorus_frequency = normal_value; break;

    case 62: master_volume    = normal_value; break;
    }
}

void VoiceSettings::HandleButton(int note)
{
    switch (note){
    case 1:
        waveform_1 = CycleWaveformNext(waveform_1);
        std::cout << "Wave 1 : " << WaveformToString(waveform_1) << std::endl;
        break;
    case 4:
        waveform_2 = CycleWaveformNext(waveform_2);
        std::cout << "Wave 2 : " << WaveformToString(waveform_2) << std::endl;
        break;
    }

}
