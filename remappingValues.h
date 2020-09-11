

#ifndef REMAPPINGVALUES_H_INCLUDED
#define REMAPPINGVALUES_H_INCLUDED

unsigned char remapvalue_ub(unsigned char x, unsigned char in_min, unsigned char in_max, unsigned char out_min, unsigned char out_max);
char remapvalue_b(char x, char in_min, char in_max, char out_min, char out_max);
int remapvalue_i(int x, int in_min, int in_max, int out_min, int out_max);
float remapvalue_f(float x, float in_min, float in_max, float out_min, float out_max);

#endif // REMAPPINGVALUES_H_INCLUDED
