#include "vgmstream.h"
#include "coding/coding.h"


/* Defines the list of accepted extensions. vgmstream doesn't use it internally so it's here
 * to inform plugins that need it. Common extensions are commented out to avoid stealing them
 * and possibly adding an unwanted association to the player. */

/* Common extensions (like .wav or .ogg) should go in the common_extension_list. It should only
 * contain common formats that vgmstream can also parse, to avoid hijacking them (since their
 * plugins typically are faster and have desirable features vgmstream won't handle). Extensions of
 * formats not parsed don't need to go there (for example .stm is a Scream Tracker Module elsewhere,
 * but our .stm is very different so there is no conflict). */

/* Some extensions require external libraries and could be #ifdef, not worth. */

/* Formats marked as "not parsed" mean they'll go through FFmpeg, the header/extension isn't
 * parsed by vgmstream and typically won't not be fully accurate. */


static const char* extension_list[] = {
    //"", /* vgmstream can play extensionless files too, but plugins must accept them manually */

    "04sw",
    "208",
    "2dx9",
    "2pfs",
    "4", // for Game.com audio
    "8", //txth/reserved [Gungage (PS1)]
    "800",
    "9tav",

    //"aac", //common
    "aa3", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "aax",
    "abc", //txth/reserved [Find My Own Way (PS2) tech demo]
    "abk",
    //"ac3", //common, FFmpeg/not parsed (AC3)
    "acb",
    "ace", //fake extension for tri-Ace's .aac (renamed, to be removed)
    "acm",
    "ad", //txth/reserved [Xenosaga Freaks (PS2)]
    "adc", //txth/reserved [Tomb Raider The Last Revelation (DC), Tomb Raider Chronicles (DC)]
    "adm",
    "adp",
    "adpcm",
    "adpcmx",
    "ads",
    "adw",
    "adx",
    "afc",
    "afs2",
    "agsc",
    "ahx",
    "ahv",
    "ai",
    //"aif", //common
    "aif-Loop",
    "aifc", //common?
    "aifcl", //fake extension for .aif???
    //"aiff", //common
    "aiffl", //fake extension for .aif???
    "aix",
    "akb",
    "al2",
    "amts", //fake extension/header id for .stm (renamed? to be removed?)
    "ao",
    "apc",
    "as4",
    "asd",
    "asf",
    "asr",
    "ass",
    "ast",
    "at3",
    "at9",
    "atsl",
    "atsl3",
    "atsl4",
    "atx",
    "aud",
    "aus",
    "awb",
    "awc",

    "b1s",
    "baf",
    "baka",
    "bank",
    "bar",
    "bcstm",
    "bcwav",
    "bd3",
    "bdsp",
    "bfstm",
    "bfwav",
    "bfwavnsmbu", //fake extension for New Super Smash Bros U (renamed to fix bug)
    "bg00",
    "bgm",
    "bgw",
    "bh2pcm",
    "bik",
    "bika",
    "bik2",
    //"bin", //common
    "bk2",
    "bmdx",
    "bms",
    "bnk",
    "bnm",
    "bns",
    "bnsf",
    "bo2",
    "brstm",
    "brstmspm",
    "btsnd",
    "bvg",
    "bwav",

    "caf",
    "capdsp",
    "cbd2",
    "ccc",
    "cd",
    "cfn", //fake extension for CAF (renamed, to be removed?)
    "ckb",
    "ckd",
    "cks",
    "cnk",
    "cpk",
    "cps",
    "csa", //txth/reserved [LEGO Racers 2 (PS2)]
    "csmp",
    "cvs",
    "cxs",

    "da",
    "data",
    "dax",
    "dbm",
    "dct",
    "dcs",
    "ddsp",
    "de2",
    "dec",
    "dmsg",
    "ds2", //txth/reserved [Star Wars Bounty Hunter (GC)]
    "dsf",
    "dsp",
    "dspw",
    "dtk",
    "dvi",
    "dxh",

    "e4x",
    "eam",
    "eas",
    "eda", //txth/reserved [Project Eden (PS2)]
    "emff", //fake extension for .mul (to be removed)
    "enm",
    "eno",
    "ens",
    "enth",
    "exa",
    "ezw",

    "fag",
    "fda",
    "ffw",
    "filp",
    //"flac", //common
    "flx",
    "fsb",
    "fsv",
    "funk",
    "fwav",
    "fwse",

    "g1l",
    "gbts",
    "gca",
    "gcm",
    "gcub",
    "gcw",
    "genh",
    "gin",
    "gms",
    "gsb",
    "gsf",
    "gtd",
    "gwm",

    "h4m",
    "hab",
    "hca",
    "hdr",
    "hgc1",
    "his",
    "hps",
    "hsf",
    "hx2",
    "hx3",
    "hxc",
    "hxd",
    "hxg",
    "hxx",
    "hwas",

    "iab",
    "iadp",
    "idmsf",
    "idsp",
    "idvi", //fake extension/header id for .pcm (renamed, to be removed)
    "idwav",
    "idx",
    "idxma",
    "ikm",
    "ild",
    "ilv", //txth/reserved [Star Wars Episode III (PS2)]
    "ima",
    "imc",
    "int",
    "is14",
    "isb",
    "isd",
    "isws",
    "itl",
    "ivaud",
    "ivag",
    "ivb",
    "ivs", //txth/reserved [Burnout 2 (PS2)]

    "joe",
    "jstm",

    "kces",
    "kcey", //fake extension/header id for .pcm (renamed, to be removed)
    "khv", //fake extension/header id for .vas (renamed, to be removed)
    "km9",
    "kovs", //fake extension/header id for .kvs
    "kns",
    "kraw",
    "ktss", //fake extension/header id for .kns
    "kvs",

    "l",
    "l00", //txth/reserved [Disney's Dinosaur (PS2)]
    "laac", //fake extension for .aac (tri-Ace)
    "laif", //fake extension for .aif (various)
    "laiff", //fake extension for .aiff
    "laifc", //fake extension for .aifc
    "lac3", //fake extension for .ac3, FFmpeg/not parsed
    "lasf", //fake extension for .asf (various)
    "lbin", //fake extension for .bin (various)
    "leg",
    "lflac", //fake extension for .flac, FFmpeg/not parsed
    "lin",
    "lm0",
    "lm1",
    "lm2",
    "lm3",
    "lm4",
    "lm5",
    "lm6",
    "lm7",
    "lmp2", //fake extension for .mp2, FFmpeg/not parsed
    "lmp3", //fake extension for .mp3, FFmpeg/not parsed
    "lmp4", //fake extension for .mp4
    "lmpc", //fake extension for .mpc, FFmpeg/not parsed
    "logg", //fake extension for .ogg
    "lopus", //fake extension for .opus
    "lpcm",
    "lpk",
    "lps",
    "lrmb",
    "lse",
    "lsf",
    "lstm", //fake extension for .stm
    "lwav", //fake extension for .wav
    "lwma", //fake extension for .wma, FFmpeg/not parsed

    "mab",
    "mad",
    "map",
    "matx",
    "mc3",
    "mca",
    "mcadpcm",
    "mcg",
    "mds",
    "mdsp",
    "med",
    "mi4",
    "mib",
    "mic",
    "mihb",
    "mnstr",
    "mogg",
    //"mp+", //common [Moonshine Runners (PC)]
    //"mp2", //common
    //"mp3", //common
    //"mp4", //common
    //"mpc", //common
    "mpdsp",
    "mpds",
    "mpf",
    "mps", //txth/reserved [Scandal (PS2)]
    "ms",
    "msa",
    "msb",
    "msd",
    "msf",
    "mss",
    "msv",
    "msvp",
    "mta2",
    "mtaf",
    "mul",
    "mus",
    "musc",
    "musx",
    "mvb", //txth/reserved [Porsche Challenge (PS1)]
    "mwv",
    "mxst",
    "myspd",

    "naac",
    "ndp",
    "ngca",
    "nlsd",
    "nop",
    "nps",
    "npsf", //fake extension/header id for .nps (in bigfiles)
    "nub",
    "nub2",
    "nus3audio",
    "nus3bank",
    "nwa",
    "nwav",
    "nxa",

    //"ogg", //common
    "ogl",
    "oma", //FFmpeg/not parsed (ATRAC3/ATRAC3PLUS/MP3/LPCM/WMA)
    "omu",
    //"opus", //common
    "opusx",
    "otm",
    "oto", //txth/reserved [Vampire Savior (SAT)]
    "ovb",

    "p04", //txth/reserved [Psychic Force 2012 (DC), Skies of Arcadia (DC)]
    "p16", //txth/reserved [Astal (SAT)]
    "p1d", //txth/reserved [Farming Simulator 18 (3DS)]
    "p2a", //txth/reserved [Thunderhawk Operation Phoenix (PS2)]
    "p2bt",
    "p3d",
    "past",
    "pcm",
    "pdt",
    "pk",
    "pnb",
    "pona",
    "pos",
    "ps2stm", //fake extension for .stm (renamed? to be removed?)
    "psf",
    "psh", //fake extension for .vsv (to be removed)
    "psnd",
    "psw", //fake extension for .wam (renamed, to be removed)

    "r",
    "rac", //txth/reserved [Manhunt (Xbox)]
    "rad",
    "rak",
    "ras",
    "raw",
    "rda", //FFmpeg/reserved [Rhythm Destruction (PC)]
    "rkv",
    "rnd",
    "rof",
    "rpgmvo",
    "rrds",
    "rsd",
    "rsf",
    "rsm",
    "rstm", //fake extension/header id for .rstm (in bigfiles)
    "rvws",
    "rwar",
    "rwav",
    "rws",
    "rwsd",
    "rwx",
    "rxw",
    "rxx", //txth/reserved [Full Auto (X360)]

    "s14",
    "s3v", //txth/reserved [Sound Voltex 5 (AC)]
    "sab",
    "sad",
    "sap",
    "sb0",
    "sb1",
    "sb2",
    "sb3",
    "sb4",
    "sb5",
    "sb6",
    "sb7",
    "sbr",
    "sbv",
    "sm0",
    "sm1",
    "sm2",
    "sm3",
    "sm4",
    "sm5",
    "sm6",
    "sm7",
    "sbin",
    "sc",
    "scd",
    "sch",
    "sd9",
    "sdf",
    "sdt",
    "seb",
    "sed",
    "seg",
    "sf0",
    "sfl",
    "sfs",
    "sfx",
    "sgb",
    "sgd",
    "sgx",
    "sl3",
    "slb", //txth/reserved [THE Nekomura no Hitobito (PS2)]
    "sli",
    "smc",
    "smk",
    "smp",
    "smpl", //fake extension/header id for .v0/v1 (renamed, to be removed)
    "smv",
    "snd",
    "snds",
    "sng",
    "sngw",
    "snr",
    "sns",
    "snu",
    "sod",
    "son",
    "spd",
    "spm",
    "sps",
    "spsd",
    "spw",
    "ss2",
    "ssm",
    "sss",
    "ster",
    "sth",
    "stm",
    "stma", //fake extension/header id for .stm
    "str",
    "stream",
    "strm",
    "sts",
    "stx",
    "svag",
    "svs",
    "svg",
    "swag",
    "swav",
    "swd",
    "switch_audio",
    "sx",
    "sxd",
    "sxd2",
    "sxd3",

    "tec",
    "tgq",
    "thp",
    "tk5",
    "tmx",
    "tra",
    "tun",
    "txth",
    "txtp",
    "tydsp",

    "ue4opus",
    "ulw",
    "um3",
    "utk",
    "uv",

    "v0",
    //"v1", //dual channel with v0
    "va3",
    "vag",
    "vai",
    "vam", //txth/reserved [Rocket Power: Beach Bandits (PS2)]
    "vas",
    "vawx",
    "vb",
    "vbk",
    "vbx", //txth/reserved [THE Taxi 2 (PS2)]
    "vds",
    "vdm",
    "vgm", //txth/reserved [Maximo (PS2)]
    "vgs",
    "vgv",
    "vig",
    "vis",
    "vms",
    "vmu", //txth/reserved [Red Faction (PS2)]
    "voi",
    "vp6",
    "vpk",
    "vs",
    "vsf",
    "vsv",
    "vxn",

    "waa",
    "wac",
    "wad",
    "waf",
    "wam",
    "was",
    //"wav", //common
    "wavc",
    "wave",
    "wavebatch",
    "wavm",
    "wavx", //txth/reserved [LEGO Star Wars (Xbox)]
    "wb",
    "wb2",
    "wbd",
    "wd",
    "wem",
    "wii",
    "wip", //txth/reserved [Colin McRae DiRT (PC)]
    "wma", //common
    "wmus",
    "wp2",
    "wpd",
    "wsd",
    "wsi",
    "wua",
    "wv2",
    "wv6",
    "wve",
    "wvs",
    "wvx",

    "x",
    "xa",
    "xa2",
    "xa30",
    "xag",
    "xau",
    "xav",
    "xen",
    "xma",
    "xma2",
    "xmu",
    "xnb",
    "xsf",
    "xsew",
    "xss",
    "xvag",
    "xvas",
    "xwav", //fake extension for .wav (renamed, to be removed)
    "xwb",
    "xmd",
    "xopus",
    "xps",
    "xwc",
    "xwm",
    "xwma",
    "xws",
    "xwv",

    "ydsp",
    "ymf",

    "zic",
    "zsd",
    "zsm",
    "zss",
    "zwdsp",

    "vgmstream" /* fake extension, catch-all for FFmpeg/txth/etc */

    //, NULL //end mark
};

