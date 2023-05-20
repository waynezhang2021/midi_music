#include<bits/stdc++.h>
#include<windows.h> 
using namespace std;
double reference;
inline HMIDIOUT midi_open()
{
	HMIDIOUT h;
	midiOutOpen(&h,0,0,0,0);
	return h;
}
inline void midi_close(HMIDIOUT h)
{
	midiOutClose(h);
	return;
}
BYTE translate_note(string note,short transpose=0)
{
	BYTE offset[7]={9,11,0,2,4,5,7};
	if(note[1]=='#')
		return 12*stod(note.substr(2))+offset[note[0]-'A']+transpose+13;
	else
		return 12*stod(note.substr(1))+offset[note[0]-'A']+transpose+12;
}
void note_on(HMIDIOUT h,BYTE volume,string note,short transpose,BYTE channel)
{
	midiOutShortMsg(h,(volume<<16)|(translate_note(note,transpose)<<8)|0x90|channel);
	return;
}
void note_off(HMIDIOUT h,BYTE volume,string note,short transpose,BYTE channel)
{
	midiOutShortMsg(h,(volume<<16)|(translate_note(note,transpose)<<8)|0x80|channel);
	return;	
}
inline double precise_clock()
{
	return chrono::high_resolution_clock::now().time_since_epoch().count()/1000000000.0-reference;
}
int main()
{
	HMIDIOUT midi_device;
	midi_device=midi_open();
	string file,line;
	stringstream ss;
	ifstream fin;
	double start,end,duration;//duration is not used for anything, just the file contains it
	string note;
	int volume;
	short transpose;
	double time_multiplier;
	vector<pair<string,double>> active_notes;
	while(1)
	{
		cout<<"play CSV file:";
		cin>>file;
		cout<<"transpose:";
		cin>>transpose;
		cout<<"time multiplier:";
		cin>>time_multiplier;
		fin.open(file);
		getline(fin,line);
		reference=0;
		reference=precise_clock();
		while(getline(fin,line))
		{
			ss.clear();
			ss<<line;
			ss>>note>>start>>end>>duration>>volume;
			while(precise_clock()*time_multiplier<start)
			{
again:
				for(pair<string,double> p:active_notes)
					if(precise_clock()*time_multiplier>=p.second)
					{
						note_off(midi_device,0,p.first,transpose,0);
						active_notes.erase(find(active_notes.begin(),active_notes.end(),p));
						goto again;
					}
			}
			note_on(midi_device,volume,note,transpose,0);
			cout<<note<<" ";
			active_notes.push_back(make_pair(note,end));
		}
		while(!active_notes.empty())
		{
again2:
			for(pair<string,double> p:active_notes)
				if(precise_clock()*time_multiplier>=p.second)
				{
					note_off(midi_device,0,p.first,transpose,0);
					active_notes.erase(find(active_notes.begin(),active_notes.end(),p));
					goto again2;
				}
		}
		system("cls");
		fin.close();
	}
}
