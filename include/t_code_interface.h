#ifndef _T_CODE_INTERFACE_H
#define _T_CODE_INTERFACE_H


#define SOURCE_SYM_BITSIZE 6
#define ENCODED_SYM_BITSIZE 24

#define aligned_size(BufLen, SymLen) ((BufLen/SymLen + (BufLen%SymLen != 0))*SymLen)
#define eval_encoded_buf_by_input(InByteSize) (aligned_size(aligned_size(InByteSize*8, SOURCE_SYM_BITSIZE)/SOURCE_SYM_BITSIZE*ENCODED_SYM_BITSIZE, 8)/8)
#define eval_decoded_buf_by_input(InByteSize) (aligned_size(((InByteSize*8/ENCODED_SYM_BITSIZE)*SOURCE_SYM_BITSIZE), 8)/8)
#define max(A,B) (((A)>(B))?A:B)

typedef char SourceSymbol [SOURCE_SYM_BITSIZE];
typedef char EncodedSymbol [ENCODED_SYM_BITSIZE];
typedef unsigned char BYTE;



/**
* @brief encode one plain 6-bit symbol.
*
* @param src [IN] - plain symbol to be encoded
* @param dst [OUT] - encoded symbol
*
* @return 0 if success
*/
int fec_encode_sym(SourceSymbol src, EncodedSymbol *dst);

/**
* @brief decode one encoded 24-bit symbol
*
* @param src [IN] - encoded symbol to be decoded
* @param dst [OUT] - decoded symbol
*
* @return 0 if success
*/
int fec_decode_sym(EncodedSymbol src, SourceSymbol *dst);

/**
* @brief encode plain byte buffer
*
* @param src [IN] - plain buffer to be encoded
* @param src_bytelen [IN] - length of plain buffer
* @param dst [OUT] - storage for encoding
* @param dst_bytelen [IN] - byte size of storage
*
* @return lenght of encoded buffer or -1
*/
int fec_encode_buffer(BYTE *src, int src_bytelen, BYTE *dst, int dst_bytelen);

/**
* @brief decode encoded buffer
*
* @param src [IN] - buffer to be decoded
* @param src_bytelen [IN] - lenght of encoded buffer
* @param dst [OUT] - storage for decoding
* @param dst_bytelen [IN] - byte size of storage
*
* @return length of decoded buffer or -1
*/
int fec_decode_buffer(BYTE *src, int src_bytelen, BYTE *dst, int dst_bytelen);
#endif
