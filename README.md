
# GMSSimpleMIDI


A DLL for using RtMidi with Gamemaker Studio

GMSSimpleMIDI uses the RtMidi library to enable sending and receiving MIDI messages in Gamemaker Studio. This allows the creation of sequencers or arpeggiators in Gamemaker, or the use of MIDI controllers for game input. Any kind of message can be received, including any sysex message, and standard messages of up to three bytes can be sent. See documentation.md for details.

GMSSimpleMIDI is available on the Yoyogames marketplace as a full extension that also includes a couple basic utility scripts, and the source for a simple example arpeggiator that can use an internal or external clock. The extension only has a Windows DLL available, since I don't have regular access to a Mac system to compile and test a .dylib, but the source should be platform agnostic so I'm making the code freely available here if anyone wants to recompile and use it for Mac or Linux. Check out the RtMidi site for compilation info:https://www.music.mcgill.ca/~gary/rtmidi/index.html#compiling Feel free to reach out if you have any questions or suggestions!

The full extension, ready to be used in Gamemaker Studio projects, is available on the [YoYoGames Marketplace] and [Itch.io.]

[YoYoGames Marketplace]:https://marketplace.yoyogames.com/assets/9499/midi-with-gmssimplemidi
[Itch.io.]:https://rhymingarfunkle.itch.io/gmssimplemidi

Thanks to [Grix] for Mac binaries! I can't personally test them or provide official support for them, but they're there for anyone who doesn't want to compile their own.
[Grix]:https://github.com/Grix
