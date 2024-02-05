/* Copyright (c) 2020 Matthew Garner

Permission is hereby granted, free of charge, to any person obtaining a copy
of this softwareand associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright noticeand this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 A set of basic functions to allow Gamemaker Studio to use RtMidi
 Mostly based on modified test projects that come with the RtMidi source
 by Matthew Garner, 2020
*/


#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include "RtMidi.h"

#ifdef _WIN32
    #define fn_export extern "C" __declspec (dllexport)
#else
    #define fn_export extern "C"
#endif

unsigned int inPorts = 0;
unsigned int outPorts = 0;
unsigned int outPort = 0;//the output port to use
static char* outPortName;
static char* outPortNames[32];
static char* inPortName;
char* inPortNames[32];

std::vector<unsigned char> message;//MIDI messages are saved to this after being retreived

static std::string portName;

RtMidiOut* gmidiout = 0;//the global MIDI output instance, for sending messages
RtMidiIn* gmidiin = 0;//the global MIDI input instance




//return the number of output ports
fn_export int rtmidi_num_outs()
{
    return outPorts;
}

//return the number of input ports
fn_export int rtmidi_num_ins()
{
    return inPorts;
}

//return the name of the indicated output port
fn_export char* rtmidi_name_out(double f)
{
    f += 0.5;
    int j = (int) f;
    
    double p = 0;

    RtMidiOut* midiout = 0;

    try {
        // RtMidiOut constructor ... exception possible
        midiout = new RtMidiOut();
        
        portName = midiout->getPortName(j);
        outPortName = const_cast<char*>(portName.c_str());
       
    }
    catch (RtMidiError& error) {
        error.printMessage();
    }

    delete midiout;

    return outPortName;

}

//return the name of the indicated input port as a string
fn_export char* rtmidi_name_in(double f)
{
    f += 0.5;
    int j = (int)f;

    double p = 0;

    RtMidiIn* midiin = 0;

    try {
        // RtMidiOut constructor ... exception possible
        midiin = new RtMidiIn();

        portName = midiin->getPortName(j);
        inPortName = const_cast<char*>(portName.c_str());

    }
    catch (RtMidiError& error) {
        error.printMessage();
    }

    delete midiin;

    return inPortName;
}

//sets the global output port, returns 1 if successful, -1 if not
//gmidiout must be created first!
fn_export double rtmidi_set_outport(double p)
{
    p += 0.5;
    int port = (int)p;
    double success = 1;
    std::string outport;
    //set the port
    try {
        gmidiout->closePort();
        gmidiout->openPort(port);
        outport = gmidiout->getPortName(port);
        std::cout << "\nSetting MIDI port to " << port << std::endl;
    }
    catch (RtMidiError& error) {
        error.printMessage();
        std::cout << "\nCouldn't set the port!" << std::endl;
        success = -1;
    }

    return success;
}

//sets the global input port, returns 1 if successful, -1 if not
//gmidiin must be created first!
fn_export double rtmidi_set_inport(double p)
{
    p += 0.5;
    unsigned int port = (unsigned int)p;
    double success = 1;
    std::string inport;
    //set the port
    try {
        gmidiin->closePort();
        gmidiin->openPort(port);
        inport = gmidiin->getPortName(port);
        std::cout << "\nSetting MIDI port to " << inport << std::endl;
    }
    catch (RtMidiError& error) {
        error.printMessage();
        std::cout << "\nCouldn't set the port!" << std::endl;
        success = -1;
    }

    return success;
}

