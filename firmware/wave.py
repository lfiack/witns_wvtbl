from scipy.io.wavfile import read
import matplotlib.pyplot as plt

table_sample_in = 2048
table_sample_out = 512
table_x_len = 8
table_y_len = 8

f = open("table.c", "w")

plt.rcParams["figure.figsize"] = [7.50, 3.50]
plt.rcParams["figure.autolayout"] = True

input_data = read("/home/laurentf/Téléchargements/wavetables/BassTables/BassTables 01.wav")
#input_data = read("/home/laurentf/Téléchargements/wavetables/BassTables Vol. 2/BassTables 31.wav")
#input_data = read("/home/laurentf/Téléchargements/wavetables/BassTables Vol. 2/Mandatory 1.wav")
#input_data = read("/home/laurentf/Téléchargements/wavetables/Wavetables_2048/wt10.wav")

audio = input_data[1]

sp_max = audio.max()
sp_min = audio.min()
wave_nb = int(audio.size / table_sample_in)

print("Max = " + str(audio.max()))
print("Min = " + str(audio.min()))
print("Length = " + str(audio.size))
print("Number of waves = " + str(wave_nb))

audio = audio.astype(float)
audio = audio - audio.min()
audio = audio * 4095 / audio.max()

audio = audio[0::int(table_sample_in/table_sample_out)]
wave_nb = int(audio.size / 512)

print("Max = " + str(audio.max()))
print("Min = " + str(audio.min()))
print("Length = " + str(audio.size))
print("Number of waves = " + str(wave_nb))

increment = int(wave_nb / 8)
print("increment = ", str(increment))
audio = audio.astype(int)

for x in range(table_x_len):
    f.write("\t\t{")
    
    for spl in (audio[x * increment * table_sample_out:((x * increment + 1) * table_sample_out)]):
        f.write(str(spl)) 
    
        f.write(",")

    
    if (x == table_x_len-1):
        f.write("}\n")
    else:
        f.write("},\n")

    plt.plot(audio[x * increment * table_sample_out:((x * increment + 1) * table_sample_out) - 1])
plt.ylabel("Amplitude")
plt.xlabel("Time")
plt.show()
