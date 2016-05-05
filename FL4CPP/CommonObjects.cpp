#include "CommonObjects.h"

using namespace fl4cpp;

AgeRange::AgeRange( std::string min, std::string max ) :
_min( min ),
_max( max )
{
    
}

CoverPhoto::CoverPhoto( std::string nodeId, std::string source, int offsetY ) :
_nodeId( nodeId ),
_source( source ),
_offsetY( offsetY )
{
}

ImageSource::ImageSource( int width, int height, std::string source ) :
_width( width ),
_height( height ),
_source( source )
{

}

NameTag::NameTag( std::string nodeId, std::string name, std::string createdTime, int x, int y ) :
_nodeId( nodeId ),
_name( name ),
_createdTime( createdTime ),
_x( x ),
_y( y )
{
    
}