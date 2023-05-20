import mido
from mido import MetaMessage
import time
filename=input()
f=mido.MidiFile(filename)

absolute_time=0
meta_messages=False

start_time = time.time()
input_time = 0.0
for msg in f:
	input_time += msg.time

	playback_time = time.time() - start_time
	duration_to_next_event = input_time - playback_time

	#if duration_to_next_event > 0.0:
	#       time.sleep(duration_to_next_event)

	if isinstance(msg, MetaMessage) and not meta_messages:
		#if(msg.type=="set_tempo"):
		#	print("tempo ",mido.tempo2bpm(msg.tempo))
		print(msg)
		continue
	else:
		print(input_time,msg.hex())
