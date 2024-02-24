#sudo apt -y install libasound2-dev
g++ -Wall -D__LINUX_ALSA__ -std=c++14 -fPIC -O2 -c ./RtMidi.cpp &&
g++ -Wall -std=c++14 -fPIC -O2 -c ./GMSSimpleMIDI.cpp &&
g++ -shared -o libmidistuff.so ./GMSSimpleMIDI.o ./RtMidi.o -lasound -lpthread