static const char* common_extension_list[] = {
    "aac", //common
    "ac3", //common, FFmpeg/not parsed (AC3)
    "aif", //common
    "aiff", //common
    "bin", //common
    "flac", //common
    "mp+", //common [Moonshine Runners (PC)]
    "mp2", //common
    "mp3", //common
    "mp4", //common
    "mpc", //common
    "ogg", //common
    "opus", //common
    "wav", //common
};


/* List supported formats and return elements in the list, for plugins that need to know. */
const char ** vgmstream_get_formats(size_t * size) {
    *size = sizeof(extension_list) / sizeof(char*);
    return extension_list;
}

const char ** vgmstream_get_common_formats(size_t * size) {
    *size = sizeof(common_extension_list) / sizeof(char*);
    return common_extension_list;
}


/* internal description info */

typedef struct {
    coding_t type;
    const char *description;
} coding_info;

typedef struct {
    layout_t type;
    const char *description;
} layout_info;

typedef struct {
    meta_t type;
    const char *description;
} meta_info;


static const coding_info coding_info_list[] = {
        {coding_PCM16LE,            "Little Endian 16-bit PCM"},
        {coding_PCM16BE,            "Big Endian 16-bit PCM"},
        {coding_PCM16BE_FUNK,       "FUNKY Big Endian 16-bit PCM"},
        {coding_TRI_LIN_BE,         "FUNKY Custom 1-bit Triangle Wave"},
        {coding_PCM16_int,          "16-bit PCM with 2 byte interleave (block)"},
        {coding_PCM8,               "8-bit signed PCM"},
        {coding_PCM8_int,           "8-bit signed PCM with 1 byte interleave (block)"},
        {coding_PCM8_U,             "8-bit unsigned PCM"},
        {coding_PCM8_U_int,         "8-bit unsigned PCM with 1 byte interleave (block)"},
        {coding_PCM8_SB,            "8-bit PCM with sign bit"},
        {coding_PCM4,               "4-bit signed PCM"},
        {coding_PCM4_U,             "4-bit unsigned PCM"},
        {coding_ULAW,               "8-bit u-Law"},
        {coding_ULAW_int,           "8-bit u-Law with 1 byte interleave (block)"},
        {coding_ALAW,               "8-bit a-Law"},
        {coding_PCMFLOAT,           "32-bit float PCM"},

        {coding_CRI_ADX,            "CRI ADX 4-bit ADPCM"},
        {coding_CRI_ADX_fixed,      "CRI ADX 4-bit ADPCM (fixed coefficients)"},
        {coding_CRI_ADX_exp,        "CRI ADX 4-bit ADPCM (exponential scale)"},
        {coding_CRI_ADX_enc_8,      "CRI ADX 4-bit ADPCM (type 8 encryption)"},
        {coding_CRI_ADX_enc_9,      "CRI ADX 4-bit ADPCM (type 9 encryption)"},

        {coding_NGC_DSP,            "Nintendo DSP 4-bit ADPCM"},
        {coding_NGC_DSP_subint,     "Nintendo DSP 4-bit ADPCM (subinterleave)"},
        {coding_NGC_DTK,            "Nintendo DTK 4-bit ADPCM"},
        {coding_NGC_AFC,            "Nintendo AFC 4-bit ADPCM"},

        {coding_G721,               "CCITT G.721 4-bit ADPCM"},

        {coding_XA,                 "CD-ROM XA 4-bit ADPCM"},
        {coding_PSX,                "Playstation 4-bit ADPCM"},
        {coding_PSX_badflags,       "Playstation 4-bit ADPCM (bad flags)"},
        {coding_PSX_cfg,            "Playstation 4-bit ADPCM (configurable)"},
        {coding_PSX_pivotal,        "Playstation 4-bit ADPCM (Pivotal)"},
        {coding_HEVAG,              "Sony HEVAG 4-bit ADPCM"},

        {coding_EA_XA,              "Electronic Arts EA-XA 4-bit ADPCM v1"},
        {coding_EA_XA_int,          "Electronic Arts EA-XA 4-bit ADPCM v1 (mono/interleave)"},
        {coding_EA_XA_V2,           "Electronic Arts EA-XA 4-bit ADPCM v2"},
        {coding_MAXIS_XA,           "Maxis EA-XA 4-bit ADPCM"},
        {coding_EA_XAS_V0,          "Electronic Arts EA-XAS 4-bit ADPCM v0"},
        {coding_EA_XAS_V1,          "Electronic Arts EA-XAS 4-bit ADPCM v1"},

        {coding_IMA,                "IMA 4-bit ADPCM"},
        {coding_IMA_int,            "IMA 4-bit ADPCM (mono/interleave)"},
        {coding_DVI_IMA,            "Intel DVI 4-bit IMA ADPCM"},
        {coding_DVI_IMA_int,        "Intel DVI 4-bit IMA ADPCM (mono/interleave)"},
        {coding_3DS_IMA,            "3DS IMA 4-bit ADPCM"},
        {coding_SNDS_IMA,           "Heavy Iron .snds 4-bit IMA ADPCM"},
        {coding_OTNS_IMA,           "Omikron: The Nomad Soul 4-bit IMA ADPCM"},
        {coding_WV6_IMA,            "Gorilla Systems WV6 4-bit IMA ADPCM"},
        {coding_ALP_IMA,            "High Voltage ALP 4-bit IMA ADPCM"},
        {coding_FFTA2_IMA,          "Final Fantasy Tactics A2 4-bit IMA ADPCM"},
        {coding_BLITZ_IMA,          "Blitz Games 4-bit IMA ADPCM"},
        {coding_MTF_IMA,            "MT Framework 4-bit IMA ADPCM"},

        {coding_MS_IMA,             "Microsoft 4-bit IMA ADPCM"},
        {coding_XBOX_IMA,           "XBOX 4-bit IMA ADPCM"},
        {coding_XBOX_IMA_mch,       "XBOX 4-bit IMA ADPCM (multichannel)"},
        {coding_XBOX_IMA_int,       "XBOX 4-bit IMA ADPCM (mono/interleave)"},
        {coding_NDS_IMA,            "NDS-style 4-bit IMA ADPCM"},
        {coding_DAT4_IMA,           "Eurocom DAT4 4-bit IMA ADPCM"},
        {coding_RAD_IMA,            "Radical 4-bit IMA ADPCM"},
        {coding_RAD_IMA_mono,       "Radical 4-bit IMA ADPCM (mono/interleave)"},
        {coding_APPLE_IMA4,         "Apple Quicktime 4-bit IMA ADPCM"},
        {coding_FSB_IMA,            "FSB 4-bit IMA ADPCM"},
        {coding_WWISE_IMA,          "Audiokinetic Wwise 4-bit IMA ADPCM"},
        {coding_REF_IMA,            "Reflections 4-bit IMA ADPCM"},
        {coding_AWC_IMA,            "Rockstar AWC 4-bit IMA ADPCM"},
        {coding_UBI_IMA,            "Ubisoft 4-bit IMA ADPCM"},

        {coding_H4M_IMA,            "Hudson HVQM4 4-bit IMA ADPCM"},

        {coding_MSADPCM,            "Microsoft 4-bit ADPCM"},
        {coding_MSADPCM_int,        "Microsoft 4-bit ADPCM (mono/interleave)"},
        {coding_MSADPCM_ck,         "Microsoft 4-bit ADPCM (Cricket Audio)"},
        {coding_WS,                 "Westwood Studios VBR ADPCM"},
        {coding_AICA,               "Yamaha AICA 4-bit ADPCM"},
        {coding_AICA_int,           "Yamaha AICA 4-bit ADPCM (mono/interleave)"},
        {coding_ASKA,               "tri-Ace Aska 4-bit ADPCM"},
        {coding_NXAP,               "Nex NXAP 4-bit ADPCM"},
        {coding_TGC,                "Tiger Game.com 4-bit ADPCM"},
        {coding_NDS_PROCYON,        "Procyon Studio Digital Sound Elements NDS 4-bit APDCM"},
        {coding_L5_555,             "Level-5 0x555 4-bit ADPCM"},
        {coding_LSF,                "lsf 4-bit ADPCM"},
        {coding_MTAF,               "Konami MTAF 4-bit ADPCM"},
        {coding_MTA2,               "Konami MTA2 4-bit ADPCM"},
        {coding_MC3,                "Paradigm MC3 3-bit ADPCM"},
        {coding_FADPCM,             "FMOD FADPCM 4-bit ADPCM"},
        {coding_ASF,                "Argonaut ASF 4-bit ADPCM"},
        {coding_DSA,                "Ocean DSA 4-bit ADPCM"},
        {coding_XMD,                "Konami XMD 4-bit ADPCM"},
        {coding_PCFX,               "PC-FX 4-bit ADPCM"},
        {coding_OKI16,              "OKI 4-bit ADPCM (16-bit output)"},
        {coding_OKI4S,              "OKI 4-bit ADPCM (4-shift)"},
        {coding_PTADPCM,            "Platinum 4-bit ADPCM"},

        {coding_SDX2,               "Squareroot-delta-exact (SDX2) 8-bit DPCM"},
        {coding_SDX2_int,           "Squareroot-delta-exact (SDX2) 8-bit DPCM with 1 byte interleave"},
        {coding_CBD2,               "Cuberoot-delta-exact (CBD2) 8-bit DPCM"},
        {coding_CBD2_int,           "Cuberoot-delta-exact (CBD2) 8-bit DPCM with 1 byte interleave"},
        {coding_SASSC,              "Activision / EXAKT SASSC 8-bit DPCM"},
        {coding_DERF,               "Xilam DERF 8-bit DPCM"},
        {coding_ACM,                "InterPlay ACM"},
        {coding_NWA,                "VisualArt's NWA DPCM"},
        {coding_CIRCUS_ADPCM,       "Circus 8-bit ADPCM"},
        {coding_UBI_ADPCM,          "Ubisoft 4/6-bit ADPCM"},

        {coding_EA_MT,              "Electronic Arts MicroTalk"},

        {coding_RELIC,              "Relic Codec"},
        {coding_CRI_HCA,            "CRI HCA"},
        
#ifdef VGM_USE_VORBIS
        {coding_OGG_VORBIS,         "Ogg Vorbis"},
        {coding_VORBIS_custom,      "Custom Vorbis"},
#endif
#ifdef VGM_USE_MPEG
        {coding_MPEG_custom,        "Custom MPEG Audio"},
        {coding_MPEG_ealayer3,      "EALayer3"},
        {coding_MPEG_layer1,        "MPEG Layer I Audio (MP1)"},
        {coding_MPEG_layer2,        "MPEG Layer II Audio (MP2)"},
        {coding_MPEG_layer3,        "MPEG Layer III Audio (MP3)"},
#endif
#ifdef VGM_USE_G7221
        {coding_G7221C,             "ITU G.722.1 annex C (Polycom Siren 14)"},
#endif
#ifdef VGM_USE_G719
        {coding_G719,               "ITU G.719 annex B (Polycom Siren 22)"},
#endif
#ifdef VGM_USE_MAIATRAC3PLUS
        {coding_AT3plus,            "ATRAC3plus"},
#endif
#ifdef VGM_USE_ATRAC9
        {coding_ATRAC9,             "ATRAC9"},
#endif
#ifdef VGM_USE_CELT
        {coding_CELT_FSB,           "Custom CELT"},
#endif
#ifdef VGM_USE_FFMPEG
        {coding_FFmpeg,             "FFmpeg"},
#endif
};

