#include <stdio.h>
#include "t_code_interface.h"


int main()
{
	int i, j, k;
	SourceSymbol X = {1,1,0,0,1,0}, XX = {0};
	EncodedSymbol Y = {0};

	printf("\n============== Test1 ==================\n");
	printf("Plain:\n");
	for(i = 0; i < SOURCE_SYM_BITSIZE; i++)printf("%d ", X[i]);
	printf("\n");
	fec_encode_sym(X, &Y);
	printf("Encoded:\n");
	for(i = 0; i < ENCODED_SYM_BITSIZE; i++)printf("%d ", Y[i]);
	printf("\n");

	Y[1] ^= 1; Y[3]^= 1; Y[7]^= 1;
	printf("Noised:\n");
	for(i = 0; i < ENCODED_SYM_BITSIZE; i++)printf("%d ", Y[i]);
	printf("\n");

	fec_decode_sym(Y, &XX);
	printf("Decoded:\n");
	for(i = 0; i < SOURCE_SYM_BITSIZE; i++)printf("%d ", XX[i]);
	printf("\n");

	printf("\n============== Test2 ==================\n");
	BYTE cX[] = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'A', 'n', 'i', 'm', 'a', 'n', 't', 'e'};
	BYTE cXX[eval_decoded_buf_by_input(eval_encoded_buf_by_input(sizeof(cX)))] = {0};
	BYTE cY[eval_encoded_buf_by_input(sizeof(cX))] = {0};
	int ret_len;

	printf("Plain text (%d -> %d ->%d):\n", sizeof(cX), eval_encoded_buf_by_input(sizeof(cX)), eval_decoded_buf_by_input(eval_encoded_buf_by_input(sizeof(cX))));
	for(i = 0; i < sizeof(cX); i++) printf("%02x ", cX[i]);
	printf("\n");

	ret_len = fec_encode_buffer(cX, sizeof(cX), cY, sizeof(cY));
	if(ret_len == -1)
	{
		printf("Error: receiving buffer is tooo small\n");
	}

	printf("Encoded text:\n");
	for(i = 0; i < ret_len; i++) printf("%02x ", cY[i]);
	printf("\n");

	ret_len = fec_decode_buffer(cY, sizeof(cY), cXX, sizeof(cXX));
	if(ret_len == -1)
	{
		printf("Error: receiving buffer is tooo small\n");
	}
	printf("Decoded text:\n");
	for(i = 0; i < ret_len; i++) printf("%02x ", cXX[i]);
	printf("\n");
	return 0;
}
