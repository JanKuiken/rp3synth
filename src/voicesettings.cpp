#include "voicesettings.h"

#include <iostream>  // TODO: moet weer weg

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


VoiceSettings::VoiceSettings(const std::string& in_settings_file)
{
    settings_file = in_settings_file;
    Load();
}

void VoiceSettings::Load()
{
    boost::property_tree::ptree pt;

    read_json(settings_file, pt);

    attack  = pt.get("attack"   , 0.05);
    decay   = pt.get("decay"    , 0.05);
    sustain = pt.get("sustain"  , 0.9 );
    release = pt.get("release"  , 0.05);
}

void VoiceSettings::Save()
{
    boost::property_tree::ptree pt;

    pt.put("attack",  attack);
    pt.put("decay",   decay);
    pt.put("sustain", sustain);
    pt.put("release", release);

    write_json(settings_file, pt);
}