static const layout_info layout_info_list[] = {
        {layout_none,                   "flat"},
        {layout_interleave,             "interleave"},

        {layout_segmented,              "segmented"},
        {layout_layered,                "layered"},

        {layout_blocked_mxch,           "blocked (MxCh)"},
        {layout_blocked_ast,            "blocked (AST)"},
        {layout_blocked_funk,           "blocked (FUNK)"},
        {layout_blocked_halpst,         "blocked (HALPST)"},
        {layout_blocked_xa,             "blocked (XA)"},
        {layout_blocked_ea_schl,        "blocked (EA SCHl)"},
        {layout_blocked_ea_1snh,        "blocked (EA 1SNh)"},
        {layout_blocked_caf,            "blocked (CAF)"},
        {layout_blocked_wsi,            "blocked (WSI)"},
        {layout_blocked_xvas,           "blocked (.xvas)"},
        {layout_blocked_str_snds,       "blocked (.str SNDS)"},
        {layout_blocked_ws_aud,         "blocked (Westwood Studios .aud)"},
        {layout_blocked_matx,           "blocked (Matrix .matx)"},
        {layout_blocked_dec,            "blocked (DEC)"},
        {layout_blocked_vs,             "blocked (Melbourne House VS)"},
        {layout_blocked_mul,            "blocked (MUL)"},
        {layout_blocked_gsb,            "blocked (GSB)"},
        {layout_blocked_thp,            "blocked (THP)"},
        {layout_blocked_filp,           "blocked (FILP)"},
        {layout_blocked_ea_swvr,        "blocked (EA SWVR)"},
        {layout_blocked_adm,            "blocked (ADM)"},
        {layout_blocked_bdsp,           "blocked (BDSP)"},
        {layout_blocked_ivaud,          "blocked (IVAUD)"},
        {layout_blocked_ps2_iab,        "blocked (IAB)"},
        {layout_blocked_vs_str,         "blocked (STR VS)"},
        {layout_blocked_rws,            "blocked (RWS)"},
        {layout_blocked_hwas,           "blocked (HWAS)"},
        {layout_blocked_tra,            "blocked (TRA)"},
        {layout_blocked_ea_sns,         "blocked (EA SNS)"},
        {layout_blocked_awc,            "blocked (AWC)"},
        {layout_blocked_vgs,            "blocked (VGS)"},
        {layout_blocked_vawx,           "blocked (VAWX)"},
        {layout_blocked_xvag_subsong,   "blocked (XVAG subsong)"},
        {layout_blocked_ea_wve_au00,    "blocked (EA WVE au00)"},
        {layout_blocked_ea_wve_ad10,    "blocked (EA WVE Ad10)"},
        {layout_blocked_sthd,           "blocked (STHD)"},
        {layout_blocked_h4m,            "blocked (H4M)"},
        {layout_blocked_xa_aiff,        "blocked (XA AIFF)"},
        {layout_blocked_vs_square,      "blocked (Square VS)"},
};

