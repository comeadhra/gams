/*********************************************************************
 * Usage of this software requires acceptance of the SMASH-CMU License,
 * which can be found at the following URL:
 *
 * https://code.google.com/p/smash-cmu/wiki/License
 *********************************************************************/

/*********************************************************************
 * Position.h - Declares the structures for a Position and a Region.
 *********************************************************************/

#ifndef _POSITION_H
#define _POSITION_H

namespace SMASH
{
    namespace Utilities
    {
        /**
         * @brief Represents a two dimensional position, ignoring height.
         **/
        class Position
        {
        public:
	        double x;
	        double y;

            /**
	         * Blank default constructor.
             **/
	        Position() {}

	        /** Constructor from data.
             * @param   newX    The value for the X coordinate.
             * @param   newY    The value for the Y coordinate.
             **/
	        Position(double newX, double newY)
	        {
		        x = newX;
		        y = newY;
	        }
        };

        /**
         * @brief Represents a rectangular region defined by its top left and bottom right corners.
         **/
        class Region
        {
        public:
            Position topLeftCorner;
            Position bottomRightCorner;

            /**
	         * Blank default constructor.
             **/
	        Region() {}

	        /** Constructor from data.
             * @param   newTopLeftCorner        The Position for the top left corner.
             * @param   newBottomRightCorner    The Position for the bottom right corner.
             **/
	        Region(Position newTopLeftCorner, Position newBottomRightCorner)
	        {
		        topLeftCorner = newTopLeftCorner;
		        bottomRightCorner = newBottomRightCorner;
	        }
        };
    }
}

#endif