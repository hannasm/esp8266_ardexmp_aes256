#ifndef base64cC_H
#define base64cC_H

#include <stddef.h>
#include <stdint.h>

/**
 * base64c_encoding_length - calculate length to allocate for encode
 * @len: Length of input string
 * Returns: number of bytes required to base64c encode, this includes room for  '\0' terminator
 */
size_t base64c_encoding_length(size_t len);

/**
 * base64c_decoding_length - calculate length to allocate for decode
 * @len: Length of (base64 encoded) input string
 * Returns: maximum number of bytes required to decode
 */
size_t base64c_decoding_length(size_t inlen);

/**
 * base64c_encode - base64c encode
 * @src: Data to be encoded
 * @len: Length of the data to be encoded
 * @out: Mutable output buffer destination, all encoded bytes will be written to the destination
 * @out_len: length of output buffer
 * Returns: number of bytes written, or -1 if there was an error
 */
size_t base64c_encode(const unsigned char *src, size_t len, unsigned char* out, const size_t out_len);

/**
 * base64c_decode - base64c decode
 * @src: Data to be decoded
 * @len: Length of the data to be decoded
 * @out_len: Pointer to output length variable
 * Returns: Allocated buffer of out_len bytes of decoded data,
 * or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
size_t base64c_decode(const unsigned char *src, size_t len, unsigned char *out, const size_t out_len);
#endif
