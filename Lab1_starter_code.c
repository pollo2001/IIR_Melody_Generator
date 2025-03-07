#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sndfile.h>
#include <math.h>

#define Fs 8000
#define ORDER_A 4
#define ORDER_B 4


#define dob 220
#define reb 248
#define mib 275
#define fab 293
#define sob 330
#define lab 367
#define tib 206

#define do 440
#define re 495
#define mi 550
#define fa 587
#define so 660
#define la 733
#define ti 825
#define nu 0


// Declare function
float* generate_note(float f, float t);
float** generate_melody(int L, float t, float* in);
void hex_dump_melody(float t, int L, float** x);
void free_melody(float** melody,int n);
float* flatten(float **melody, int L, int X);
float ** apply_weights(float **x1, float** x2, float t, int L, float w1, float w2);
void iir_filter(float **x, float t, int L, float *a, float *b);
void write_to_wav(const char* filename, float* melody, int num_samples);

int main(int argc, char *argv[]) //no args, just write out to a .wav
{
	const char* output_filename = "C_output_without_filter.wav"; //output file

    // start here
	// load parameters and coefficients
	float t = 0.5;
	int L = 32; //length of 32 notes
    // allocate memory for melody note x_m
	float* x_m = calloc(L,sizeof(float)); //32 notes melody
	// allocate memory for chorus note x_c
	float* x_c = calloc(L,sizeof(float)); //32 notes chorus

	if (!x_m || !x_c) {
    printf("Memory allocation failed for melody or chorus!\n");
    return -1;
}

	// Your implementation
	//set x_m notes
 	x_m[0] = do; x_m[1] = nu; x_m[2] = do; x_m[3] = nu;
	x_m[4] = so; x_m[5] = nu; x_m[6] = so; x_m[7] = nu;
	x_m[8] = la; x_m[9] = nu; x_m[10] = la; x_m[11] = nu;
	x_m[12] = so; x_m[13] = nu; x_m[14] = nu; x_m[15] = nu;
	x_m[16] = fa; x_m[17] = nu; x_m[18] = fa; x_m[19] = nu;
	x_m[20] = mi; x_m[21] = nu; x_m[22] = mi; x_m[23] = nu;
	x_m[24] = re; x_m[25] = nu; x_m[26] = re; x_m[27] = nu;
	x_m[28] = do; x_m[29] = nu; x_m[30] = nu; x_m[31] = nu;
    // set x_cnotes
    x_c[0] = dob; x_c[1] = sob; x_c[2] = mib; x_c[3] = sob;
    x_c[4] = dob; x_c[5] = sob; x_c[6] = mib; x_c[7] = sob;
    x_c[8] = dob; x_c[9] = lab; x_c[10] = fab; x_c[11] = lab;
    x_c[12] = dob; x_c[13] = sob; x_c[14] = mib; x_c[15] = sob;
    x_c[16] = tib; x_c[17] = sob; x_c[18] = fab; x_c[19] = sob;
    x_c[20] = dob; x_c[21] = sob; x_c[22] = mib; x_c[23] = sob;
    x_c[24] = tib; x_c[25] = sob; x_c[26] = fab; x_c[27] = sob;
    x_c[28] = dob; x_c[29] = sob; x_c[30] = mib; x_c[31] = sob;

	//no need to pack, same  L
	float** chorus = generate_melody(L, t, x_c); //make chorus
	float** melody = generate_melody(L, t, x_m);	//make melody 
	if (!melody || !chorus) {
    printf("Failed to generate melody/chorus!\n");
    free(x_m);
    free(x_c);
    return -1; 
}
	float w2 = .6; float w1 = .4;
	float a[] = {0.62477732, -2.444978, 3.64114, -2.444978, 0.62477732}; //numerator
	float b[] = {1, -3.1820023, 3.9741082, -2.293354, 0.52460587}; //denominator

	float** output = apply_weights(chorus, melody, t, L, w1, w2);

	//apply filter 
	iir_filter(output, t, L, a, b);

	//hex_dump_melody(t, L, melody); //test line
	//write the output .wav
	write_to_wav(output_filename, flatten(output, L, (int)(L*Fs*t)), (int)(L* Fs *t));

	free(x_m);
	free(x_c);
	free_melody(melody, L);
	free_melody(chorus, L);
	free(output);
	return 1;
}

// Implement function
//input some frequency and time length and make a sin wave out of it, return the address to that sin wave
float* generate_note(float f, float t){
	
	int num_samples = (int)(Fs * t); // get sample count 
	float* note = calloc(num_samples, sizeof(float)); //make space for this note
	  
	if (!note) {
        printf("generate_note memory allocation failed! \n");
        return NULL;
    }
	
	if(!f || !num_samples) //save time and return early if zeror frequency is to read
	{
		return note;
	}

	for(int i = 0; i < num_samples; i++)
	{
		float t_val = (float)i/Fs; //time vector
		note[i] = sinf(2 * M_PI* f * t_val ); 
	}

	return note;  //return address of output
}

