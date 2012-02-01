
#pragma once

#include "cinder/Cinder.h"
#include "cinder/Vector.h"
#include "cinder/Thread.h"
#include "cinder/Function.h"
#include "cinder/app/TouchEvent.h"
#include "OscSender.h"
#include "OscMessage.h"

#include <list>

#include "TuioObject.h"
#include "TuioCursor.h"


namespace cinder { namespace tuio {

class Server
{
    public: 
        Server();
        Server(const char *host, int port);
		
        tuio::Cursor* addCursor(float x, float y);
        void updateCursor( Cursor* cursor, float x, float y );
        void removeCursor( Cursor* cursor );
        void addExternalCursor(Cursor* cursor);
        void removeExternalCursor(Cursor* cursor);
        void updateExternalCursor(Cursor* cursor);


        Object* addObject( int fid, float x, float y, float a );
		Object* insertObject( std::list<ci::tuio::Object*>::iterator iter, int fid, float x, float y, float a );
        void updateObject( Object* object, float x, float y, float a );
        void removeObject( Object* object );
        void addExternalObject(Object* object);
        void removeExternalObject(Object* object);
        void updateExternalObject(Object* object);

        void run();
        /*! /brief		Calls the OSC setup method
            /param		const char * host hostaddress
            /param		int port defines on with port the application run
            /return		void
        !*/
        void initialize( const char* host, int port );

		void shutdown();
    private:
        
        /*! /brief		adds a cursor message to a bundle
            /param		Cursor * cursor 
            /return		void
        !*/
        void addCursorMessage(Cursor* cursor);
        /*! /brief		adds an object message to a bundle
            /param		Object * object
            /return		void
        !*/
        void addObjectMessage(Object* object);
        
        /*! /brief		starts a new CursorBundle
            /return		void
        !*/
        void startCursorBundle();
        /*! /brief		starts a new ObjectBundle
            /return		void
        !*/
        void startObjectBundle();

        /*! /brief		send an empty bundle of the specified type
            /param		const char * bundleType 
            /return		void
        !*/
        void sendEmptyBundle(const char* bundleType);
        /*! /brief		Calls sendEmptyBundle with 2Dcur as bundleType
            /sa			sendEmptyBundle, sendEmptyObjectBundle
            /return		void
        !*/
        void sendEmptyCursorBundle();
        /*! /brief		Calls sendEmptyBundle with 2Dobj as bundleType
            /sa			sendEmptyBundle, sendEmptyCursorBundle
            /return		void
        !*/
        void sendEmptyObjectBundle();

        /*! /brief		Sends a bundle 
            /param		const char * bundleType
            /param		long fseg
            /return		void
        !*/
        void sendBundle( const char* bundleType, long fseg);
        /*! /brief      Calls sendBundle with 2Dcur as bundleType
            /sa         sendBundle, sendObjectBundle
            /param		long fseq
            /return		void
        !*/
        void sendCursorBundle(long fseq);
        /*! /brief		Calls sendBundle with 2Dobj as bundleType
            /sa         sendBundle, sendCursorBundle
            /param		long fseq
            /return		void
        !*/
        void sendObjectBundle(long fseq);

        osc::Sender			mSender;
        osc::Bundle			mBundle;
        osc::Message		mMessage;

        long				mCurrentFrame;

        std::list<Object*>	objectList;
        std::list<Cursor*>	cursorList;

        long sessionID;
        bool bUpdateCursor, bUpdateObject;

        boost::mutex		mObjectListMutex;
        boost::mutex		mCursorListMutex;
};


} } //namespace cinder::tuio

