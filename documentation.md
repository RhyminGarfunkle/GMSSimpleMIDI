# GMSSimpleMIDI documentation

GMSSimpleMIDI is an extension that allows games made with Gamemaker Studio to send and receive MIDI messages. This could be used to make sequencers or arpeggiators in GMS, or use hardware MIDI controllers as input devices for games made with GMS. Any kind of message can be received, including sysex, and messages up to three bytes long can be sent. Messages are sent and received over ports that usually represent the computer’s physical MIDI connections, for example a USB MIDI interface. If you run into any bugs or need any features that aren’t present, please let me know at loudcactusaudio@gmail.com and I’ll fix it as soon as possible! (no promises about adding features, but I’ll certainly consider them)

In general, using GMSSimpleMIDI looks something like this:
- Initialize with rtmidi_init()
- Check what ports are available with rtmidi_probe_ins() and rtmidi_probe_outs()
- Get the names of those ports with rtmidi_name_in(port) and rtmidi_name_out(port)
- Set the proper input and ouput ports with rtmidi_set_inport(port) and rtmidi_set_outport(port)
- Send messages with rtmidi_send_message(byte,byte,byte)
- Check for incomming MIDI messages with rtmidi_check_message() and get their contents with rtmidi_get_message(byte)
- When done, cleanup with rtmidi_deinit()

NOTE: There’s a lot of talk of “ports” in here. Ports are entirely different from MIDI channels. A port represents the device that’s sending or receiving MIDI data, and MIDI data can be on any of 16 different channels. For example, say you have an interface that’s connected to your computer via USB, and five synthesizers via DIN cable(daisy-chained or through a MIDI-thru box). The port represents the connection between the computer and the interface, while each synth is likely set to receive on a different channel so they don’t all play the same thing. GMSSimpleMIDI can send data over a single port at a time, and each port can send MIDI data for 16 channels simultaneously. It can get confusing!

By default, on Windows software cannot connect to other software via MIDI ports, but you can easily enable this with third-party software like loopMidi. The built-in MIDI drivers for Mac and Linux (Ubuntu) allow this with no external software or drivers necessary, but I’m not directly offering this for Mac or Linux (see below)

I’m offering the code for the DLL that underlies GMSSimpleMIDI as open source. While I’ll only offer official support to users using purchased copies with Windows(I don’t have much access to other systems to test or troubleshoot on), the C++ source should be platform agnostic and easily compiled to a .dylib or .so for use with Mac or Linux. The functions in the extension can then be copied to your own compiled DLL. If you do this, make sure to check out the RtMidi page for necessary compiler statements on different platforms: http://www.music.mcgill.ca/~gary/rtmidi/index.html#compiling and even if you’re not in the group I’m giving “official” support to, I’m usually happy to help if you shoot me a message!

GMSSimpleMIDI uses the RTMidi library, which is free open-source software provided through the MIT license. For more information on RtMidi, see https://www.music.mcgill.ca/~gary/rtmidi/index.html


## USING THE DEMO ARPEGGIATOR:

You’ll need a MIDI source (a controller or sequencer, or DAW) and something to receive the MIDI notes sent by the arpeggiator. Hold down notes on the controller to have the arpeggiator cycle through them in an up-down pattern, release notes to remove them from the pattern. Click the inport box to cycle through available MIDI in ports, same with the outport. The clock can be internal or external, BPM cannot be set when using an external clock source and when set to external notes will only cycled through when receiving active clock pulses. Changing clock source while the arpeggiator is running may result in temporarily glitched behavior for a few steps. Click the boxes to cycle through MIDI channels to determine both what channel to listen for notes on, and what channel to send notes to. Note length determines the note divisions, 1/4, 1/8, 1/16 or 1/32. The notes that the arpeggiator is cycling through is displayed along the bottom as the MIDI note number, with the current note displayed in red.

