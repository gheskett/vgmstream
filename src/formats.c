#include "formats.h"

//#define VGM_REGISTER_TYPE(extension) ...
//#define VGM_REGISTER_TYPE_COMMON(extension) ... /* for common extensions like aiff */


/* some extensions could be #ifdef but no really needed */
/* some formats marked as "not parsed" mean they'll go through FFmpeg, the header/extension is not parsed */

static const char* extension_list[] = {
    "2dx9",
    "2pfs",

    "aa3", //FFmpeg, not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "aaap",
    "aax",
    //"ac3", //FFmpeg, not parsed //common?
    "acm",
    "adm",
    "adp",
    "adpcm",
    "ads",
    "adx",
    "afc",
    "agsc",
    "ahx",
    "aifc",
    "aifcl",
    //"aiff", //common
    "aix",
    "akb", //AAC
    "amts",
    "as4",
    "asd",
    "asf",
    "asr",
    "ass",
    "ast",
    "at3",
    "aud",
    "aus",

    "b1s",
    "baf",
    "baka",
    "bar",
    "bcstm",
    "bcwav",
    "bdsp",
    "bfstm",
    "bfwav",
    "bfwavnsmbu",
    "bg00",
    "bgw",
    "bh2pcm",
    "bmdx",
    "bms",
    "bnk",
    "bns",
    "bnsf",
    "bo2",
    "brstm",
    "brstmspm",
    "btsnd",
    "bvg",

    "caf",
    "capdsp",
    "cbd2",
    "ccc",
    "cfn",
    "ckd",
    "cnk",
    "cps",

    "dcs",
    "ddsp",
    "de2",
    "dmsg",
    "dsp",
    "dspw",
    "dtk",
    "dvi",
    "dxh",

    "eam",
    "emff",
    "enth",

    "fag",
    "ffw",
    "filp",
    "fsb",
    "fwav",

    "g1l",
    "gbts",
    "gca",
    "gcm",
    "gcub",
    "gcw",
    "genh",
    "gms",
    "gsb",

    "hca",
    "hgc1",
    "his",
    "hlwav",
    "hps",
    "hsf",
    "hwas",

    "iab",
    "iadp",
    "idsp",
    "idvi",
    "ikm",
    "ild",
    "int",
    "isd",
    "isws",
    "ivaud",
    "ivag",
    "ivb",

    "joe",
    "jstm",

    "kces",
    "kcey",
    "khv",
    "kovs",
    "kraw",

    "leg",
    "logg",
    "lpcm",
    "lps",
    "lsf",
    "lwav",

    "matx",
    "mca",
    "mcg",
    "mi4",
    "mib",
    "mic",
    "mihb",
    "mnstr",
    "mpdsp",
    "mpds",
    "msa",
    "msf",
    "mss",
    "msvp",
    "mtaf",
    "mus",
    "musc",
    "musx",
    "mwv",
    "mxst",
    "myspd",

    "ndp",
    "ngca",
    "nps",
    "npsf",
    "nus3bank", //todo not existing?
    "nwa",

    "oma", //FFmpeg, not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "omu",
    "otm",

    "p2bt",
    "p3d",
    "past",
    "pcm",
    "pdt",
    "pnb",
    "pona",
    "pos",
    "ps2stm",
    "psh",
    "psnd",
    "psw",

    "ras",
    "raw",
    "rkv",
    "rnd",
    "rrds",
    "rsd",
    "rsf",
    "rstm",
    "rvws",
    "rwar",
    "rwav",
    "rws",
    "rwsd",
    "rwx",
    "rxw",

    "s14",
    "sab",
    "sad",
    "sap",
    "sc",
    "scd",
    "sck",
    "sd9",
    "sdt",
    "seg",
    "sf0",
    "sfl",
    "sfs",
    "sfx",
    "sgb",
    "sgd",
    "sgx",
    "sl3",
    "sli",
    "smp",
    "smpl",
    "snd",
    "snds",
    "sng",
    "sns",
    "spd",
    "spm",
    "sps",
    "spsd",
    "spw",
    "ss2",
    "ss3",
    "ss7",
    "ssm",
    "sss",
    "ster",
    "sth",
    //"stm", //common
    "stma",
    "str",
    "strm",
    "sts",
    "stx",
    "svag",
    "svs",
    "swav",
    "swd",

    "tec",
    "thp",
    "tk1",
    "tk5",
    "tra",
    "tun",
    "tydsp",

    "um3",

    "vag",
    "vas",
    "vawx",
    "vb",
    "vbk",
    "vgs",
    "vgv",
    "vig",

    "vms",
    "voi",
    "vpk",
    "vs",
    "vsf",

    "waa",
    "wac",
    "wad",
    "wam",
    "was",
    "wavm",
    "wb",
    "wii",
    "wmus",
    "wp2",
    "wpd",
    "wsd",
    "wsi",
    "wvs",

    "xa",
    "xa2",
    "xa30",
    "xag",
    "xau",
    "xma",
    "xma2",
    "xmu",
    "xnb",
    "xsf",
    "xss",
    "xvag",
    "xvas",
    "xwav",
    "xwb",
    "xwm", //FFmpeg, not parsed (XWMA)
    "xwma", //FFmpeg, not parsed (XWMA)
    "xwv",

    "ydsp",
    "ymf",

    "zsd",
    "zwdsp",

    "vgmstream"

    //, NULL //end mark
};

/**
 * List of supported formats.
 *
 * For plugins that need to know (test.exe doesn't use it)
 */
const char ** vgmstream_get_formats() {
    return extension_list;
}

/**
 * Number of elements in the list.
 */
int vgmstream_get_formats_length() {
    return sizeof(extension_list) / sizeof(char*);
}
