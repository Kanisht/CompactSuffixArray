using namespace std;
#include <divsufsort.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "rank9b.h"

#define VALUE_A 0
#define VALUE_T 1
#define VALUE_G 2
#define VALUE_C 3

static inline unsigned char get_value(unsigned char *data, long index) {
	return (data[index/4] >> ((index % 4) * 2)) & 3;
}
	

int main(int argc, char *argv[]) {
	char *in;
	char tmp;
	unsigned char *data = NULL;
	saidx_t *SA;
	unsigned long actual_size = 0;
	unsigned long in_size = 0;
	unsigned long packed_size;
	string pat;
	unsigned long low = 0;
	unsigned long high = 0;
	unsigned long mid = 0;
	unsigned long val = 0;
	char cval;
        rank9b *rds;
	uint64_t *bit_arr;
	uint64_t start_rank;
	FILE *fp = NULL;
	char *temp_buf = NULL;
	int read = 0;
	struct stat stat_buf;

	temp_buf = (char *)malloc(500000);
	if (temp_buf == NULL)
		return 1;

	fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("Could not open file\n");
		return 1;
	}
	int rc = stat(argv[1], &stat_buf);

	in = (char *)malloc(stat_buf.st_size);
	if (!in)
		return 1;
	bit_arr = (uint64_t *) malloc(stat_buf.st_size/8 + 8);
	while(1) {
		read = fscanf(fp, "%s", temp_buf);
		if(read < 1)
			break;
		memset(temp_buf, 0, 500000);
		fscanf(fp, "%s", temp_buf);
		strcpy(in + actual_size, temp_buf);
		actual_size = actual_size + strlen(temp_buf);
		bit_arr[(actual_size - 1)/64] |= (1 << ((actual_size - 1)%64));
	}
	in_size = actual_size;
	packed_size = in_size/4 + 1;
	/*for (int i = 0; i < actual_size; i++) {
		if (in[i] != '#') {
			in_size++;
		}
	}
	packed_size = in_size/4 + 1;
	bit_arr = (uint64_t *) malloc(in_size/8+8);
	in = (char *) malloc(actual_size+1);
	strcpy(in,in1);
	for (int i = 0, j = 0; i < actual_size; i++, j++) {
		if (in[i] != '#' && j != i) {
			in[j] = in[i];
		}
		else if(in[i] == '#') {
			j--;
			//set j'th bit of bit_arr
			bit_arr[j/64] |= (1 << (j%64));
		}
	}*/
	rds = new rank9b(bit_arr, in_size);

	SA = (saidx_t *) malloc(actual_size * sizeof(saidx_t));
	if (SA == NULL)
		return 1;
	saint_t ret = divsufsort((unsigned char *) in, SA, in_size);
	
	data = (unsigned char *) malloc(packed_size);
	if (data == NULL) {
		return 1;
	}
	memset(data, 0, packed_size);
	for (int i = 0; i < in_size; i++) {
		if (in[i] == 'A') {
			data[i/4] = data[i/4] | (VALUE_A <<((i % 4) * 2));
		}
		else if(in[i] == 'T') {
                        data[i/4] = data[i/4] | (VALUE_T <<((i % 4) * 2));
                }
		else if (in[i] == 'G') {
                        data[i/4] = data[i/4] | (VALUE_G <<((i % 4) * 2));
                }
		else if (in[i] == 'C') {
                        data[i/4] = data[i/4] | (VALUE_C <<((i % 4) * 2));
                }
	}
	/*for (int i = 0; i < in_size; i++) {
		if (get_value(data, i) == VALUE_A) 
			cout<<"A";
		else if (get_value(data, i) == VALUE_T)
                        cout<<"T";
		else if (get_value(data, i) == VALUE_G)
                        cout<<"G";
		else
			cout<<"C";
	}*/
	free(in);
	free(temp_buf);
	while(1) {
		cout<<"Please enter a pattern or enter 'q' to quit\n";
		cin>>pat;
		if (pat.at(0) == 'q')
			break;
		
		high = in_size - 1;
		int i;
		while (low <= high)
		{
			mid = low + (high - low) / 2;
			start_rank = rds->rank(SA[mid]);
			for (i = 0; i < pat.length() && SA[mid] + i < in_size; i++)
			{
				val = get_value(data, SA[mid] + i);
				if (val == VALUE_A) {
					cval = 'A';
				}
				else if (val == VALUE_T) {
                	                cval = 'T';
                        	}
				else if (val == VALUE_G) {
	                                cval = 'G';
        	                }
				else {
					cval = 'C';
				}
				if (cval == pat.at(i) && start_rank == rds->rank(SA[mid] + i)) {
					continue;
				}
				else {
					break;
				}
			}
			if (i == pat.length())
			{
				cout << "Pattern found at index "<< SA[mid]<<"\n";
				break;
			}
 
			if (pat.at(i) - cval < 0) {
				high = mid - 1;
			} 
			else {
				low = mid + 1;
			}
		}
	}
	return 0;
}
