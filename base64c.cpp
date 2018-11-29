#include <string.h>
#include <stdio.h>

/*
 * Base64 encoding/decoding (RFC1341)
 * Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
static const unsigned char base64c_table[65] =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const unsigned char base64c_dtable[256] = {
  /*000*/0x80,/*001*/0x80,/*002*/0x80,/*003*/0x80,/*004*/0x80,/*005*/0x80,/*006*/0x80,/*007*/0x80,/*008*/0x80,/*009*/0x80,/*010*/0x80,/*011*/0x80,/*012*/0x80,/*013*/0x80,/*014*/0x80,/*015*/0x80,/*016*/0x80,/*017*/0x80,/*018*/0x80,/*019*/0x80,
  /*020*/0x80,/*021*/0x80,/*022*/0x80,/*023*/0x80,/*024*/0x80,/*025*/0x80,/*026*/0x80,/*027*/0x80,/*028*/0x80,/*029*/0x80,/*030*/0x80,/*031*/0x80,/*032*/0x80,/*033*/0x80,/*034*/0x80,/*035*/0x80,/*036*/0x80,/*037*/0x80,/*038*/0x80,/*039*/0x80,
  /*040*/0x80,/*041*/0x80,/*042*/0x80,/*043*/0x3e,/*044*/0x80,/*045*/0x80,/*046*/0x80,/*047*/0x3f,/*048*/0x34,/*049*/0x35,/*050*/0x36,/*051*/0x37,/*052*/0x38,/*053*/0x39,/*054*/0x3a,/*055*/0x3b,/*056*/0x3c,/*057*/0x3d,/*058*/0x80,/*059*/0x80,
  /*060*/0x80,/*061*/0x00,/*062*/0x80,/*063*/0x80,/*064*/0x80,/*065*/0x00,/*066*/0x01,/*067*/0x02,/*068*/0x03,/*069*/0x04,/*070*/0x05,/*071*/0x06,/*072*/0x07,/*073*/0x08,/*074*/0x09,/*075*/0x0a,/*076*/0x0b,/*077*/0x0c,/*078*/0x0d,/*079*/0x0e,
  /*080*/0x0f,/*081*/0x10,/*082*/0x11,/*083*/0x12,/*084*/0x13,/*085*/0x14,/*086*/0x15,/*087*/0x16,/*088*/0x17,/*089*/0x18,/*090*/0x19,/*091*/0x80,/*092*/0x80,/*093*/0x80,/*094*/0x80,/*095*/0x80,/*096*/0x80,/*097*/0x1a,/*098*/0x1b,/*099*/0x1c,
  /*100*/0x1d,/*101*/0x1e,/*102*/0x1f,/*103*/0x20,/*104*/0x21,/*105*/0x22,/*106*/0x23,/*107*/0x24,/*108*/0x25,/*109*/0x26,/*110*/0x27,/*111*/0x28,/*112*/0x29,/*113*/0x2a,/*114*/0x2b,/*115*/0x2c,/*116*/0x2d,/*117*/0x2e,/*118*/0x2f,/*119*/0x30,
  /*120*/0x31,/*121*/0x32,/*122*/0x33,/*123*/0x80,/*124*/0x80,/*125*/0x80,/*126*/0x80,/*127*/0x80,/*128*/0x80,/*129*/0x80,/*130*/0x80,/*131*/0x80,/*132*/0x80,/*133*/0x80,/*134*/0x80,/*135*/0x80,/*136*/0x80,/*137*/0x80,/*138*/0x80,/*139*/0x80,
  /*140*/0x80,/*141*/0x80,/*142*/0x80,/*143*/0x80,/*144*/0x80,/*145*/0x80,/*146*/0x80,/*147*/0x80,/*148*/0x80,/*149*/0x80,/*150*/0x80,/*151*/0x80,/*152*/0x80,/*153*/0x80,/*154*/0x80,/*155*/0x80,/*156*/0x80,/*157*/0x80,/*158*/0x80,/*159*/0x80,
  /*160*/0x80,/*161*/0x80,/*162*/0x80,/*163*/0x80,/*164*/0x80,/*165*/0x80,/*166*/0x80,/*167*/0x80,/*168*/0x80,/*169*/0x80,/*170*/0x80,/*171*/0x80,/*172*/0x80,/*173*/0x80,/*174*/0x80,/*175*/0x80,/*176*/0x80,/*177*/0x80,/*178*/0x80,/*179*/0x80,
  /*180*/0x80,/*181*/0x80,/*182*/0x80,/*183*/0x80,/*184*/0x80,/*185*/0x80,/*186*/0x80,/*187*/0x80,/*188*/0x80,/*189*/0x80,/*190*/0x80,/*191*/0x80,/*192*/0x80,/*193*/0x80,/*194*/0x80,/*195*/0x80,/*196*/0x80,/*197*/0x80,/*198*/0x80,/*199*/0x80,
  /*200*/0x80,/*201*/0x80,/*202*/0x80,/*203*/0x80,/*204*/0x80,/*205*/0x80,/*206*/0x80,/*207*/0x80,/*208*/0x80,/*209*/0x80,/*210*/0x80,/*211*/0x80,/*212*/0x80,/*213*/0x80,/*214*/0x80,/*215*/0x80,/*216*/0x80,/*217*/0x80,/*218*/0x80,/*219*/0x80,
  /*220*/0x80,/*221*/0x80,/*222*/0x80,/*223*/0x80,/*224*/0x80,/*225*/0x80,/*226*/0x80,/*227*/0x80,/*228*/0x80,/*229*/0x80,/*230*/0x80,/*231*/0x80,/*232*/0x80,/*233*/0x80,/*234*/0x80,/*235*/0x80,/*236*/0x80,/*237*/0x80,/*238*/0x80,/*239*/0x80,
  /*240*/0x80,/*241*/0x80,/*242*/0x80,/*243*/0x80,/*244*/0x80,/*245*/0x80,/*246*/0x80,/*247*/0x80,/*248*/0x80,/*249*/0x80,/*250*/0x80,/*251*/0x80,/*252*/0x80,/*253*/0x80,/*254*/0x80,/*255*/0x00,
};