//this returns a pointer to an array of pointers
float** generate_melody(int L, float t, float* in){

	float** melody = calloc(L, sizeof(float*)); //pointer to pointers
	if(!melody)
		{
			printf("generate_melody failed to allocate \n");
			return NULL;
		}

	//generate note for each element of in
	for(int i = 0; i < L; i++){

		float * a_note = generate_note(in[i],t);
		melody[i] = a_note;

		if(!melody[i])
			{
				printf("error allocating note in meolody array \n");
				free_melody(melody,i); //only to the point we made it
				return NULL;
			}
	}
	return melody; //free melody with a stack pointer in main...
}

//clean up melody and notes during failure of after use
void free_melody(float** melody,int n)
{
	if(!melody) return;

	for(int i = 0; i<n; i++)
	{
		if(melody[i]) free(melody[i]); //free each note
	}

	free(melody); //free the pointer arrray
}

//apply weights
float** apply_weights(float **x1, float** x2, float t, int L, float w1, float w2) {
	//iterate through x1 and x2 , and add each of their ith a
	float **output  = calloc(L, sizeof(float*));
	if(!output)
		{
			printf("apply weights failed to allocate \n");
			return NULL;
		}

	int X = (int)(Fs * t); //all notes same length

		// allocate memory for each row in the output array (same length X)
    for (int i = 0; i < L; i++) {
        output[i] = calloc(X, sizeof(float)); 
        if (!output[i]) {
            printf("iir_filter failed to allocate memory for output row %d\n", i);
            // Free previously allocated rows and the output array on failure
            for (int j = 0; j < i; j++) {
                free(output[j]);
            }
            free(output);
            return NULL;
        }
    }


	for (int i = 0; i < L; i++)
	{	
		for (int j = 0; j < X; j++)
			{
				output[i][j] = w1 * (x1[i][j]) + w2 * (x2[i][j]);
			}
	}

	return output;
}

//turn 2D array into 1D for .WAV file
float* flatten(float **melody, int L, int X){

	float * flat_melody = calloc(L*X, sizeof(float)); //making this 2D array 1D
	if (!flat_melody) {
    printf("mem alloc failed to flatten melody\n");
    return NULL;
	}
	// Flatten the 2D array into the 1D array
    int idx = 0;
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < X; j++) {
            flat_melody[idx++] = melody[i][j];
        }
    }
    return flat_melody;
}

//hex dump for debug
void hex_dump_melody(float t, int L, float** x){
	
	FILE *hex_file = fopen("hex_dump.txt", "w");
    if (!hex_file) {
        printf("Failed to open file for writing hex dump!\n");
        return;
    }


	int X = (int)(Fs * t); //all notes same length
	for (int i = 0; i < L; i++)
	{
		uint32_t* p_first_note = (uint32_t*)&x[i][0];
		fprintf(hex_file,"Note %d: %08x \n\n", i, *p_first_note);

		for (int j = 0; j < X; j++)
			{
				 // yypecast the float value to an unsigned int pointer to see its raw bytes
				  uint32_t* p = (uint32_t*)&x[i][j]; //%08x prints the value as a 8-digit hex number
				  fprintf(hex_file, "%08x\n", *p);
			}
	}
	fprintf(hex_file, "Hex dump saved to: hex_dump.txt\n");
	fclose(hex_file);
}

//write signal to .wav
void write_to_wav(const char* filename, float* melody, int num_samples) {
    SF_INFO sndInfoOut;
    sndInfoOut.format = SF_FORMAT_WAV | SF_FORMAT_PCM_32;  //floating point
    sndInfoOut.channels = 1;       // mono channel (1 channel)
    sndInfoOut.samplerate = Fs;   

    SNDFILE* sndFileOut = sf_open(filename, SFM_WRITE, &sndInfoOut);
    if (!sndFileOut) {
        printf("Failed to open .wav file for writing\n");
        return;
    }

    // Write the flattened melody to the .wav file
    sf_writef_float(sndFileOut, melody, num_samples);

    // Close the file after writing
    sf_close(sndFileOut);

    // Confirm the save operation
    printf("Melody successfully written to: %s\n", filename);
}

//apply iir filter to signal
void iir_filter(float **x, float t, int L, float *a, float *b){

	//x is input, y is output
	// a and b are order 4
	float **y; //y is temp;

	for (int i = 0; i < L; i++) { 
        for (int j = 0; j < Fs*t; j++) {
            float sum_a = 0.0f, sum_b = 0.0f;

            //(feedforward: a coefficients)
            for (int k = 0; k <= ORDER_A; k++) {
                if (j - k >= 0) { // avoid negative indexing
                    sum_a += a[k] * x[i][j - k];
                }
            }

            // feedback: b coefficients)
            for (int k = 1; k <= ORDER_B; k++) {
                if (j - k >= 0) { // avoid negative indexing
                    sum_b += b[k] * y[i][j - k];
                }
            }

            y[i][j] = sum_a - sum_b; //diff eq
        }
    }

	return;
}
//gcc Lab1_starter_code.c -o starter_out -lsndfile