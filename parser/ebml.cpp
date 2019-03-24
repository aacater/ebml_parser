// ebml.cpp
// function definitions for ebml.h

#include "ebml.h"

// string array of ebml type names
string ebmlTypeName[8] = { "MASTER", "UNIT", "INT", "STRING", "UTF8", "BINARY", "FLOAT", "DATE" };

// returns string of the name of the embl element type based on the enum value
string getebmlTypeName(enum ebml_element_type type)
{
	return ebmlTypeName[type];
}

// data taken from 
// https://www.matroska.org/technical/specs/index.html
// each array index has Element Name, Element ID, and Element Type
const ebml_element * ebml_spec[SPEC_LEN] =
{
	new ebml_element("EBML", {{0x1a, 0x45, 0xdf, 0xa3}}, MASTER),
	new ebml_element("EBMLVersion", {{0x42, 0x86}}, UINT),
	new ebml_element("EBMLReadVersion", {{0x42, 0xf7}}, UINT),
	new ebml_element("EBMLMaxIDLength", {{0x42, 0xf2}}, UINT),
	new ebml_element("EBMLMaxSizeLength", {{0x42, 0xf3}}, UINT),
	new ebml_element("DocType", {{0x42, 0x82}}, STRING),
	new ebml_element("DocTypeVersion", {{0x42, 0x87}}, UINT),
	new ebml_element("DocTypeReadVersion", {{0x42, 0x85}}, UINT),
	new ebml_element("Void", {{0xec}}, BINARY),
	new ebml_element("CRC-32", {{0xbf}}, BINARY),
	new ebml_element("SignatureSlot", {{0x1b, 0x53, 0x86, 0x67}}, MASTER),
	new ebml_element("SignatureAlgo", {{0x7e, 0x8a}}, UINT),
	new ebml_element("SignatureHash", {{0x7e, 0x9a}}, UINT),
	new ebml_element("SignaturePublicKey", {{0x7e, 0xa5}}, BINARY),
	new ebml_element("Signature", {{0x7e, 0xb5}}, BINARY),
	new ebml_element("SignatureElements", {{0x7e, 0x5b}}, MASTER),
	new ebml_element("SignatureElementList", {{0x7e, 0x7b}}, MASTER),
	new ebml_element("SignedElement", {{0x65, 0x32}}, BINARY),
	new ebml_element("Segment", {{0x18, 0x53, 0x80, 0x67}}, MASTER),
	new ebml_element("SeekHead", {{0x11, 0x4d, 0x9b, 0x74}}, MASTER),
	new ebml_element("Seek", {{0x4d, 0xbb}}, MASTER),
	new ebml_element("SeekID", {{0x53, 0xab}}, BINARY),
	new ebml_element("SeekPosition", {{0x53, 0xac}}, UINT),
	new ebml_element("Info", {{0x15, 0x49, 0xa9, 0x66}}, MASTER),
	new ebml_element("SegmentUID", {{0x73, 0xa4}}, BINARY),
	new ebml_element("SegmentFilename", {{0x73, 0x84}}, UTF8),
	new ebml_element("PrevUID", {{0x3c, 0xb9, 0x23}}, BINARY),
	new ebml_element("PrevFilename", {{0x3c, 0x83, 0xab}}, UTF8),
	new ebml_element("NextUID", {{0x3e, 0xb9, 0x23}}, BINARY),
	new ebml_element("NextFilename", {{0x3e, 0x83, 0xbb}}, UTF8),
	new ebml_element("SegmentFamily", {{0x44, 0x44}}, BINARY),
	new ebml_element("ChapterTranslate", {{0x69, 0x24}}, MASTER),
	new ebml_element("ChapterTranslateEditionUID", {{0x69, 0xfc}}, UINT),
	new ebml_element("ChapterTranslateCodec", {{0x69, 0xbf}}, UINT),
	new ebml_element("ChapterTranslateID", {{0x69, 0xa5}}, BINARY),
	new ebml_element("TimecodeScale", {{0x2a, 0xd7, 0xb1}}, UINT),
	new ebml_element("Duration", {{0x44, 0x89}}, FLOAT),
	new ebml_element("DateUTC", {{0x44, 0x61}}, DATE),
	new ebml_element("Title", {{0x7b, 0xa9}}, UTF8),
	new ebml_element("MuxingApp", {{0x4d, 0x80}}, UTF8),
	new ebml_element("WritingApp", {{0x57, 0x41}}, UTF8),
	new ebml_element("Cluster", {{0x1f, 0x43, 0xb6, 0x75}}, MASTER),
	new ebml_element("Timecode", {{0xe7}}, UINT),
	new ebml_element("SilentTracks", {{0x58, 0x54}}, MASTER),
	new ebml_element("SilentTrackNumber", {{0x58, 0xd7}}, UINT),
	new ebml_element("Position", {{0xa7}}, UINT),
	new ebml_element("PrevSize", {{0xab}}, UINT),
	new ebml_element("SimpleBlock", {{0xa3}}, BINARY),
	new ebml_element("BlockGroup", {{0xa0}}, MASTER),
	new ebml_element("Block", {{0xa1}}, BINARY),
	new ebml_element("BlockVirtual", {{0xa2}}, BINARY),
	new ebml_element("BlockAdditions", {{0x75, 0xa1}}, MASTER),
	new ebml_element("BlockMore", {{0xa6}}, MASTER),
	new ebml_element("BlockAddID", {{0xee}}, UINT),
	new ebml_element("BlockAdditional", {{0xa5}}, BINARY),
	new ebml_element("BlockDuration", {{0x9b}}, UINT),
	new ebml_element("ReferencePriority", {{0xfa}}, UINT),
	new ebml_element("ReferenceBlock", {{0xfb}}, INT),
	new ebml_element("ReferenceVirtual", {{0xfd}}, INT),
	new ebml_element("CodecState", {{0xa4}}, BINARY),
	new ebml_element("DiscardPadding", {{0x75, 0xa2}}, INT),
	new ebml_element("Slices", {{0x8e}}, MASTER),
	new ebml_element("TimeSlice", {{0xe8}}, MASTER),
	new ebml_element("LaceNumber", {{0xcc}}, UINT),
	new ebml_element("FrameNumber", {{0xcd}}, UINT),
	new ebml_element("BlockAdditionID", {{0xcb}}, UINT),
	new ebml_element("Delay", {{0xce}}, UINT),
	new ebml_element("SliceDuration", {{0xcf}}, UINT),
	new ebml_element("ReferenceFrame", {{0xc8}}, MASTER),
	new ebml_element("ReferenceOffset", {{0xc9}}, UINT),
	new ebml_element("ReferenceTimeCode", {{0xca}}, UINT),
	new ebml_element("EncryptedBlock", {{0xaf}}, BINARY),
	new ebml_element("Tracks", {{0x16, 0x54, 0xae, 0x6b}}, MASTER),
	new ebml_element("TrackEntry", {{0xae}}, MASTER),
	new ebml_element("TrackNumber", {{0xd7}}, UINT),
	new ebml_element("TrackUID", {{0x73, 0xc5}}, UINT),
	new ebml_element("TrackType", {{0x83}}, UINT),
	new ebml_element("FlagEnabled", {{0xb9}}, UINT),
	new ebml_element("FlagDefault", {{0x88}}, UINT),
	new ebml_element("FlagForced", {{0x55, 0xaa}}, UINT),
	new ebml_element("FlagLacing", {{0x9c}}, UINT),
	new ebml_element("MinCache", {{0x6d, 0xe7}}, UINT),
	new ebml_element("MaxCache", {{0x6d, 0xf8}}, UINT),
	new ebml_element("DefaultDuration", {{0x23, 0xe3, 0x83}}, UINT),
	new ebml_element("DefaultDecodedFieldDuration", {{0x23, 0x4e, 0x7a}}, UINT),
	new ebml_element("TrackTimecodeScale", {{0x23, 0x31, 0x4f}}, FLOAT),
	new ebml_element("TrackOffset", {{0x53, 0x7f}}, INT),
	new ebml_element("MaxBlockAdditionID", {{0x55, 0xee}}, UINT),
	new ebml_element("Name", {{0x53, 0x6e}}, UTF8),
	new ebml_element("Language", {{0x22, 0xb5, 0x9c}}, STRING),
	new ebml_element("CodecID", {{0x86}}, STRING),
	new ebml_element("CodecPrivate", {{0x63, 0xa2}}, BINARY),
	new ebml_element("CodecName", {{0x25, 0x86, 0x88}}, UTF8),
	new ebml_element("AttachmentLink", {{0x74, 0x46}}, UINT),
	new ebml_element("CodecSettings", {{0x3a, 0x96, 0x97}}, UTF8),
	new ebml_element("CodecInfoURL", {{0x3b, 0x40, 0x40}}, STRING),
	new ebml_element("CodecDownloadURL", {{0x26, 0xb2, 0x40}}, STRING),
	new ebml_element("CodecDecodeAll", {{0xaa}}, UINT),
	new ebml_element("TrackOverlay", {{0x6f, 0xab}}, UINT),
	new ebml_element("CodecDelay", {{0x56, 0xaa}}, UINT),
	new ebml_element("SeekPreRoll", {{0x56, 0xbb}}, UINT),
	new ebml_element("TrackTranslate", {{0x66, 0x24}}, MASTER),
	new ebml_element("TrackTranslateEditionUID", {{0x66, 0xfc}}, UINT),
	new ebml_element("TrackTranslateCodec", {{0x66, 0xbf}}, UINT),
	new ebml_element("TrackTranslateTrackID", {{0x66, 0xa5}}, BINARY),
	new ebml_element("Video", {{0xe0}}, MASTER),
	new ebml_element("FlagInterlaced", {{0x9a}}, UINT),
	new ebml_element("FieldOrder", {{0x9d}}, UINT),
	new ebml_element("StereoMode", {{0x53, 0xb8}}, UINT),
	new ebml_element("AlphaMode", {{0x53, 0xc0}}, UINT),
	new ebml_element("OldStereoMode", {{0x53, 0xb9}}, UINT),
	new ebml_element("PixelWidth", {{0xb0}}, UINT),
	new ebml_element("PixelHeight", {{0xba}}, UINT),
	new ebml_element("PixelCropBottom", {{0x54, 0xaa}}, UINT),
	new ebml_element("PixelCropTop", {{0x54, 0xbb}}, UINT),
	new ebml_element("PixelCropLeft", {{0x54, 0xcc}}, UINT),
	new ebml_element("PixelCropRight", {{0x54, 0xdd}}, UINT),
	new ebml_element("DisplayWidth", {{0x54, 0xb0}}, UINT),
	new ebml_element("DisplayHeight", {{0x54, 0xba}}, UINT),
	new ebml_element("DisplayUnit", {{0x54, 0xb2}}, UINT),
	new ebml_element("AspectRatioType", {{0x54, 0xb3}}, UINT),
	new ebml_element("ColourSpace", {{0x2e, 0xb5, 0x24}}, BINARY),
	new ebml_element("GammaValue", {{0x2f, 0xb5, 0x23}}, FLOAT),
	new ebml_element("FrameRate", {{0x23, 0x83, 0xe3}}, FLOAT),
	new ebml_element("Colour", {{0x55, 0xb0}}, MASTER),
	new ebml_element("MatrixCoefficients", {{0x55, 0xb1}}, UINT),
	new ebml_element("BitsPerChannel", {{0x55, 0xb2}}, UINT),
	new ebml_element("ChromaSubsamplingHorz", {{0x55, 0xb3}}, UINT),
	new ebml_element("ChromaSubsamplingVert", {{0x55, 0xb4}}, UINT),
	new ebml_element("CbSubsamplingHorz", {{0x55, 0xb5}}, UINT),
	new ebml_element("CbSubsamplingVert", {{0x55, 0xb6}}, UINT),
	new ebml_element("ChromaSitingHorz", {{0x55, 0xb7}}, UINT),
	new ebml_element("ChromaSitingVert", {{0x55, 0xb8}}, UINT),
	new ebml_element("Range", {{0x55, 0xb9}}, UINT),
	new ebml_element("TransferCharacteristics", {{0x55, 0xba}}, UINT),
	new ebml_element("Primaries", {{0x55, 0xbb}}, UINT),
	new ebml_element("MaxCLL", {{0x55, 0xbc}}, UINT),
	new ebml_element("MaxFALL", {{0x55, 0xbd}}, UINT),
	new ebml_element("MasteringMetadata", {{0x55, 0xd0}}, MASTER),
	new ebml_element("PrimaryRChromaticityX", {{0x55, 0xd1}}, FLOAT),
	new ebml_element("PrimaryRChromaticityY", {{0x55, 0xd2}}, FLOAT),
	new ebml_element("PrimaryGChromaticityX", {{0x55, 0xd3}}, FLOAT),
	new ebml_element("PrimaryGChromaticityY", {{0x55, 0xd4}}, FLOAT),
	new ebml_element("PrimaryBChromaticityX", {{0x55, 0xd5}}, FLOAT),
	new ebml_element("PrimaryBChromaticityY", {{0x55, 0xd6}}, FLOAT),
	new ebml_element("WhitePointChromaticityX", {{0x55, 0xd7}}, FLOAT),
	new ebml_element("WhitePointChromaticityY", {{0x55, 0xd8}}, FLOAT),
	new ebml_element("LuminanceMax", {{0x55, 0xd9}}, FLOAT),
	new ebml_element("LuminanceMin", {{0x55, 0xda}}, FLOAT),
	new ebml_element("Audio", {{0xe1}}, MASTER),
	new ebml_element("SamplingFrequency", {{0xb5}}, FLOAT),
	new ebml_element("OutputSamplingFrequency", {{0x78, 0xb5}}, FLOAT),
	new ebml_element("Channels", {{0x9f}}, UINT),
	new ebml_element("ChannelPositions", {{0x7d, 0x7b}}, BINARY),
	new ebml_element("BitDepth", {{0x62, 0x64}}, UINT),
	new ebml_element("TrackOperation", {{0xe2}}, MASTER),
	new ebml_element("TrackCombinePlanes", {{0xe3}}, MASTER),
	new ebml_element("TrackPlane", {{0xe4}}, MASTER),
	new ebml_element("TrackPlaneUID", {{0xe5}}, UINT),
	new ebml_element("TrackPlaneType", {{0xe6}}, UINT),
	new ebml_element("TrackJoinBlocks", {{0xe9}}, MASTER),
	new ebml_element("TrackJoinUID", {{0xed}}, UINT),
	new ebml_element("TrickTrackUID", {{0xc0}}, UINT),
	new ebml_element("TrickTrackSegmentUID", {{0xc1}}, BINARY),
	new ebml_element("TrickTrackFlag", {{0xc6}}, UINT),
	new ebml_element("TrickMasterTrackUID", {{0xc7}}, UINT),
	new ebml_element("TrickMasterTrackSegmentUID", {{0xc4}}, BINARY),
	new ebml_element("ContentEncodings", {{0x6d, 0x80}}, MASTER),
	new ebml_element("ContentEncoding", {{0x62, 0x40}}, MASTER),
	new ebml_element("ContentEncodingOrder", {{0x50, 0x31}}, UINT),
	new ebml_element("ContentEncodingScope", {{0x50, 0x32}}, UINT),
	new ebml_element("ContentEncodingType", {{0x50, 0x33}}, UINT),
	new ebml_element("ContentCompression", {{0x50, 0x34}}, MASTER),
	new ebml_element("ContentCompAlgo", {{0x42, 0x54}}, UINT),
	new ebml_element("ContentCompSettings", {{0x42, 0x55}}, BINARY),
	new ebml_element("ContentEncryption", {{0x50, 0x35}}, MASTER),
	new ebml_element("ContentEncAlgo", {{0x47, 0xe1}}, UINT),
	new ebml_element("ContentEncKeyID", {{0x47, 0xe2}}, BINARY),
	new ebml_element("ContentSignature", {{0x47, 0xe3}}, BINARY),
	new ebml_element("ContentSigKeyID", {{0x47, 0xe4}}, BINARY),
	new ebml_element("ContentSigAlgo", {{0x47, 0xe5}}, UINT),
	new ebml_element("ContentSigHashAlgo", {{0x47, 0xe6}}, UINT),
	new ebml_element("Cues", {{0x1c, 0x53, 0xbb, 0x6b}}, MASTER),
	new ebml_element("CuePoint", {{0xbb}}, MASTER),
	new ebml_element("CueTime", {{0xb3}}, UINT),
	new ebml_element("CueTrackPositions", {{0xb7}}, MASTER),
	new ebml_element("CueTrack", {{0xf7}}, UINT),
	new ebml_element("CueClusterPosition", {{0xf1}}, UINT),
	new ebml_element("CueRelativePosition", {{0xf0}}, UINT),
	new ebml_element("CueDuration", {{0xb2}}, UINT),
	new ebml_element("CueBlockNumber", {{0x53, 0x78}}, UINT),
	new ebml_element("CueCodecState", {{0xea}}, UINT),
	new ebml_element("CueReference", {{0xdb}}, MASTER),
	new ebml_element("CueRefTime", {{0x96}}, UINT),
	new ebml_element("CueRefCluster", {{0x97}}, UINT),
	new ebml_element("CueRefNumber", {{0x53, 0x5f}}, UINT),
	new ebml_element("CueRefCodecState", {{0xeb}}, UINT),
	new ebml_element("Attachments", {{0x19, 0x41, 0xa4, 0x69}}, MASTER),
	new ebml_element("AttachedFile", {{0x61, 0xa7}}, MASTER),
	new ebml_element("FileDescription", {{0x46, 0x7e}}, UTF8),
	new ebml_element("FileName", {{0x46, 0x6e}}, UTF8),
	new ebml_element("FileMimeType", {{0x46, 0x60}}, STRING),
	new ebml_element("FileData", {{0x46, 0x5c}}, BINARY),
	new ebml_element("FileUID", {{0x46, 0xae}}, UINT),
	new ebml_element("FileReferral", {{0x46, 0x75}}, BINARY),
	new ebml_element("FileUsedStartTime", {{0x46, 0x61}}, UINT),
	new ebml_element("FileUsedEndTime", {{0x46, 0x62}}, UINT),
	new ebml_element("Chapters", {{0x10, 0x43, 0xa7, 0x70}}, MASTER),
	new ebml_element("EditionEntry", {{0x45, 0xb9}}, MASTER),
	new ebml_element("EditionUID", {{0x45, 0xbc}}, UINT),
	new ebml_element("EditionFlagHidden", {{0x45, 0xbd}}, UINT),
	new ebml_element("EditionFlagDefault", {{0x45, 0xdb}}, UINT),
	new ebml_element("EditionFlagOrdered", {{0x45, 0xdd}}, UINT),
	new ebml_element("ChapterAtom", {{0xb6}}, MASTER),
	new ebml_element("ChapterUID", {{0x73, 0xc4}}, UINT),
	new ebml_element("ChapterStringUID", {{0x56, 0x54}}, UTF8),
	new ebml_element("ChapterTimeStart", {{0x91}}, UINT),
	new ebml_element("ChapterTimeEnd", {{0x92}}, UINT),
	new ebml_element("ChapterFlagHidden", {{0x98}}, UINT),
	new ebml_element("ChapterFlagEnabled", {{0x45, 0x98}}, UINT),
	new ebml_element("ChapterSegmentUID", {{0x6e, 0x67}}, BINARY),
	new ebml_element("ChapterSegmentEditionUID", {{0x6e, 0xbc}}, UINT),
	new ebml_element("ChapterPhysicalEquiv", {{0x63, 0xc3}}, UINT),
	new ebml_element("ChapterTrack", {{0x8f}}, MASTER),
	new ebml_element("ChapterTrackNumber", {{0x89}}, UINT),
	new ebml_element("ChapterDisplay", {{0x80}}, MASTER),
	new ebml_element("ChapString", {{0x85}}, UTF8),
	new ebml_element("ChapLanguage", {{0x43, 0x7c}}, STRING),
	new ebml_element("ChapCountry", {{0x43, 0x7e}}, STRING),
	new ebml_element("ChapProcess", {{0x69, 0x44}}, MASTER),
	new ebml_element("ChapProcessCodecID", {{0x69, 0x55}}, UINT),
	new ebml_element("ChapProcessPrivate", {{0x45, 0x0d}}, BINARY),
	new ebml_element("ChapProcessCommand", {{0x69, 0x11}}, MASTER),
	new ebml_element("ChapProcessTime", {{0x69, 0x22}}, UINT),
	new ebml_element("ChapProcessData", {{0x69, 0x33}}, BINARY),
	new ebml_element("Tags", {{0x12, 0x54, 0xc3, 0x67}}, MASTER),
	new ebml_element("Tag", {{0x73, 0x73}}, MASTER),
	new ebml_element("Targets", {{0x63, 0xc0}}, MASTER),
	new ebml_element("TargetTypeValue", {{0x68, 0xca}}, UINT),
	new ebml_element("TargetType", {{0x63, 0xca}}, STRING),
	new ebml_element("TagTrackUID", {{0x63, 0xc5}}, UINT),
	new ebml_element("TagEditionUID", {{0x63, 0xc9}}, UINT),
	new ebml_element("TagChapterUID", {{0x63, 0xc4}}, UINT),
	new ebml_element("TagAttachmentUID", {{0x63, 0xc6}}, UINT),
	new ebml_element("SimpleTag", {{0x67, 0xc8}}, MASTER),
	new ebml_element("TagName", {{0x45, 0xa3}}, UTF8),
	new ebml_element("TagLanguage", {{0x44, 0x7a}}, STRING),
	new ebml_element("TagDefault", {{0x44, 0x84}}, UINT),
	new ebml_element("TagString", {{0x44, 0x87}}, UTF8),
	new ebml_element("TagBinary", {{0x44, 0x85}}, BINARY)
};