/*
 Copyright (c) 2010, The Barbarian Group
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/Area.h"

#if defined( CINDER_MAC )
#	include <CoreVideo/CoreVideo.h>
#	if defined( __OBJC__ )
		@class NSScreen;
#	else
		class NSScreen;
#	endif
#elif defined( CINDER_MSW )
	#include <windows.h>
	#undef min
	#undef max
#endif

#include <map>
#include <vector>

namespace cinder {

typedef std::shared_ptr<class Display> 	DisplayRef;

class Display {
 public:
	~Display();
	int		getWidth() const { return mArea.getWidth(); }
	int		getHeight() const { return mArea.getHeight(); }
	Area	getArea() const { return mArea; }
	int		getBitsPerPixel() const { return mBitsPerPixel; }

	//! Returns whether the Display's coordinates contain \a pt.
	bool	contains( const Vec2i &pt ) const { return mArea.contains( pt ); }

#if defined( CINDER_MAC )
	NSScreen*			getNsScreen() const { return mScreen; }
	CGDirectDisplayID	getCgDirectDisplayId() const { return mDirectDisplayID; }
#endif

	static DisplayRef						getMainDisplay();
	static const std::vector<DisplayRef>&	getDisplays();
	//! Returns the Display which contains a given point. Returns a NULL DisplayRef on failure
	static DisplayRef						getDisplayForPoint( const Vec2i &pt );
	
#if defined( CINDER_MAC )
	static DisplayRef			findFromCgDirectDisplayId( CGDirectDisplayID displayID );
#elif defined( CINDER_MSW )
	//! Finds a Display based on its HMONITOR. Returns the primary display if it's not found
	static DisplayRef			findFromHmonitor( HMONITOR hMonitor );
	static BOOL CALLBACK enumMonitorProc( HMONITOR hMonitor, HDC hdc, LPRECT rect, LPARAM lParam );
#endif
	
 private:
	Area		mArea;
	int			mBitsPerPixel;
#if defined( CINDER_MAC )
	NSScreen			*mScreen;
	CGDirectDisplayID	mDirectDisplayID;
#elif defined( CINDER_MSW )
	HMONITOR			mMonitor;
#endif
	
	static void		enumerateDisplays();
	
	static std::vector<DisplayRef>	sDisplays;
	static bool						sDisplaysInitialized;
};

} // namespace cinder
