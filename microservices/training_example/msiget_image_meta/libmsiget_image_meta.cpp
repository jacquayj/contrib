// =-=-=-=-=-=-=-
#include "apiHeaderAll.hpp"
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
#include "irods_ms_plugin.hpp"

// =-=-=-=-=-=-=-
// STL/boost Includes
#include <string>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <ImageMagick/Magick++.h>

std::string convertCompressTypeToStr(const MagickCore::CompressionType& t);
std::string convertColorSpaceTypeToStr(const MagickCore::ColorspaceType& t);

using namespace Magick;

extern "C" {
    double get_plugin_interface_version() {
        return 1.0;
    }

    // =-=-=-=-=-=-=-
    // Returns the meta data as a string for the image.  Example:  CompressionType=JPEG%Width=10%Height=20
    int msiget_image_meta_impl(msParam_t* _in, msParam_t* _out, ruleExecInfo_t* rei) {
        using std::cout;
        using std::endl;
        using std::string;

        char *filePath = parseMspForStr( _in );
        if( !filePath ) {
            cout << "msiget_image_meta - null filePath parameter" << endl;
            return SYS_INVALID_INPUT_PARAM;
        }

        InitializeMagick((const char*)0);

        Image imgObj;
        imgObj.read(filePath);

        std::stringstream metaData;
        metaData << "ImageDepth=" << imgObj.modulusDepth()
                 << "%Width=" << imgObj.columns()
                 << "%Height=" << imgObj.rows()
                 << "%CompressionType=" << convertCompressTypeToStr(imgObj.compressType())
                 << "%Format=" << imgObj.format()
                 << "%Colorspace=" << convertColorSpaceTypeToStr(imgObj.colorSpace());

        fillStrInMsParam(_out, metaData.str().c_str());

        // Done
        return 0;

    }

    irods::ms_table_entry* plugin_factory() {
        irods::ms_table_entry* msvc = new irods::ms_table_entry(2);

        msvc->add_operation("msiget_image_meta_impl", "msiget_image_meta");

        return msvc;
    }

} // extern "C"

std::string convertCompressTypeToStr(const MagickCore::CompressionType& t) {

	switch (t) {
	case UndefinedCompression:
		return "undefined";
	case NoCompression:
		return "none";
	case FaxCompression:
		return "FAX";
	case Group4Compression:
		return "GROUP4";
	case JPEGCompression:
		return "JPEG";
	case LZWCompression:
		return "LZW";
	case RLECompression:
		return "RLE";
	case LZMACompression:
		return "LZMA";
	default:
		return "undefined";
	}
}

std::string convertColorSpaceTypeToStr(const MagickCore::ColorspaceType& t) {
	switch (t) {
	case UndefinedColorspace:
		return "undefined";
	case RGBColorspace:
		return "RGB";
	case GRAYColorspace:
		return "GRAY";
	case TransparentColorspace:
		return "TRANSPARENT";
	case OHTAColorspace:
		return "OHTA";
	case XYZColorspace:
		return "XYZ";
	case YCbCrColorspace:
		return "YCbCr";
	case YCCColorspace:
		return "YCC";
	case YIQColorspace:
		return "YIQ";
	case YPbPrColorspace:
		return "YPbPr";
	case YUVColorspace:
		return "YUV";
	case CMYKColorspace:
		return "CMYK";
	case sRGBColorspace:
		return "sRGB";
	case HSLColorspace:
		return "HSL";
	case HWBColorspace:
		return "HWB";
	case Rec601LumaColorspace:
		return "REC601Luma";
	case Rec709LumaColorspace:
		return "REC709";
	case LogColorspace:
		return "LOG";
	default:
		return "undefined";
	}
}
