#include "voicesettings.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <fstream>



VoiceSettings::VoiceSettings(const std::string& in_settings_file)
{
    settings_file = in_settings_file;
    Load();
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

    read_json(settings_file, pt);

    waveform_1 = pt.get("waveform_1",  "square");
    gain_1     = pt.get("gain_1",      1.0 );
    attack_1   = pt.get("attack_1",    0.05);
    decay_1    = pt.get("decay_1",     0.05);
    sustain_1  = pt.get("sustain_1",   0.9 );
    release_1  = pt.get("release_1",   0.05);

    waveform_2 = pt.get("waveform_2",  "noise");
    gain_2     = pt.get("gain_2",      0.8 );
    attack_2   = pt.get("attack_2",    0.05);
    decay_2    = pt.get("decay_2",     0.05);
    sustain_2  = pt.get("sustain_2",   0.9 );
    release_2  = pt.get("release_2",   0.2 );


}

void VoiceSettings::Save()
{
    boost::property_tree::ptree pt;

    pt.put("waveform_1", waveform_1);
    pt.put("gain_1",     gain_1);
    pt.put("attack_1",   attack_1);
    pt.put("decay_1",    decay_1);
    pt.put("sustain_1",  sustain_1);
    pt.put("release_1",  release_1);

    pt.put("waveform_2", waveform_2);
    pt.put("gain_2",     gain_2);
    pt.put("attack_2",   attack_2);
    pt.put("decay_2",    decay_2);
    pt.put("sustain_2",  sustain_2);
    pt.put("release_2",  release_2);

    write_json(settings_file, pt);
}

