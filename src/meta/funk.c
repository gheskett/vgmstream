#include "meta.h"
#include "../layout/layout.h"
#include "../util.h"

/* .FUNK - funkify your audio! */
VGMSTREAM * init_vgmstream_funk(STREAMFILE *streamFile) {
	VGMSTREAM * vgmstream = NULL;
	off_t start_offset;
	int loop_flag, channel_count, codec;


	/* checks */
	if (!check_extensions(streamFile, "funk"))
		goto fail;
	if (read_32bitBE(0x00, streamFile) != 0x46554E4B) /* "FUNK" */
		goto fail;
	if (read_16bitBE(0x0e, streamFile) != 0x00) /* ? */
		goto fail;

	if (read_32bitBE(0x04, streamFile) + 0x40 != get_streamfile_size(streamFile))
		goto fail;
	loop_flag = read_16bitBE(0x08, streamFile);
	channel_count = read_16bitBE(0x0a, streamFile);
	codec = read_16bitBE(0x0c, streamFile);
	//max_block   = read_32bitBE(0x20,streamFile);

	start_offset = 0x40;
	if (read_32bitBE(start_offset, streamFile) != 0x424C434B)  /* "BLCK" */
		goto fail;


	/* build the VGMSTREAM */
	vgmstream = allocate_vgmstream(channel_count, loop_flag);
	if (!vgmstream) goto fail;

	vgmstream->meta_type = meta_FUNK;
	vgmstream->loop_start_sample = read_32bitBE(0x10, streamFile);
	vgmstream->loop_end_sample = read_32bitBE(0x14, streamFile);
	vgmstream->sample_rate = read_32bitBE(0x18, streamFile);
	vgmstream->num_samples = read_32bitBE(0x1c, streamFile);

	vgmstream->layout_type = layout_blocked_funk;
	switch (codec) {
	case 0x00: /* Unchanged */
		vgmstream->coding_type = coding_PCM16BE;
		break;
	case 0x01: /* FFT */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x02: /* Derivative */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x03: /* FFT, Dereivative */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x04: /* Integral */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x05: /* FFT, Integral */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x06: /* Derivative + Integral */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x07: /* FFT, Derivative + Integral */
		vgmstream->coding_type = coding_PCM16BE_FUNK;
		break;
	case 0x08: /* Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x09: /* FFT, Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x0a: /* Derivative, Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x0b: /* FFT, Dereivative, Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x0c: /* Integral, Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x0d: /* FFT, Integral, Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x0e: /* Derivative + Integral, Triangle Linear */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	case 0x0f: /* F U N K Y   F R E S H */
		vgmstream->coding_type = coding_TRI_LIN_BE;
		break;
	default:
		goto fail;
	}

	if (!vgmstream_open_stream(vgmstream, streamFile, start_offset))
		goto fail;
	return vgmstream;

fail:
	close_vgmstream(vgmstream);
	return NULL;
}
