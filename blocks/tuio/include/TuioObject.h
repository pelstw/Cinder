/*
 Copyright (c) 2010, The Cinder Project: http://libcinder.org
 All rights reserved.

 Portions Copyright (c) 2010, Hector Sanchez-Pajares
 Aer Studio http://www.aerstudio.com
 All rights reserved.

 Modified by Andreas Stallinger, 2011

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
#include "cinder/Vector.h"

#include "TuioProfileBase.h"

namespace cinder { namespace tuio {

class Object : public ProfileBase {
  public:
	Object() : ProfileBase() {}

	Object( std::string source, int32_t sessionId, int32_t fiducialId, Vec2f pos, float angle, Vec2f speed, float rotationSpeed, float motionAccel, float rotationAccel )
		: ProfileBase(source,sessionId), mPos( pos ), mSpeed( speed ), mPrevPos( pos ),
		mFiducialId( fiducialId ), mAngle( angle ), mRotationSpeed( rotationSpeed ), mRotationAccel( rotationAccel )
	{}

	// Create from a '2dobj' 'set' message
	static Object createFromSetMessage( const osc::Message &message )
	{
		std::string source = message.getRemoteIp();
		int32_t sessionId = message.getArgAsInt32( 1 );
		int32_t fiducialId = message.getArgAsInt32( 2 );
		Vec2f pos = Vec2f( message.getArgAsFloat( 3 ), message.getArgAsFloat( 4 ) );
		float angle = message.getArgAsFloat( 5 );
		Vec2f speed = Vec2f( message.getArgAsFloat( 6 ), message.getArgAsFloat( 7 ) );
		float rotationSpeed = message.getArgAsFloat( 8 );
		float motionAccel = message.getArgAsFloat( 9 );
		float rotationAccel = message.getArgAsFloat( 10 );

		return Object( source, sessionId, fiducialId, pos, angle, speed, rotationSpeed, motionAccel, rotationAccel );
	}

	osc::Message createSetMessage()
	{
		osc::Message message;
		message.setAddress( "/tuio/2Dobj" );
		message.addStringArg( "set" );
		message.addIntArg( mSessionId );
		message.addIntArg( mFiducialId );
		message.addFloatArg( mPos.x );
		message.addFloatArg( mPos.y );
		message.addFloatArg( mAngle );
		message.addFloatArg( mSpeed.x );
		message.addFloatArg( mSpeed.y );
		message.addFloatArg( mRotationSpeed );
		message.addFloatArg( mMotionAccel );
		message.addFloatArg( mRotationAccel );

		return message;
	}

	Vec2f	getPos() const { return mPos; }
	Vec2f	getPrevPos() const { return mPrevPos; }
	Vec2f	getSpeed() const { return mSpeed; }
	float	getMotionAccel() const { return mMotionAccel; }

	int32_t getFiducialId() const {  return mFiducialId; };
	//! Returns the angle of the object measured in radians
	float   getAngle() const { return mAngle; }
	float   getRotationSpeed() const { return mRotationSpeed; }
	float   getRotationAccel() const {  return mRotationAccel; }

	void    update( Vec2f pos, float a) { mPos = pos; mAngle = a; }
    void    update( Vec2f pos, float a, Vec2f speed) { mPos = pos; mAngle = a; mSpeed = speed;}

    void    setPosition( Vec2f pos) { mPos = pos; }
    void    setAngle(float angle) { mAngle = angle; }
    void    setSpeed(Vec2f speed) { mSpeed = speed; }
    void    setMotionAccel(float accel) { mMotionAccel = accel; }
    void    setRotationAccel( float accel) { mRotationAccel = accel; }
    void    setRotationSpeed(float speed) { mRotationSpeed = speed; }
    
  protected:	
	int32_t		mFiducialId;
	float		mAngle;
	float		mRotationSpeed, mRotationAccel;
	Vec2f		mPos, mPrevPos;
	Vec2f		mSpeed;
	float		mMotionAccel;
};

} } // namespace cinder::tuio