TROUBLESHOOTING: Use the internal clock until you’re certain everything else is working. If you see notes in the form of numbers along the bottom when you hold down notes on your MIDI controller, notes are being received properly. If the red note cycles up and down when you hold multiple notes, the internal clock is working. Make sure that you’re sending on both the proper port and proper channel, check that your instrument is receiving on the same channel you’re sending.
If you see no notes along the bottom when you hold down notes, make sure you’re using the proper MIDI input port and receiving on the proper channel.
If everything else is working properly but external clock isn’t working, double check the clock settings on the sending device. The simple arpeggiator expects 24ppq (you can use whatever clock rate you choose in when implementing GMSSimpleMIDI, but 24ppq is the standard).
	If you see "undefined" under inport or outport, you don't have any MIDI input or output ports available on your computer. Double check that your interface/controller is correctly installed, or use external software like rtpMidi to create virtual ports.
	RtMidi doesn't consume received messages. If you have a DAW or other software open, it may be necessary to force it to ignore MIDI messages, at least on certain channels, while using the arpeggiator to prevent duplicate signals.


## GMSSimpleMIDI Functions

**rtmidi_init()**
ARGUMENTS: none
RETURNS: none

Initializes MIDI inputs and outputs. It MUST be called before using any other MIDI functions.


**rtmidi_probe_outs()**
ARGUMENTS: none
RETURNS: real

Returns the number of MIDI output ports available.


**rtmidi_probe_ins()**
ARGUMENTS: none
RETURNS: real

Returns the number of MIDI input ports available.


**rtmidi_name_out(port)**
ARGUMENTS: real port, the number of the output port to get the name of
RETURNS: string

Returns the name of the indicated MIDI output port as a string. The available ports must be first determined with rtmidi_probe_outs(). Port numbers start at 0, so if you have four MIDI devices connected to your computer, each one would be between 0-3. The port number of a device can change with different software and hardware connections, so if you wanted to, for example, save the port that MIDI data was being sent or received on in an .ini file, you could get errors if you save the actual port number.  It would be better to save the name of the port, then check which port number corresponded to that string on startup.


**rtmidi_name_in(port)**
ARGUMENTS: port, real, the number of the input port to get the name of
RETURNS: string

Returns the name of the indicated MIDI input port as a string. The port number issue described above also applies to input ports.


**rtmidi_set_outport(port)**
ARGUMENTS: real port, the number of the output port to send output to
RETURNS: real

Sets the output port that MIDI messages will be transmitted on. The available ports must first be found with rtmidi_probe_outs and you probably also want to select the proper output port using rtmidi_name_out. Returns 1 if successful, -1 if not (usually the result of trying to use an invalid port)

**rtmidi_set_inport(port)**
ARGUMENTS: real port, the number of the input port to receive messages on
RETURNS: real

Sets the input port that MIDI messages will be received on. The available ports must first be found with rtmidi_probe_ins and you probably also want to select the proper input port with rtmidi_name_in. Returns 1 if successful, -1 if not (usually the result of trying to use an invalid port)

**rtmidi_send_message(byte1,byte2,byte3)**
ARGUMENTS: real byte1, real byte2, real byte3
RETURNS: none

Sends a three-byte MIDI message using the previously selected output port. Numbers are used instead of bytes. To send a message with less than three bytes, for example a timing pulse, use a negative number for the unneeded bytes. Example: rtmidi_send_message(146,60,80) translates to “Channel 3 Note On, C5, Velocity 80”, rtmidi_send_message(248,-1,-1) sends a single timing pulse to all channels. There’s a quick breakdown of some simple MIDI messages at the end of this document. This argument accepts hexadecimal instead of decimal, if you prefer.


**RECEIVING MESSAGES:** Before documenting the two functions to receive MIDI messages, let me explain how it works in GMSSimpleMIDI. When a MIDI message is received on the selected port, it goes into a queue. Calling rtmidi_check_message does two things: it returns if there’s an unread message in the queue, and loads that message into a message array contained in the DLL. This will destroy any information that was previously in the array, even if there was no message in the queue, so make sure to get the data before calling rtmidi_check_message again! Up to 1024 messages will stack in the queue - more than that, and they’ll start getting discarded as they come in. You usually want to check for messages every step (or better, multiple times a step, until there are no more new messages).
	Once a message is loaded into the array, it’s accessed through Gamemaker Studio with the function rtmidi_get_message(byte). MIDI is sent in a series of bytes, and each entry in the array is one byte. Unfortunately, there’s no simple way to pass an array between a DLL and Gamemaker Studio - fortunately, MIDI messages are generally short, usually only two or three bytes, so it’s easy to simply access one value at a time. There’s a code example after the function descriptions here.


