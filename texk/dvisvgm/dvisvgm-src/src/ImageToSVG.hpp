/*************************************************************************
** ImageToSVG.hpp                                                       **
**                                                                      **
** This file is part of dvisvgm -- a fast DVI to SVG converter          **
** Copyright (C) 2005-2018 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#ifndef IMAGETOSVG_HPP
#define IMAGETOSVG_HPP

#include <istream>
#include <memory>
#include <string>
#include "SpecialActions.hpp"
#include "SVGTree.hpp"

struct SVGOutputBase;

class ImageToSVG : protected SpecialActions {
	public:
		ImageToSVG (const std::string &fname, SVGOutputBase &out) : _fname(fname), _out(out) {}
		virtual ~ImageToSVG () =default;
		void convert ();
		void setTransformation (const Matrix &m);
		void setPageSize (const std::string &name);
		std::string filename () const {return _fname;}

	protected:
		virtual std::string imageFormat () const =0;
		virtual bool imageIsValid () const =0;
		virtual BoundingBox imageBBox () const =0;
		virtual std::string psSpecialCmd () const =0;
		// implement abstract base class SpecialActions
		double getX () const override                           {return _x;}
		double getY () const override                           {return _y;}
		void setX (double x) override                           {_x = x; _svg.setX(x);}
		void setY (double y) override                           {_y = y; _svg.setY(y);}
		void finishLine () override                             {}
		void setColor (const Color &color) override             {_svg.setColor(color);}
		Color getColor () const override                        {return _svg.getColor();}
		void setMatrix (const Matrix &m) override               {_svg.setMatrix(m);}
		const Matrix& getMatrix () const override               {return _svg.getMatrix();}
		void getPageTransform (Matrix &matrix) const override   {}
		void setBgColor (const Color &color) override           {}
		void appendToPage(std::unique_ptr<XMLNode> &&node) override  {_svg.appendToPage(std::move(node));}
		void appendToDefs(std::unique_ptr<XMLNode> &&node) override  {_svg.appendToDefs(std::move(node));}
		void prependToPage(std::unique_ptr<XMLNode> &&node) override {_svg.prependToPage(std::move(node));}
		void pushContextElement (std::unique_ptr<XMLElementNode> &&node) override {_svg.pushContextElement(std::move(node));}
		void popContextElement () override                      {_svg.popContextElement();}
		void embed (const BoundingBox &bbox) override           {_bbox.embed(bbox);}
		void embed (const DPair &p, double r=0) override        {if (r==0) _bbox.embed(p); else _bbox.embed(p, r);}
		void progress (const char *id) override;
		unsigned getCurrentPageNumber() const override          {return 0;}
		BoundingBox& bbox () override                           {return _bbox;}
		BoundingBox& bbox (const std::string &name, bool reset=false) override {return _bbox;}
		std::string getSVGFilename (unsigned pageno) const override;
		std::string getBBoxFormatString () const override {return "";}

	private:
		std::string _fname;   ///< name of image file
		SVGTree _svg;
		SVGOutputBase &_out;
		double _x=0, _y=0;
		BoundingBox _bbox;
};

#endif