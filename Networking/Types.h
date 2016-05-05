// Types.h

/** Definitions of types with set sizes, and a few other relatively basic data types.
 *
 * @author Hans de Ruiter
 * 
 * @copyright 2009 by Hans de Ruiter, all rights reserved (See license.txt for 
 * licensing information)
 *
 * @version 0.1 -- 2009/03/10 - Initial version
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#if defined(__amigaos_4__) || defined(__morphos__)

#include <exec/types.h>

#elif defined(WIN32) || defined(WIN64)

typedef unsigned char uint8;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64; 
typedef unsigned long long uint64;

#elif defined(__linux__)

typedef unsigned char uint8;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long int64; 
typedef unsigned long uint64;

#elif defined(__APPLE__)

typedef unsigned char uint8;
typedef char int8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long int64;
typedef unsigned long uint64;

#else
#error "Operating system not recognized. Please add type definitions for your OS in " __FILE__
#endif

#ifndef NULL
#define NULL 0
#endif

/** A template class for rectangles.
 * Allows the dimensions to be stored in different types (e.g., int, float, etc.)
 */
template<class Type>
class RectangleTpl
{
public:
	inline RectangleTpl()
	{
	}

	/** Constructs a new RectangleTpl.
	 *
	 * @param x the x coordinate of the bottom left edge of the rectangle
	 * in image coordinates
	 * @param y the y coordiante of the bottom left edge of the rectangle
	 * in image coordinates
	 * @param width the width of the rectangle in image coordinates
	 * @param height the height of the rectangle in image coordinates
	 */
	inline RectangleTpl(Type x, Type y, Type width, Type height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	};

	bool operator == (const RectangleTpl<Type> &rect) const
	{
		return (x == rect.x && y == rect.y && width == rect.width && height == rect.height);
	}

	bool operator != (const RectangleTpl<Type> &rect) const
	{
		return !(*this == rect);
	}

	Type x;

	Type y;

	Type width;

	Type height;
};

typedef class RectangleTpl<int> RectangleInt;
typedef class RectangleTpl<float> RectangleFlt;
typedef class RectangleTpl<double> RectangleDbl;

#endif
