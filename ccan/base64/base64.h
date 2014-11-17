/* Licensed under LGPLv2+ - see LICENSE file for details */
#ifndef CCAN_BASE64_H
#define CCAN_BASE64_H

#include <stdio.h> /* For size_t */
#include <stdbool.h>

/**
 * base64_alphabet_rfc4648 - the base64 alphabet as defined in rfc4648
 */
static const char base64_alphabet_rfc4648[] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**
 * base64_alphabet_t - structure to hold maps for encode/decode
 */
typedef struct {
	char encode_map[64];
	signed char decode_map[256];
} base64_alphabet_t;

/**
 * base64_encoded_length - Calculate encode buffer length
 * @param srclen the size of the data to be encoded
 * @note add 1 to this to get null-termination
 * @return Buffer length required for encode
 */
size_t base64_encoded_length(size_t srclen);

/**
 * base64_encode - Encode a buffer into base64 according to rfc4648
 * @param dest Buffer to encode into
 * @param destlen Length of the destination buffer
 * @param src Buffer to encode
 * @param srclen Length of the data to encode
 * @return Number of encoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen
 *
 * This function encodes src according to http://tools.ietf.org/html/rfc4648
 *
 * Example:
 *	size_t encoded_length;
 *	char dest[100];
 *	const char *src = "This string gets encoded";
 *	encoded_length = base64_encode(dest, sizeof(dest), src, strlen(src));
 *	printf("Returned data of length %zd @%p\n", encoded_length, &dest);
 */
size_t base64_encode(char *dest, const size_t destlen, const char *src, const size_t srclen);

/**
 * base64_decoded_length - Calculate decode buffer length
 * @param srclen Length of the data to be decoded
 * @note This does not return the size of the decoded data!  see base64_decode
 * @return Minimum buffer length for safe decode
 */
size_t base64_decoded_length(size_t srclen);

/**
 * base64_decode - decode An rfc4648 base64-encoded string
 * @param dest Buffer to decode into
 * @param destlen Length of the destination buffer
 * @param src Buffer to decode
 * @param srclen Length of the data to decode
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen
 *
 * This function decodes the buffer according to
 * http://tools.ietf.org/html/rfc4648
 *
 * Example:
 *	size_t decoded_length;
 *	char ret[100];
 *	const char *src = "Zm9vYmFyYmF6";
 *	decoded_length = base64_decode(ret, sizeof(ret), src, strlen(src));
 *	printf("Returned data of length %zd @%p\n", decoded_length, &ret);
 */
size_t base64_decode(char *dest, const size_t destlen,
		     const char *src, const size_t srclen);


/**
 * base64_encode_triplet - encode 3 bytes into base64
 * @param dest Buffer containing at least 4 bytes
 * @param src Buffer containing 3 bytes
 */
void base64_encode_triplet(char dest[4], const char src[3]);

/**
 * base64_encode_tail - encode the final bytes of a source
 * @param dest Buffer containing at least 4 bytes
 * @param src Buffer containing at least srclen bytes
 * @param srclen Number of bytes to encode in src
 */
void base64_encode_tail(char dest[4], const char *src, const size_t srclen);


/**
 * base64_decode_quartet - decode the first 4 characters in src into dest
 * @param dest Buffer containing at least 3 bytes
 * @param src Buffer containing at least 4 characters
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 */
int base64_decode_quartet(char dest[3], const char src[4]);

/**
 * @brief decode the final bytes of a base64 string from src into dest
 * @param dest Buffer containing 3 bytes
 * @param src Buffer containing 4 bytes - padded with '=' as required
 * @param srclen Number of bytes to decode in src
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 */
size_t base64_decode_tail(char dest[3], const char src[4], const size_t srclen);


/**
 * base64_encode_triplet_using_alphabet - encode 3 bytes into base64 using a specific alphabet
 * @param alphabet Alphabet to use for encoding
 * @param dest Buffer containing at least 3 bytes
 * @param src Buffer containing at least 4 characters
 */
void base64_encode_triplet_using_alphabet(const base64_alphabet_t *alphabet,
					  char dest[4], const char src[3]);

/**
 * base64_encode_tail_using_alphabet - encode the final bytes of a source using a specific alphabet
 * @param alphabet Alphabet to use for encoding
 * @param dest Buffer containing at least 4 bytes
 * @param src Buffer containing at least srclen bytes
 * @param srclen Number of bytes to encode in src
 */
void base64_encode_tail_using_alphabet(const base64_alphabet_t *alphabet,
				       char dest[4],
				       const char *src, const size_t srclen);

/**
 * base64_encode_using_alphabet - encode a buffer into base64 using a specific alphabet
 * @param alphabet Alphabet to use for encoding
 * @param dest Buffer to encode into
 * @param destlen Length of dest
 * @param src Buffer to encode
 * @param srclen Length of the data to encode
 * @return Number of encoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen
 */
size_t base64_encode_using_alphabet(const base64_alphabet_t *alphabet,
				    char *dest, const size_t destlen,
				    const char *src, const size_t srclen);

/**
 * base64_init_alphabet - populate a base64_alphabet_t based on a supplied map
 * @param dest A base64 alphabet
 * @param src Map to populate the alphabet from (e.g. base64_alphabet_rfc4648)
 */
void base64_init_alphabet(base64_alphabet_t *dest, const char src[64]);

/*
 * base64_char_in_alphabet - returns true if character can be part of an encoded string
 * @param alphabet A base 64 alphabet (see base64_init_alphabet)
 * @param b64char Character to check
 */
bool base64_char_in_alphabet(const base64_alphabet_t *alphabet,
			     const char b64char);

/**
 * base64_decode_using_alphabet - decode a base64-encoded string using a specific alphabet
 * @param alphabet A base 64 alphabet (see base64_init_alphabet)
 * @param dest Buffer to decode into
 * @param destlen length of dest
 * @param src the buffer to decode
 * @param srclen the length of the data to decode
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 * @note dest will be nul-padded to destlen
 */
size_t base64_decode_using_alphabet(const base64_alphabet_t *alphabet,
				    char *dest, const size_t destlen,
				    const char *src, const size_t srclen);

/**
 * base64_decode_quartet_using_alphabet - decode 4 bytes from base64 using a specific alphabet
 * @param alphabet A base 64 alphabet (see base64_init_alphabet)
 * @param dest Buffer containing at least 3 bytes
 * @param src Buffer containing at least 4 characters
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 */
int base64_decode_quartet_using_alphabet(const base64_alphabet_t *alphabet,
					 char dest[3], const char src[4]);

/**
 * base64_decode_tail_using_alphabet - decode the final bytes of a base64 string using a specific alphabet
 * @param alphabet A base 64 alphabet (see base64_init_alphabet)
 * @param dest Buffer of at least 3 bytes
 * @param src Buffer containing 4 bytes - padded with '=' as required
 * @param srclen Number of bytes to decode in src
 * @return Number of decoded bytes set in dest. -1 on error (and errno set)
 */
int base64_decode_tail_using_alphabet(const base64_alphabet_t *alphabet,
				      char *dest,
				      const char *src, const size_t srclen);

#endif /* CCAN_BASE64_H */