static const meta_info meta_info_list[] = {
        {meta_RSTM,                 "Nintendo RSTM header"},
        {meta_STRM,                 "Nintendo STRM header"},
        {meta_ADX_03,               "CRI ADX header type 03"},
        {meta_ADX_04,               "CRI ADX header type 04"},
        {meta_ADX_05,               "CRI ADX header type 05"},
        {meta_AIX,                  "CRI AIX header"},
        {meta_AAX,                  "CRI AAX header"},
        {meta_UTF_DSP,              "CRI ADPCM_WII header"},
        {meta_AGSC,                 "Retro Studios AGSC header"},
        {meta_CSMP,                 "Retro Studios CSMP header"},
        {meta_RFRM,                 "Retro Studios RFRM header"},
        {meta_NGC_ADPDTK,           "Nintendo ADP raw header"},
        {meta_RSF,                  "Retro Studios RSF raw header"},
        {meta_AFC,                  "Nintendo .AFC header"},
        {meta_AST,                  "Nintendo AST header"},
        {meta_FUNK,                 "Funky fresh header"},
        {meta_HALPST,               "HAL Laboratory HALPST header"},
        {meta_DSP_RS03,             "Retro Studios RS03 header"},
        {meta_DSP_STD,              "Nintendo DSP header"},
        {meta_DSP_CSTR,             "Namco Cstr header"},
        {meta_GCSW,                 "MileStone GCSW header"},
        {meta_PS2_SShd,             "Sony ADS header"},
        {meta_NPS,                  "Namco NPSF header"},
        {meta_RWSD,                 "Nintendo RWSD header (single stream)"},
        {meta_RWAR,                 "Nintendo RWAR header (single RWAV stream)"},
        {meta_RWAV,                 "Nintendo RWAV header"},
        {meta_CWAV,                 "Nintendo CWAV header"},
        {meta_FWAV,                 "Nintendo FWAV header"},
        {meta_XA,                   "Sony XA header"},
        {meta_PS2_RXWS,             "Sony RXWS header"},
        {meta_RAW_INT,              "PS2 .int raw header"},
        {meta_PS2_OMU,              "Alter Echo OMU Header"},
        {meta_DSP_STM,              "Intelligent Systems STM header"},
        {meta_PS2_EXST,             "Sony EXST header"},
        {meta_PS2_SVAG,             "Konami SVAG header"},
        {meta_PS_HEADERLESS,        "Headerless PS-ADPCM raw header"},
        {meta_PS2_MIB_MIH,          "Sony MultiStream MIH+MIB header"},
        {meta_DSP_MPDSP,            "Single DSP header stereo by .mpdsp extension"},
        {meta_PS2_MIC,              "KOEI .MIC header"},
        {meta_DSP_JETTERS,          "Double DSP header stereo by _lr.dsp extension"},
        {meta_DSP_MSS,              "Double DSP header stereo by .mss extension"},
        {meta_DSP_GCM,              "Double DSP header stereo by .gcm extension"},
        {meta_IDSP_TT,              "Traveller's Tales IDSP header"},
        {meta_RSTM_SPM,             "Nintendo RSTM header (brstmspm)"},
        {meta_RAW_PCM,              "PC .raw raw header"},
        {meta_PS2_VAGi,             "Sony VAGi header"},
        {meta_PS2_VAGp,             "Sony VAGp header"},
        {meta_PS2_pGAV,             "Sony pGAV header"},
        {meta_PS2_VAGp_AAAP,        "Acclaim Austin AAAp VAG header"},
        {meta_SEB,                  "Game Arts .SEB header"},
        {meta_STR_WAV,              "Blitz Games .STR+WAV header"},
        {meta_PS2_ILD,              "ILD header"},
        {meta_PS2_PNB,              "assumed PNB (PsychoNauts Bgm File) by .pnb extension"},
        {meta_RAW_WAVM,             "Xbox .wavm raw header"},
        {meta_DSP_STR,              "assumed Conan Gamecube STR File by .str extension"},
        {meta_EA_SCHL,              "Electronic Arts SCHl header (variable)"},
        {meta_EA_SCHL_fixed,        "Electronic Arts SCHl header (fixed)"},
        {meta_CAF,                  "tri-Crescendo CAF Header"},
        {meta_VPK,                  "SCE America VPK Header"},
        {meta_GENH,                 "GENH generic header"},
        {meta_DSP_SADB,             "Procyon Studio SADB header"},
        {meta_SADL,                 "Procyon Studio SADL header"},
        {meta_PS2_BMDX,             "Beatmania .bmdx header"},
        {meta_DSP_WSI,              "Alone in the Dark .WSI header"},
        {meta_AIFC,                 "Apple AIFF-C (Audio Interchange File Format) header"},
        {meta_AIFF,                 "Apple AIFF (Audio Interchange File Format) header"},
        {meta_STR_SNDS,             "3DO .str header"},
        {meta_WS_AUD,               "Westwood Studios .aud header"},
        {meta_WS_AUD_old,           "Westwood Studios .aud (old) header"},
        {meta_PS2_IVB,              "IVB/BVII header"},
        {meta_SVS,                  "Square SVS header"},
        {meta_RIFF_WAVE,            "RIFF WAVE header"},
        {meta_RIFF_WAVE_POS,        "RIFF WAVE header and .pos for looping"},
        {meta_NWA,                  "VisualArt's NWA header"},
        {meta_NWA_NWAINFOINI,       "VisualArt's NWA header (NWAINFO.INI looping)"},
        {meta_NWA_GAMEEXEINI,       "VisualArt's NWA header (Gameexe.ini looping)"},
        {meta_XSS,                  "Dino Crisis 3 XSS File"},
        {meta_HGC1,                 "Knights of the Temple 2 hgC1 Header"},
        {meta_AUS,                  "Capcom AUS Header"},
        {meta_RWS,                  "RenderWare RWS header"},
        {meta_EA_1SNH,              "Electronic Arts 1SNh header"},
        {meta_EA_EACS,              "Electronic Arts EACS header"},
        {meta_SL3,                  "Atari Melbourne House SL3 header"},
        {meta_FSB1,                 "FMOD Sample Bank (FSB1) Header"},
        {meta_FSB2,                 "FMOD Sample Bank (FSB2) Header"},
        {meta_FSB3,                 "FMOD Sample Bank (FSB3) Header"},
        {meta_FSB4,                 "FMOD Sample Bank (FSB4) Header"},
        {meta_FSB5,                 "FMOD Sample Bank (FSB5) Header"},
        {meta_RWX,                  "RWX Header"},
        {meta_XWB,                  "Microsoft XWB header"},
        {meta_PS2_XA30,             "Reflections XA30 PS2 header"},
        {meta_MUSC,                 "Krome MUSC header"},
        {meta_MUSX,                 "Eurocom MUSX header"},
        {meta_LEG,                  "Legaia 2 - Duel Saga LEG Header"},
        {meta_FILP,                 "Bio Hazard - Gun Survivor FILp Header"},
        {meta_IKM,                  "MiCROViSiON IKM header"},
        {meta_SFS,                  "Baroque SFS Header"},
        {meta_SAT_DVI,              "Konami KCEN DVI. header"},
        {meta_DC_KCEY,              "Konami KCEY KCEYCOMP header"},
        {meta_BG00,                 "Falcom BG00 Header"},
        {meta_PS2_RSTM,             "Rockstar Games RSTM Header"},
        {meta_ACM,                  "InterPlay ACM Header"},
        {meta_MUS_ACM,              "InterPlay MUS ACM header"},
        {meta_PS2_KCES,             "Konami KCES Header"},
        {meta_PS2_DXH,              "Tokobot Plus DXH Header"},
        {meta_VSV,                  "Square Enix .vsv Header"},
        {meta_RIFF_WAVE_labl,       "RIFF WAVE header with loop markers"},
        {meta_RIFF_WAVE_smpl,       "RIFF WAVE header with sample looping info"},
        {meta_RIFF_WAVE_wsmp,       "RIFF WAVE header with wsmp looping info"},
        {meta_RIFX_WAVE,            "RIFX WAVE header"},
        {meta_RIFX_WAVE_smpl,       "RIFX WAVE header with sample looping info"},
        {meta_XNB,                  "Microsoft XNA Game Studio 4.0 header"},
        {meta_SCD_PCM,              "Lunar: Eternal Blue .PCM header"},
        {meta_PS2_PCM,              "Konami KCEJ East .PCM header"},
        {meta_PS2_RKV,              "Legacy of Kain - Blood Omen 2 RKV PS2 header"},
        {meta_PS2_VAS,              "Konami .VAS header"},
        {meta_PS2_TEC,              "assumed TECMO badflagged stream by .tec extension"},
        {meta_PS2_ENTH,             ".enth Header"},
        {meta_SDT,                  "High Voltage .sdt header"},
        {meta_NGC_TYDSP,            ".tydsp Header"},
        {meta_XBOX_WVS,             "Metal Arms WVS Header (XBOX)"},
        {meta_NGC_WVS,              "Metal Arms WVS Header (GameCube)"},
        {meta_XBOX_MATX,            "assumed Matrix file by .matx extension"},
        {meta_DEC,                  "Falcom DEC RIFF header"},
        {meta_VS,                   "Melbourne House .VS header"},
        {meta_DC_STR,               "Sega Stream Asset Builder header"},
        {meta_DC_STR_V2,            "variant of Sega Stream Asset Builder header"},
        {meta_XMU,                  "Outrage XMU header"},
        {meta_XVAS,                 "Konami .XVAS header"},
        {meta_PS2_XA2,              "Acclaim XA2 Header"},
        {meta_SAP,                  "VING .SAP header"},
        {meta_DC_IDVI,              "Capcom IDVI header"},
        {meta_KRAW,                 "Geometry Wars: Galaxies KRAW header"},
        {meta_NGC_YMF,              "YMF DSP Header"},
        {meta_PS2_CCC,              "CCC Header"},
        {meta_FAG,                  "Radical .FAG Header"},
        {meta_PS2_MIHB,             "Sony MultiStream MIC header"},
        {meta_DSP_WII_MUS,          "mus header"},
        {meta_WII_SNG,              "SNG DSP Header"},
        {meta_RSD,                  "Radical RSD header"},
        {meta_DC_ASD,               "ASD Header"},
        {meta_NAOMI_SPSD,           "Naomi SPSD header"},
        {meta_FFXI_BGW,             "Square Enix .BGW header"},
        {meta_FFXI_SPW,             "Square Enix .SPW header"},
        {meta_PS2_ASS,              "SystemSoft .ASS header"},
        {meta_NUB,                  "Namco NUB header"},
        {meta_IDSP_NL,              "Next Level IDSP header"},
        {meta_IDSP_IE,              "Inevitable Entertainment IDSP Header"},
        {meta_UBI_JADE,             "Ubisoft Jade RIFF header"},
        {meta_SEG,                  "Stormfront SEG header"},
        {meta_NDS_STRM_FFTA2,       "Final Fantasy Tactics A2 RIFF Header"},
        {meta_STR_ASR,              "Donkey Kong Jet Race KNON/WII Header"},
        {meta_ZWDSP,                "Zack and Wiki custom DSP Header"},
        {meta_GCA,                  "GCA DSP Header"},
        {meta_SPT_SPD,              "SPT+SPD DSP Header"},
        {meta_ISH_ISD,              "ISH+ISD DSP Header"},
        {meta_GSP_GSB,              "Tecmo GSP+GSB Header"},
        {meta_YDSP,                 "Yuke's DSP (YDSP) Header"},
        {meta_MSVP,                 "MSVP Header"},
        {meta_NGC_SSM,              "SSM DSP Header"},
        {meta_PS2_JOE,              "Asobo Studio .JOE header"},
        {meta_VGS,                  "Guitar Hero VGS Header"},
        {meta_DCS_WAV,              "In Utero DCS+WAV header"},
        {meta_SMP,                  "Infernal Engine .smp header"},
        {meta_MUL,                  "Crystal Dynamics .MUL header"},
        {meta_THP,                  "Nintendo THP header"},
        {meta_STS_WII,              "Shikigami no Shiro (WII) Header"},
        {meta_PS2_P2BT,             "Pop'n'Music 7 Header"},
        {meta_PS2_GBTS,             "Pop'n'Music 9 Header"},
        {meta_NGC_DSP_IADP,         "IADP Header"},
        {meta_RSTM_shrunken,        "Nintendo RSTM header, corrupted by Atlus"},
        {meta_RIFF_WAVE_MWV,        "RIFF WAVE header with .mwv flavoring"},
        {meta_FFCC_STR,             "Final Fantasy: Crystal Chronicles STR header"},
        {meta_SAT_BAKA,             "BAKA header from Crypt Killer"},
        {meta_NDS_SWAV,             "SWAV Header"},
        {meta_VSF,                  "Square-Enix VSF header"},
        {meta_NDS_RRDS,             "Ridger Racer DS Header"},
        {meta_PS2_TK5,              "Tekken 5 Stream Header"},
        {meta_PS2_SND,              "Might and Magic SSND Header"},
        {meta_PS2_VSF_TTA,          "VSF with SMSS Header"},
        {meta_ADS,                  "dhSS Header"},
        {meta_PS2_MCG,              "Gunvari MCG Header"},
        {meta_ZSD,                  "ZSD Header"},
        {meta_REDSPARK,             "RedSpark Header"},
        {meta_IVAUD,                "Rockstar .ivaud header"},
        {meta_DSP_WII_WSD,          ".WSD header"},
        {meta_WII_NDP,              "Icon Games NDP header"},
        {meta_PS2_SPS,              "Ape Escape 2 SPS Header"},
        {meta_PS2_XA2_RRP,          "Acclaim XA2 Header"},
        {meta_NDS_HWAS,             "Vicarious Visions HWAS header"},
        {meta_NGC_LPS,              "Rave Master LPS Header"},
        {meta_NAOMI_ADPCM,          "NAOMI/NAOMI2 Arcade games ADPCM header"},
        {meta_SD9,                  "beatmania IIDX SD9 header"},
        {meta_2DX9,                 "beatmania IIDX 2DX9 header"},
        {meta_DSP_YGO,              "Konami custom DSP Header"},
        {meta_PS2_VGV,              "Rune: Viking Warlord VGV Header"},
        {meta_NGC_GCUB,             "GCub Header"},
        {meta_NGC_SCK_DSP,          "The Scorpion King SCK Header"},
        {meta_CAFF,                 "Apple Core Audio Format File header"},
        {meta_PC_MXST,              "Lego Island MxSt Header"},
        {meta_SAB,                  "Team17 SAB header"},
        {meta_MAXIS_XA,             "Maxis XAI/XAJ Header"},
        {meta_EXAKT_SC,             "assumed Activision / EXAKT SC by extension"},
        {meta_WII_BNS,              "Nintendo BNS header"},
        {meta_WII_WAS,              "Sumo Digital iSWS header"},
        {meta_XBOX_HLWAV,           "Half-Life 2 .WAV header"},
        {meta_MYSPD,                "U-Sing .MYSPD header"},
        {meta_HIS,                  "Her Interactive HIS header"},
        {meta_PS2_AST,              "KOEI AST header"},
        {meta_CAPDSP,               "Capcom DSP header"},
        {meta_DMSG,                 "RIFF/DMSGsegh header"},
        {meta_PONA_3DO,             "Policenauts BGM header"},
        {meta_PONA_PSX,             "Policenauts BGM header"},
        {meta_NGC_DSP_AAAP,         "Acclaim Austin AAAp DSP header"},
        {meta_NGC_DSP_KONAMI,       "Konami DSP header"},
        {meta_PS2_STER,             "STER Header"},
        {meta_BNSF,                 "Namco Bandai BNSF header"},
        {meta_PS2_WB,               "Shooting Love. ~TRIZEAL~ WB header"},
        {meta_S14,                  "Namco .S14 raw header"},
        {meta_SSS,                  "Namco .SSS raw header"},
        {meta_PS2_GCM,              "GCM 'MCG' Header"},
        {meta_PS2_SMPL,             "Homura SMPL header"},
        {meta_PS2_MSA,              "Success .MSA header"},
        {meta_NGC_PDT,              "Hudson .PDT header"},
        {meta_NGC_RKV,              "Legacy of Kain - Blood Omen 2 RKV GC header"},
        {meta_DSP_DDSP,             ".DDSP header"},
        {meta_P3D,                  "Radical P3D header"},
        {meta_PS2_TK1,              "Tekken TK5STRM1 Header"},
        {meta_NGC_DSP_MPDS,         "MPDS DSP header"},
        {meta_DSP_STR_IG,           "Infogrames .DSP header"},
        {meta_EA_SWVR,              "Electronic Arts SWVR header"},
        {meta_PS2_B1S,              "B1S header"},
        {meta_PS2_WAD,              "WAD header"},
        {meta_DSP_XIII,             "XIII dsp header"},
        {meta_DSP_CABELAS,          "Cabelas games .DSP header"},
        {meta_PS2_ADM,              "Dragon Quest V .ADM raw header"},
        {meta_PS2_LPCM,             "LPCM header"},
        {meta_PS2_VMS,              "VMS Header"},
        {meta_XAU,                  "XPEC XAU header"},
        {meta_GH3_BAR,              "Guitar Hero III Mobile .bar"},
        {meta_FFW,                  "Freedom Fighters BGM header"},
        {meta_DSP_DSPW,             "Capcom DSPW header"},
        {meta_PS2_JSTM,             "JSTM Header"},
        {meta_XVAG,                 "Sony XVAG header"},
        {meta_PS3_CPS,              "tri-Crescendo CPS Header"},
        {meta_SQEX_SCD,             "Square-Enix SCD header"},
        {meta_NGC_NST_DSP,          "Animaniacs NST header"},
        {meta_BAF,                  "Bizarre Creations .baf header"},
        {meta_MSF,                  "Sony MSF header"},
        {meta_PS3_PAST,             "SNDP header"},
        {meta_SGXD,                 "Sony SGXD header"},
        {meta_NGCA,                 "NGCA header"},
        {meta_WII_RAS,              "RAS header"},
        {meta_PS2_SPM,              "SPM header"},
        {meta_X360_TRA,             "Terminal Reality .TRA raw header"},
        {meta_PS2_VGS,              "Princess Soft VGS header"},
        {meta_PS2_IAB,              "Runtime .IAB header"},
        {meta_VS_STR,               "Square .VS STR* header"},
        {meta_LSF_N1NJ4N,           ".lsf !n1nj4n header"},
        {meta_VAWX,                 "feelplus VAWX header"},
        {meta_RAW_SNDS,             "PC .snds raw header"},
        {meta_PS2_WMUS,             "assumed The Warriors Sony ADPCM by .wmus extension"},
        {meta_HYPERSCAN_KVAG,       "Mattel Hyperscan KVAG"},
        {meta_IOS_PSND,             "PSND Header"},
        {meta_BOS_ADP,              "ADP! header"},
        {meta_OTNS_ADP,             "Omikron: The Nomad Soul ADP header"},
        {meta_EB_SFX,               "Excitebots .sfx header"},
        {meta_EB_SF0,               "assumed Excitebots .sf0 by extension"},
        {meta_MTAF,                 "Konami MTAF header"},
        {meta_PS2_VAG1,             "Konami VAG1 header"},
        {meta_PS2_VAG2,             "Konami VAG2 header"},
        {meta_TUN,                  "Lego Racers ALP header"},
        {meta_WPD,                  "WPD 'DPW' header"},
        {meta_MN_STR,               "Mini Ninjas 'STR' header"},
        {meta_MSS,                  "Guerilla MCSS header"},
        {meta_PS2_HSF,              "Lowrider 'HSF' header"},
        {meta_IVAG,                 "Namco IVAG header"},
        {meta_PS2_2PFS,             "Konami 2PFS header"},
        {meta_UBI_CKD,              "Ubisoft CKD RIFF header"},
        {meta_PS2_VBK,              "PS2 VBK Header"},
        {meta_OTM,                  "Otomedius OTM Header"},
        {meta_CSTM,                 "Nintendo CSTM Header"},
        {meta_FSTM,                 "Nintendo FSTM Header"},
        {meta_KT_WIIBGM,            "Koei Tecmo WiiBGM Header"},
        {meta_KTSS,                 "Koei Tecmo Nintendo Stream KTSS Header"},
        {meta_IDSP_NAMCO,           "Namco IDSP header"},
        {meta_WIIU_BTSND,           "Nintendo Wii U Menu Boot Sound"},
        {meta_MCA,                  "Capcom MCA header"},
        {meta_XB3D_ADX,             "Xenoblade 3D ADX header"},
        {meta_HCA,                  "CRI HCA header"},
        {meta_PS2_SVAG_SNK,         "SNK SVAG header"},
        {meta_PS2_VDS_VDM,          "Procyon Studio VDS/VDM header"},
        {meta_FFMPEG,               "FFmpeg supported file format"},
        {meta_X360_CXS,             "tri-Crescendo CXS header"},
        {meta_AKB,                  "Square-Enix AKB header"},
        {meta_X360_PASX,            "Premium Agency PASX header"},
        {meta_XMA_RIFF,             "Microsoft XMA RIFF header"},
        {meta_X360_AST,             "Capcom AST (X360) header"},
        {meta_WWISE_RIFF,           "Audiokinetic Wwise RIFF header"},
        {meta_UBI_RAKI,             "Ubisoft RAKI header"},
        {meta_SXD,                  "Sony SXD header"},
        {meta_OGL,                  "Shin'en OGL header"},
        {meta_MC3,                  "Paradigm MC3 header"},
        {meta_GTD,                  "GTD/GHS header"},
        {meta_TA_AAC_X360,          "tri-Ace AAC (X360) header"},
        {meta_TA_AAC_PS3,           "tri-Ace AAC (PS3) header"},
        {meta_TA_AAC_MOBILE,        "tri-Ace AAC (Mobile) header"},
        {meta_MTA2,                 "Konami MTA2 header"},
        {meta_NGC_ULW,              "Criterion ULW raw header"},
        {meta_XA_XA30,              "Reflections XA30 header"},
        {meta_XA_04SW,              "Reflections 04SW header"},
        {meta_TXTH,                 "TXTH generic header"},
        {meta_EA_BNK,               "Electronic Arts BNK header"},
        {meta_SK_AUD,               "Silicon Knights AUD header"},
        {meta_AHX,                  "CRI AHX header"},
        {meta_STM,                  "Angel Studios/Rockstar San Diego STMA header"},
        {meta_BINK,                 "RAD Game Tools Bink header"},
        {meta_EA_SNU,               "Electronic Arts SNU header"},
        {meta_AWC,                  "Rockstar AWC header"},
        {meta_OPUS,                 "Nintendo Switch OPUS header"},
        {meta_PC_AL2,               "Illwinter Game Design AL2 raw header"},
        {meta_PC_AST,               "Capcom AST (PC) header"},
        {meta_UBI_SB,               "Ubisoft SBx header"},
        {meta_NAAC,                 "Namco NAAC header"},
        {meta_EZW,                  "EZ2DJ EZWAVE header"},
        {meta_VXN,                  "Gameloft VXN header"},
        {meta_EA_SNR_SNS,           "Electronic Arts SNR+SNS header"},
        {meta_EA_SPS,               "Electronic Arts SPS header"},
        {meta_NGC_VID1,             "Neversoft VID1 header"},
        {meta_PC_FLX,               "Ultima IX .FLX header"},
        {meta_MOGG,                 "Harmonix Music Systems MOGG Vorbis"},
        {meta_OGG_VORBIS,           "Ogg Vorbis header"},
        {meta_OGG_SLI,              "Ogg Vorbis header (.sli looping)"},
        {meta_OPUS_SLI,             "Ogg Opus header (.sli looping)"},
        {meta_OGG_SFL,              "Ogg Vorbis header (SFPL looping)"},
        {meta_OGG_KOVS,             "Ogg Vorbis header (KOVS)"},
        {meta_OGG_encrypted,        "Ogg Vorbis header (encrypted)"},
        {meta_KMA9,                 "Koei Tecmo KMA9 header"},
        {meta_XWC,                  "Starbreeze XWC header"},
        {meta_SQEX_SAB,             "Square-Enix SAB header"},
        {meta_SQEX_MAB,             "Square-Enix MAB header"},
        {meta_WAF,                  "KID WAF header"},
        {meta_WAVE,                 "EngineBlack .WAVE header"},
        {meta_WAVE_segmented,       "EngineBlack .WAVE header (segmented)"},
        {meta_SMV,                  "Cho Aniki Zero .SMV header"},
        {meta_NXAP,                 "Nex NXAP header"},
        {meta_EA_WVE_AU00,          "Electronic Arts WVE (au00) header"},
        {meta_EA_WVE_AD10,          "Electronic Arts WVE (Ad10) header"},
        {meta_STHD,                 "Dream Factory STHD header"},
        {meta_MP4,                  "MP4/AAC header"},
        {meta_PCM_SRE,              "Capcom .PCM+SRE header"},
        {meta_DSP_MCADPCM,          "Bethesda .mcadpcm header"},
        {meta_UBI_LYN,              "Ubisoft LyN RIFF header"},
        {meta_MSB_MSH,              "Sony MultiStream MSH+MSB header"},
        {meta_TXTP,                 "TXTP generic header"},
        {meta_SMC_SMH,              "Genki SMC+SMH header"},
        {meta_PPST,                 "Parappa PPST header"},
        {meta_OPUS_PPP,             "AT9 OPUS header"},
        {meta_UBI_BAO,              "Ubisoft BAO header"},
        {meta_DSP_SWITCH_AUDIO,     "UE4 Switch Audio header"},
        {meta_TA_AAC_VITA,          "tri-Ace AAC (Vita) header"},
        {meta_DSP_SADF,             "Procyon Studio SADF header"},
        {meta_H4M,                  "Hudson HVQM4 header"},
        {meta_ASF,                  "Argonaut ASF header"},
        {meta_XMD,                  "Konami XMD header"},
        {meta_CKS,                  "Cricket Audio CKS header"},
        {meta_CKB,                  "Cricket Audio CKB header"},
        {meta_WV6,                  "Gorilla Systems WV6 header"},
        {meta_WAVEBATCH,            "Firebrand Games WBAT header"},
        {meta_HD3_BD3,              "Sony HD3+BD3 header"},
        {meta_BNK_SONY,             "Sony BNK header"},
        {meta_SCD_SSCF,             "Square-Enix SCD (SSCF) header"},
        {meta_DSP_VAG,              ".VAG DSP header"},
        {meta_DSP_ITL,              ".ITL DSP header"},
        {meta_A2M,                  "Artificial Mind & Movement A2M header"},
        {meta_AHV,                  "Amuze AHV header"},
        {meta_MSV,                  "Sony MultiStream MSV header"},
        {meta_SDF,                  "Beyond Reality SDF header"},
        {meta_SVG,                  "High Voltage SVG header"},
        {meta_VIS,                  "Konami VIS header"},
        {meta_VAI,                  "Asobo Studio .VAI header"},
        {meta_AIF_ASOBO,            "Asobo Studio .AIF header"},
        {meta_AO,                   "AlphaOgg .AO header"},
        {meta_APC,                  "Cryo APC header"},
        {meta_WV2,                  "Infogrames North America WAV2 header"},
        {meta_XAU_KONAMI,           "Konami XAU header"},
        {meta_DERF,                 "Xilam DERF header"},
        {meta_UTK,                  "Maxis UTK header"},
        {meta_NXA,                  "Entergram NXA header"},
        {meta_ADPCM_CAPCOM,         "Capcom .ADPCM header"},
        {meta_UE4OPUS,              "Epic Games UE4OPUS header"},
        {meta_XWMA,                 "Microsoft XWMA RIFF header"},
        {meta_VA3,                  "Konami VA3 header" },
        {meta_XOPUS,                "Exient XOPUS header"},
        {meta_VS_SQUARE,            "Square VS header"},
        {meta_NWAV,                 "Chunsoft NWAV header"},
        {meta_XPCM,                 "Circus XPCM header"},
        {meta_MSF_TAMASOFT,         "Tama-Soft MSF header"},
        {meta_XPS_DAT,              "From Software .XPS+DAT header"},
        {meta_ZSND,                 "Z-Axis ZSND header"},
        {meta_DSP_ADPY,             "AQUASTYLE ADPY header"},
        {meta_DSP_ADPX,             "AQUASTYLE ADPX header"},
        {meta_OGG_OPUS,             "Ogg Opus header"},
        {meta_IMC,                  "iNiS .IMC header"},
        {meta_GIN,                  "Electronic Arts Gnsu header"},
        {meta_DSF,                  "Ocean DSF header"},
        {meta_208,                  "Ocean .208 header"},
        {meta_DSP_DS2,              "LucasArts .DS2 header"},
        {meta_MUS_VC,               "Vicious Cycle .MUS header"},
        {meta_STRM_ABYLIGHT,        "Abylight STRM header"},
        {meta_MSF_KONAMI,           "Konami MSF header"},
        {meta_XWMA_KONAMI,          "Konami XWMA header"},
        {meta_9TAV,                 "Konami 9TAV header"},
        {meta_BWAV,                 "Nintendo BWAV header"},
        {meta_RAD,                  "Traveller's Tales .RAD header"},
        {meta_SMACKER,              "RAD Game Tools SMACKER header"},
        {meta_MZRT,                 "id Software MZRT header"},
        {meta_XAVS,                 "Reflections XAVS header"},
        {meta_PSF,                  "Pivotal PSF header"},
        {meta_DSP_ITL_i,            "Infernal .ITL DSP header"},
        {meta_IMA,                  "Blitz Games .IMA header"},
        {meta_XMV_VALVE,            "Valve XMV header"},
        {meta_UBI_HX,               "Ubisoft HXx header"},
        {meta_BMP_KONAMI,           "Konami BMP header"},
        {meta_ISB,                  "Creative ISACT header"},
        {meta_XSSB,                 "Artoon XSSB header"},
        {meta_XMA_UE3,              "Unreal Engine XMA header"},
        {meta_FWSE,                 "MT Framework FWSE header"},
        {meta_FDA,                  "Relic FDA header"},
        {meta_TGC,                  "Tiger Game.com .4 header"},
        {meta_KWB,                  "Koei Tecmo WaveBank header"},
        {meta_LRMD,                 "Sony LRMD header"},

};

