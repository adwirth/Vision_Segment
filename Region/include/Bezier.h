/*!
 * \file
 * Bezier class definition. This file is part of Region module.
 * Bezier class performs bezier curve fitting.
 *
 * \remarks
 * Original Bezier fitting algorithm by Xianshun Chen.
 * https://github.com/chen0040/cpp-spline
 *
 * \authors
 * Andras Wirth
 */

#ifndef _H__BEZIER_H
#define _H__BEZIER_H

#include "Curve.h"

class Bezier : public Curve
{
public:
	Bezier();
	virtual ~Bezier();

protected:
	virtual void _on_way_point_added();

protected:
	Vector interpolate(double u, const Vector& P0, const Vector& P1, const Vector& P2, const Vector& P3);
};

#endif