//creates an instance of gmidiout to send messages on the indicated port, must be destroyed when done using
fn_export void rtmidi_create_midiout()
{
    try {
        gmidiout = new RtMidiOut();
    }
    catch (RtMidiError& error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

//creates an instance of gmidiin to receive messages on the indicated port, must be destroyed when done using

fn_export void rtmidi_create_midiin()
{
    try {
        gmidiin = new RtMidiIn();
    }
    catch (RtMidiError& error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

//creates gmidiin and gmidiout, which are then used to perform operations like probing ports and sending messages
fn_export void rtmidi_init()
{
    // Create an api map.
    std::map<int, std::string> apiMap;
    apiMap[RtMidi::MACOSX_CORE] = "OS-X CoreMIDI";
    apiMap[RtMidi::WINDOWS_MM] = "Windows MultiMedia";
    apiMap[RtMidi::UNIX_JACK] = "Jack Client";
    apiMap[RtMidi::LINUX_ALSA] = "Linux ALSA";
    apiMap[RtMidi::RTMIDI_DUMMY] = "RtMidi Dummy";

    std::vector< RtMidi::Api > apis;
    RtMidi::getCompiledApi(apis);

    std::cout << "\nCompiled APIs:\n";
    for (unsigned int i = 0; i < apis.size(); i++)
    {
        std::cout << "  " << apiMap[apis[i]] << std::endl;
    }

    rtmidi_create_midiin();
    rtmidi_create_midiout();
}

//change whether sysex, timing, and active sensing are received. By default, all three of these are ignored.
//the global MIDI output must be first created!
fn_export void rtmidi_ignore_messages(double s, double t, double a)
{
    s += 0.5;
    t += 0.5;
    a += 0.5;
    unsigned int sysex = (unsigned int)s;
    unsigned int timing = (unsigned int)t;
    unsigned int active = (unsigned int)a;
    gmidiin->ignoreTypes(sysex, timing, active);
    //draw to output
    std::cout << "\nIgnore sysex: " << sysex << "Ignore timing: " << timing << "Ignore active sensing: " << active << std::endl;
}



//destroys the working instance of gmidiout
void rtmidi_destroy_midiout()
{
    delete gmidiout;
    gmidiout = 0;
}

//destroys the working instance of gmidiin
void rtmidi_destroy_midiin()
{
    delete gmidiin;
    gmidiin = 0;
}

//deinit, destroys both gmidiin and gmidiout
fn_export void rtmidi_deinit()
{
    rtmidi_destroy_midiout();
    rtmidi_destroy_midiin();
}

//sends a three-part message with gmidiout, which must have been previously created with rtmidi_create_midiout()
//the message shouldn't require any further processing
fn_export void rtmidi_send_message(double m1, double m2 = -10, double m3 = -10)
{
    //convert incoming doubles to ints
    m1 += 0.5;
    //make sure the incoming doubles aren't negative
    if (m2 >= 0){
        m2 += 0.5;
    }
    else {
        m2 -= 0.5;
    }
    if (m3 >= 0){
        m3 += 0.5;
    }
    else {
        m3 -= 0.5;
    }
    
    int vm1 = (int)m1;
    int vm2 = (int)m2;
    int vm3 = (int)m3;

    RtMidiOut* midiout = 0;
    std::vector<unsigned char> message;

    //send out the message
    message.push_back(vm1);
    //don't send negative values
    if (vm2 >= 0) {
        message.push_back(vm2);
    }
    if (vm3 >= 0) {
        message.push_back(vm3);
    }

    //std::cout << vm1 << vm2 << vm3 << std::endl;

    try {
        gmidiout->sendMessage( &message );
    }
    catch (RtMidiError& error) {
        error.printMessage();
        std::cout << "\nCouldn't send message!" << std::endl;
    }
}

//checks if a MIDI message is in the queue. If so, it's saved to vector message and the number of bytes is returned, otherwise return -1
fn_export double rtmidi_check_message()
{
    double nbytes;
    double i = -1;
    double stamp;

    stamp = gmidiin->getMessage(&message);
    nbytes = message.size();

    if (nbytes > 0){
        i = nbytes;
    }

    return i;
}

//returns the specified byte of the saved midi data, or -1 if there's no data there
fn_export double rtmidi_get_message(double b)
{
    b += 0.5;
    unsigned int byte = (unsigned int)b;
    int bytes = message.size();
    double i = -1;

    std::cout << "\nBytes: " << bytes << std::endl;

    if (byte <= bytes){
        i = (double)message[byte];
    }

    return i;
}

//probes the available output connections, returns how many
fn_export double rtmidi_probe_outs()
//int main()
{
    double p = 0;

    try {

        // Check outputs.
        outPorts = gmidiout->getPortCount();
        p = outPorts;
        std::cout << "\nThere are " << outPorts << " MIDI output ports available.\n";

        for (unsigned i = 0; i < outPorts; i++) {
            //std::string portName = midiout->getPortName(i);
            portName = gmidiout->getPortName(i);
            outPortName = const_cast<char*>(portName.c_str());
            //outPortNames[i] = outPortName;
            //outPortNames[i] = outPortName;
            std::cout << "  Output Port #" << i << ": " << outPortName << std::endl;
        }
        std::cout << std::endl;

    }
    catch (RtMidiError& error) {
        error.printMessage();
    }

    return p;
}

//probes the available MIDI input ports, returns how many
fn_export double rtmidi_probe_ins()
//int main()
{
    double p = 0;

    try {

        // Check inputs.
        inPorts = gmidiin->getPortCount();
        p = inPorts;
        std::cout << "\nThere are " << inPorts << " MIDI input ports available.\n";

        for (unsigned i = 0; i < inPorts; i++) {
            //std::string portName = midiin->getPortName(i);
            portName = gmidiin->getPortName(i);
            inPortName = const_cast<char*>(portName.c_str());
            //inPortNames[i] = inPortName;
            //inPortNames[i] = inPortName;
            std::cout << "  Input Port #" << i << ": " << inPortName << std::endl;
        }
        std::cout << std::endl;

    }
    catch (RtMidiError& error) {
        error.printMessage();
    }

    return p;
}

//test function for passing strings from GMS
fn_export void string_tester(char* string)
{
    std::cout << string << std::endl;
}

//creates a virtual output port with the provided name. Doesn't work with Windows, since only ALSA, coreMIDI, and JACK support virtual ports.
fn_export void rtmidi_create_virtual_outport(char* name)
{
    gmidiout->openVirtualPort(name);
}
//creates a virtual input port with the provided name. Doesn't work with Windows, since only ALSA, coreMIDI, and JACK support virtual ports.
fn_export void rtmidi_create_virtual_inport(char* name)
{
    gmidiin->openVirtualPort(name);
}

int main() {

}