void get_vgmstream_coding_description(VGMSTREAM *vgmstream, char *out, size_t out_size) {
    int i, list_length;
    const char *description;

    /* we need to recurse down because of FFmpeg */
    if (vgmstream->layout_type == layout_layered) {
        layered_layout_data* layout_data = vgmstream->layout_data;
        get_vgmstream_coding_description(layout_data->layers[0], out, out_size);
        return;
    } else if (vgmstream->layout_type == layout_segmented) {
        segmented_layout_data* layout_data = vgmstream->layout_data;
        get_vgmstream_coding_description(layout_data->segments[0], out, out_size);
        return;
    }

    description = "CANNOT DECODE";

    switch (vgmstream->coding_type) {
#ifdef VGM_USE_FFMPEG
        case coding_FFmpeg:
            description = ffmpeg_get_codec_name(vgmstream->codec_data);
            if (description == NULL)
                description = "FFmpeg";
            break;
#endif
        default:
            list_length = sizeof(coding_info_list) / sizeof(coding_info);
            for (i = 0; i < list_length; i++) {
                if (coding_info_list[i].type == vgmstream->coding_type)
                    description = coding_info_list[i].description;
            }
            break;
    }

    strncpy(out, description, out_size);
}
const char * get_vgmstream_layout_name(layout_t layout_type) {
    int i, list_length;

    list_length = sizeof(layout_info_list) / sizeof(layout_info);
    for (i = 0; i < list_length; i++) {
        if (layout_info_list[i].type == layout_type)
            return layout_info_list[i].description;
    }

    return NULL;
}
void get_vgmstream_layout_description(VGMSTREAM *vgmstream, char *out, size_t out_size) {
    char temp[256];
    VGMSTREAM* vgmstreamsub = NULL;
    const char* description;

    description = get_vgmstream_layout_name(vgmstream->layout_type);
    if (!description) description = "INCONCEIVABLE";

    if (vgmstream->layout_type == layout_layered) {
        vgmstreamsub = ((layered_layout_data*)vgmstream->layout_data)->layers[0];
        snprintf(temp, sizeof(temp), "%s (%i layers)", description, ((layered_layout_data*)vgmstream->layout_data)->layer_count);
    } else if (vgmstream->layout_type == layout_segmented) {
        snprintf(temp, sizeof(temp), "%s (%i segments)", description, ((segmented_layout_data*)vgmstream->layout_data)->segment_count);
        vgmstreamsub = ((segmented_layout_data*)vgmstream->layout_data)->segments[0];
    } else {
        snprintf(temp, sizeof(temp), "%s", description);
    }
    strncpy(out, temp, out_size);

    /* layouts can contain layouts infinitely let's leave it at one level deep (most common) */
    /* TODO: improve this somehow */
    if (vgmstreamsub && vgmstreamsub->layout_type == layout_layered) {
        description = get_vgmstream_layout_name(vgmstreamsub->layout_type);
        snprintf(temp, sizeof(temp), " + %s (%i layers)", description, ((layered_layout_data*)vgmstreamsub->layout_data)->layer_count);
        concatn(out_size, out, temp);
    } else if (vgmstreamsub && vgmstreamsub->layout_type == layout_segmented) {
        description = get_vgmstream_layout_name(vgmstreamsub->layout_type);
        snprintf(temp, sizeof(temp), " + %s (%i segments)", description, ((segmented_layout_data*)vgmstream->layout_data)->segment_count);
        concatn(out_size, out, temp);
    }
}
void get_vgmstream_meta_description(VGMSTREAM *vgmstream, char *out, size_t out_size) {
    int i, list_length;
    const char *description;

    description = "THEY SHOULD HAVE SENT A POET";

    list_length = sizeof(meta_info_list) / sizeof(meta_info);
    for (i=0; i < list_length; i++) {
        if (meta_info_list[i].type == vgmstream->meta_type)
            description = meta_info_list[i].description;
    }

    strncpy(out, description, out_size);
}