**rtmidi_check_message()**
ARGUMENTS: none
RETURNS: real

Checks if there’s an unread MIDI message in the queue. If so, the number of bytes in the message is returned and the message is removed from the queue and into the message array. If not, the message array is cleared and -1 is returned.


**rtmidi_get_message(byte)**
ARGUMENTS: byte, real;
RETURNS: real

Returns the data at the indicated index of the message array. Since rtmidi_check_message() clears the message array, you must call this if you don’t want your incoming MIDI data to be destroyed!

**Code example:**

	//STEP EVENT
	var b,i,listMessage;
	ds_list_clear(midiMessages)//midiMessages is a ds_list that was created previously, and holds all MIDI messages that came in on this step
	b = rtmidi_check_message()
	while b > 0{
		listMessage = ds_list_create()	
		for (i = 0; i < b; i += 1){	
			ds_list_add(listMessage,rtmidi_get_message(i))		
			}		
		ds_list_add(midiMessages,listMessage)	
		b = rtmidi_check_message()	
		}
	
	//We now have a list, midiMessages, filled with smaller lists that contain all the midi messages that were received this step. Make sure to destroy the smaller lists after using the data in them!


**rtmidi_ignore_messages(sysex,timing,activesensing)**
ARGUMENTS: sysex, bool; timing, bool; activesensing, bool;
RETURNS: none

By default, sysex, timing, and “active sensing” messages are ignored and will not be added to the message queue. True will ignore that message type, false will allow them. Timing messages can clog up the queue if you don’t need them (MIDI sends 24 timing pulses for every quarter note), sysex are specific to different devices and is usually used for things like data backup and firmware updates, and “active sensing” isn’t really used by anything - apparently when the MIDI standard was first developed in the 80s, they thought it was a good idea for controllers to send synths a message every 300ms indicating that they were still plugged in, so the synth could turn all its notes off if the connection was broken. No modern controller sends this, and no synths I know listen for it. I’d never even heard of it until I started coding this!

**rtmidi_deinit()**
ARGUMENTS: none
RETURNS: none

Destroys the objects in the DLL and frees up all memory associated with them. rtmidi_init() must be called again before any further rtmidi functions can be used.

**Included scripts:**

GMSSimpleMIDI contains a few simple GML scripts for common MIDI tasks.

**sMIDISendNoteOn(channel,note,velocity)**
ARGUMENTS: real channel, real note, real velocity
RETURNS: none
	Sends a Note On message to channel 0-15, to play note 0-127, at velocity 0-127.

**sMIDISendNoteOff(channel,note)**
ARGUMENTS: real channel, real note
RETURNS: none
	Sends a Note Off message to channel 0-15, to stop playing note 0-127.

**sMIDISendCC(channel,CC,value)**
ARGUMENTS: real channel, real CC, real value
RETURNS: none
	Sends a message to channel 0-15 to set CC 0-127 to value 0-127.

**sMIDIPanic()**
ARGUMENTS: none
RETURNS: none
	Sends note off messages for all notes on all channels.