size_t base64c_encoding_length(size_t len) {
	size_t olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
	olen++; /* nul termination */
	if (olen < len)
		return 0; /* integer overflow */
	return olen;
}

size_t base64c_encode(const unsigned char *src, size_t len,
			      unsigned char* out, const size_t out_len)
{
	unsigned char *pos;
	const unsigned char *end, *in;
	const unsigned char *out_end = out + out_len;

	end = src + len;
	in = src;
	pos = out;

  if (out_len < base64c_encoding_length(len)) { return -1; } 

	while (end - in >= 3 ) {
		*pos++ = base64c_table[in[0] >> 2];
		*pos++ = base64c_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
		*pos++ = base64c_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
		*pos++ = base64c_table[in[2] & 0x3f];
		in += 3;
	}

	if (end - in) {
		*pos++ = base64c_table[in[0] >> 2];

		if (end - in == 1) {
			*pos++ = base64c_table[(in[0] & 0x03) << 4];
			*pos++ = '=';
		} else {
			*pos++ = base64c_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
			*pos++ = base64c_table[(in[1] & 0x0f) << 2];
		}
		*pos++ = '=';
	}

	*pos = '\0';

	return out_len - (out_end-pos);
}

size_t base64c_decoding_length(size_t inlen) {
  return inlen / 4 * 3;
}

size_t base64c_decode(const unsigned char *src, size_t len, unsigned char *out, const size_t out_len)
{
	if (out == NULL) { return 0; }
	if (out_len <= 0) { return 0; }

	unsigned char *pos, block[4], tmp;
	size_t i, count;
	int pad = 0;


	if (len == 0 ){
		*out = '\0';
		return 1;
	}
	if (len % 4) {
		return -1;
	}

	size_t olen = base64c_decoding_length(len);
	if (olen > out_len) { return -1; }

	pos = out;
	count = 0;
	for (i = 0; i < len; i++) {
    if (src[i] == '=') { pad++; }
		tmp = base64c_dtable[src[i]];

    if (tmp == 0x80) { return -1; }

		block[count] = tmp;
		count++;
		if (count == 4) {
			*pos++ = (block[0] << 2) | (block[1] >> 4);
			*pos++ = (block[1] << 4) | (block[2] >> 2);
			*pos++ = (block[2] << 6) | block[3];
			count = 0;
			if (pad) {
				if (pad == 1)
					pos--;
				else if (pad == 2)
					pos -= 2;
				else {
					return -1;
				}
				break;
			}
		}
	}
  *pos = '\0';

	return pos - out;
}
