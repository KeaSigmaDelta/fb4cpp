//
// Uri-Parsed from https://github.com/CovenantEyes/uri-parser
// modified by JPHW for using in facebook login sequence
//

#include <iostream>
#include <string>
#include <stdlib.h>

namespace http {
    struct url {
        std::string host, search, anchor, access_token, expires_in, error, error_reason;
    };

    //--- Helper Functions -------------------------------------------------------------~
    static inline std::string TailSlice(std::string &subject, std::string delimiter, bool keep_delim=false) {
        // returns everything after the delimiter
        auto delimiter_location = subject.find(delimiter);
        auto delimiter_length = delimiter.length();
        std::string output = "";

        if (delimiter_location < std::string::npos) {
            auto start = keep_delim ? delimiter_location : delimiter_location + delimiter_length;
            auto end = subject.length() - start;
            output = subject.substr(start, end);
        }
        return output;
    }

    static inline std::string HeadSlice(std::string &subject, std::string delimiter) {
        // returns everthing before the delimeter
        auto delimiter_location = subject.find(delimiter);
        std::string output = subject;
        if (delimiter_location < std::string::npos) {
            output = subject.substr(0, delimiter_location);
        }
        return output;
    }


    //--- Extractors -------------------------------------------------------------------~
    static inline int ExtractPort(std::string &hostport) {
        int port;
        std::string portstring = TailSlice(hostport, ":");
        try { port = atoi(portstring.c_str()); }
        catch (std::exception e) { port = -1; }
        return port;
    }

    static inline std::string ExtractHost(std::string &in)
    {
        std::string host_with_garbage = TailSlice( in, "://" );
        return HeadSlice( host_with_garbage, "/" );
    };
    
    static inline std::string ExtractSearch(std::string &in) { return TailSlice(in, "?"); };
    
    static inline std::string ExtractAnchor(std::string &in) { return TailSlice(in, "#"); }
    
    static inline std::string ExtractAccessToken(std::string &anchor)
    {
        std::string token_with_garbage = TailSlice(anchor, "access_token=");
        return HeadSlice( token_with_garbage, "&" );
    };
    
    static inline std::string ExtractExpiresIn(std::string &anchor)
    {
        std::string expires_in_with_garbage = TailSlice(anchor, "expires_in=");
        return HeadSlice( expires_in_with_garbage, "&" );
    }
    
    static inline std::string ExtractError(std::string &search)
    {
        std::string error_with_garbage = TailSlice(search, "error");
        return HeadSlice( error_with_garbage, "&" );
    }

    static inline std::string ExtractErrorReason(std::string &search)
    {
        std::string error_reason_with_garbage = TailSlice(search, "error_reason");
        return HeadSlice( error_reason_with_garbage, "&" );
    }

    //--- Public Interface -------------------------------------------------------------~
    static inline url ParseHttpUrl(std::string &in) {
        url ret;
        
        ret.host = ExtractHost(in);
        ret.search = ExtractSearch(in);
        ret.anchor = ExtractAnchor(in);
        ret.access_token = ExtractAccessToken(ret.anchor);
        if ( ret.access_token.size() )
            ret.expires_in = ExtractExpiresIn(ret.anchor);
        ret.error = ExtractError(ret.search);
        if ( ret.error.size() )
            ret.error_reason = ExtractErrorReason(ret.search);
        
        return ret;
    }
}
