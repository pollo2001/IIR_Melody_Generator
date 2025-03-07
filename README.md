This project generates melodies using IIR (Infinite Impulse Response) filters to process the signal. It is designed to take melody and chorus notes, apply filtering, and save the output to a .wav file. The melody is based on the classic "Twinkle Twinkle Little Star" song 🧸✨, but it could easily be adapted for any melody. It was optimized for performance with minimal memory usage 🧠💻.

****************************How It Works 🚀****************************
Generate Notes 🎵:
The program generates a melody using predefined frequencies for each note. It uses sinusoidal waves to generate each note based on frequency and duration 🌀🎶.


Melody & Chorus Generation 🎼:
The melody (x_m) and chorus (x_c) are arrays of notes with predefined frequencies. For each note, a sine wave is generated and stored in memory.
IIR Filter 🧮:

The melody is processed using an IIR filter that applies weighting between the melody and the chorus 🎵 ➕ 🎶 = ❤️. The filter is defined by numerator (a) and denominator (b) coefficients.
Write to .WAV 📝🎧:

After processing the melody with the IIR filter, the result is written to a .wav file using the libsndfile library 🎧💾.

Main Components 🔧:
generate_note(f, t): Generates a sine wave for a given frequency f and duration t 🎤.
generate_melody(L, t, in): Creates an array of notes (melody or chorus) using generate_note 🔁🎼.
apply_weights(x1, x2, t, L, w1, w2): Applies a weighting factor to combine the melody and chorus into a new signal 🎶🤝🎵.
iir_filter(x, t, L, a, b): Applies an IIR filter to the melody/chorus signal 🎛️.
write_to_wav(filename, melody, num_samples): Saves the filtered melody as a .wav file 🎶📂.
Steps Taken 🔄:
Generate Melody and Chorus:
Melody (x_m) and chorus (x_c) arrays are created with predefined frequencies.

Apply Weights to Melody and Chorus:
The program applies weights (w1 = 0.4, w2 = 0.6) to blend the two melodies into a single output 🎧.

Apply IIR Filter:
The output of the weighted melody and chorus is filtered using the IIR filter with coefficients to shape the sound 🎚️.

Save to .wav File:
The final melody is saved to a .wav file for playback 🎼🎧.

Technical Details 📊:
Sampling Rate: 8000 Hz (Fs) 🌐
Melody Length: 32 notes 🎶
Filter Order: 4 for both numerator and denominator coefficients 🎚️.
Processing Time: 0.0356 seconds per melody (optimized for speed 🏎️).
Dependencies 📦:
libsndfile: For reading and writing .wav files 🎧💾
math.h: For sine wave generation and math functions 🧮

Usage Example 🎬:
To compile the code:

gcc Lab1_starter_code.c -o starter_out -lsndfile   //make sure you have this.. homebrew for mac!
Run the program:

Run the Program:
./starter_out
This will generate the filtered melody and save it as "name.wav". 🎶💾

Conclusion 🌟:
This project demonstrates the process of generating melodies, applying filters to audio signals, and saving them as .wav files. The IIR filter ensures that the melody sounds smooth and cohesive. You can easily adapt this project for your own melodies or experiment with different filter parameters! 🎶🔧