NON-WINDOWS EXPERIMENTAL FUNCTIONS
I haven’t been able to test these! Virtual ports aren’t compatible with the Windows media library, only the ALSA, JACK and coreMIDI systems on Linux and OSX. However, there’s no reason they shouldn’t work on Mac or Linux, as long as the source is recompiled as a .dylib (mac) or .so (linux). If you do recompile, make sure to use the compiler statements listed here (http://www.music.mcgill.ca/~gary/rtmidi/#compiling) or else it won’t work!

****THESE FUNCTIONS DO NOT WORK IN WINDOWS**

**rtmidi_create_virtual_inport(name)**
ARGUMENTS: name, string
RETURNS: none

Creates a “virtual” MIDI input port that other software, such as a DAW, can connect to. The supplied string will be the name that other software will use for the port.

**rtmidi_create_virtual_outport(name)**
ARGUMENTS: name, string
RETURNS: none

Creates a virtual MIDI outport for other software to connect to. The supplied string will be the name other software will use for the port.




## How to put together a MIDI message:
Most MIDI messages are comprised of three bytes - a status byte, followed by one or two data bytes. The status byte says what MIDI channel the information is for and what kind of message it is, and the data bytes contain the actual information. These bytes are usually written in hexadecimal, but I personally find that kind of clumsy so I’ll be using integers here(hexadecimal is accepted in GMSSimpleMIDI, and while I haven’t extensively tested it, basic tests show it works just fine - and if you’re comfortable using hexadecimal, you probably don’t need to read what I’ve written here). In this example, we’re going to make a MIDI message that says, “For the device on channel 5, turn on note C4 with a velocity of 80”.

The status byte can be thought of as a number representing the command, plus the channel that command is for. Channels are, somewhat confusingly, referred to with numbers 1-16, but logically in code use numbers 0-15. Note On is number 144, so if we want to send a Note on for channel 5 (logically referred to as channel 4), we’d use the status byte 144+4 = 148.

In MIDI, notes are referred to by numbers 0-127. 0 represents C0 (the C note one octave lower than is available on a standard 88-key piano keyboard) and each number increases the pitch by one half-step(each piano key, including the black keys, represents one half-step - each fret on a guitar is also one half-step), making 127 G10 (three octaves higher than the highest G on a piano). C4, or middle C, is represented by the number 48. So to say “Channel 5, turn on C4” we’d use the numbers “148 48”.

But we’re not quite done! Midi note messages have three bytes. The third byte is the velocity - if the MIDI data is generated by a keyboard, this usually corresponds to how hard the key was pressed and is used to determine the volume of the note(although it doesn’t have to - most synths let you patch velocity to any parameter you choose, and if you’re programming notes into a sequencer, velocity is whatever arbitrary number you pick). Like note data, velocity is between 0-127, where a higher number means a harder keypress. We’re just going to pick a middle-ish velocity of 80.

This makes our final MIDI message “148 48 80”, which a synthesizer will interpret as “If you’re set to receive data on channel 5, start note C4 at a velocity of 80”. In GMSSimpleMIDI, the function would look like rtmidi_send_message(148,48,80). MIDI requires that a “note off” message be sent at a later point, otherwise the note will play forever (or at least until the synth is turned off). “Note off” messages start at 128, and use the same channel system as “note on”, so you can probably figure out how to tell it to turn off the note. “Note off” messages also require a velocity - this is known as the release velocity, and while many synths and instruments don’t recognize it, it’s still required. 0 is a safe value to use.

There are lots of specific messages that can be sent over MIDI, and lots more that are defined by the user, or by the receiving device manufacturer. These messages are called “control change” or “continuous controller” messages(CC for short), and their status byte is 176 (plus the channel). CC messages are usually used to control different instrument/synthesizer parameters, and while there is some standardization (CC 33 is always the modulation wheel position) there’s lots of variation between devices. For CC messages, the status byte says what it is (a CC message) and what channel it’s on, just like note. The second byte is 0-127, and says what CC you’re sending data for, and the third byte, also 0-127, is the actual data. rtmidi_send_message(180,33,63) is “For the synth on channel 5, the mod wheel position is directly in the middle”.

Knowing how to put together messages also tells us how to interpret them when retrieved with rtmidi_check_message() and rtmidi_get_message(). First, we get the general range of the first status byte - any status byte greater than 143 and less than 160 is a “Note On” message, and subtracting 144 from the status byte will tell us what channel it’s for. We now know that the next two bytes represent the note and how hard that note was pressed. We can even use the number of bytes in a message to help determine what kind of message it is - channel aftertouch and program change messages only have one data byte, while timing messages contain only one byte, the status byte.

Intricate details of the MIDI protocol are outside the scope of this, but there’s no shortage of detailed information a quick search away. The basics covered here, note on, note off, and CC, covers the vast majority of what most people use MIDI for.
