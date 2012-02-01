
#include "TuioServer.h"
#include "cinder/app/App.h"

#include <set>
#include <map>

using namespace std;

namespace cinder { namespace tuio {

Server::Server() {
	bUpdateCursor = bUpdateObject = false;
	initialize( "localhost", 3333 );
}

Server::Server(const char *host, int port) {
	initialize( host, port );
}

void Server::initialize(const char* host, int port )
{
	mSender.setup( host , port);
	mCurrentFrame = sessionID = -1;	  
}

void Server::run()
{
	if(bUpdateCursor)
	{
		startCursorBundle();
		{
			mutex::scoped_lock lock(mCursorListMutex);
			for(std::list<Cursor*>::iterator iterCursor = cursorList.begin(); iterCursor != cursorList.end(); ++iterCursor)
			{
				addCursorMessage(*iterCursor);						
			}
		}
		sendCursorBundle(mCurrentFrame);
	}
	bUpdateCursor = false;

	if(bUpdateObject)
	{
		startObjectBundle();
		{
			mutex::scoped_lock lock(mObjectListMutex);
			for(std::list<Object*>::iterator iterObject = objectList.begin(); iterObject != objectList.end(); ++iterObject )
			{
				addObjectMessage(*iterObject);
			}
		}
		sendObjectBundle(mCurrentFrame);
	}
	bUpdateObject = false;

	++mCurrentFrame;	
}

void Server::shutdown()
{
	sendEmptyObjectBundle();
	sendEmptyCursorBundle();
}

Object* Server::addObject( int fid, float x, float y, float a )
{
	++sessionID;

	Object* object = new Object( "\tuio\2Dobj", sessionID, fid, ci::Vec2f(x, y), a, ci::Vec2f::zero(), 0.0f, 0.0f, 0.0f); 
	{
		mutex::scoped_lock lock(mObjectListMutex);
		objectList.push_back(object);
	}
	bUpdateObject = true;

	return object;
}

Object* Server::insertObject( list<ci::tuio::Object*>::iterator iter, int fid, float x, float y, float a )
{
	++sessionID;
	
	Object* object = new Object( "\tuio\2Dobj", sessionID, fid, ci::Vec2f(x, y), a, ci::Vec2f::zero(), 0.0f, 0.0f, 0.0f);
	{
		mutex::scoped_lock lock(mObjectListMutex);
		objectList.insert(iter, object);
	}
	bUpdateObject = true;

	return object;
}

Cursor* Server::addCursor( float x, float y )
{
	++sessionID;
	Cursor* cursor = new Cursor( "/tuio/2Dcur", sessionID, ci::Vec2f(x, y) );
	{
		mutex::scoped_lock lock(mCursorListMutex);
		cursorList.push_back( cursor );
	}		
	bUpdateCursor = true;

	return cursor;
}

void Server::removeCursor( Cursor* cursor )
{
	if(cursor == NULL) return;
	{
		mutex::scoped_lock lock(mCursorListMutex);
		cursorList.remove( cursor );
		delete cursor;
	}
	bUpdateCursor = true;	
}

void Server::updateCursor( Cursor* cursor, float x, float y )
{
	if(cursor == NULL) return;
	if( x < 0 || x > 1 || y < 0 || y > 1 ) return;
	{
		mutex::scoped_lock lock(mCursorListMutex);
		cursor->setPos( ci::Vec2f(x, y) );	
	}
	bUpdateCursor = true;
}

void Server::sendEmptyCursorBundle()
{
	sendEmptyBundle( "/tuio/2Dcur" );
}

void Server::startCursorBundle()
{
	mBundle.clear();
	osc::Message message;
	message.setAddress("/tuio/2Dcur");
	message.addStringArg("alive");	
	{
		mutex::scoped_lock lock(mCursorListMutex);
		for(std::list<Cursor*>::iterator iterCursor = cursorList.begin(); iterCursor != cursorList.end(); ++iterCursor)
		{
			message.addIntArg( (*iterCursor)->getSessionId() );
		}
	}
	mBundle.addMessage(message);
}

void Server::addCursorMessage( Cursor* cursor )
{	
	mBundle.addMessage( cursor->createSetMessage() );
}

void Server::sendCursorBundle( long fseq )
{
	sendBundle("/tuio/2Dcur", fseq);
}

void Server::sendEmptyObjectBundle()
{
	sendEmptyBundle( "/tuio/2Dobj" );
}

void Server::addObjectMessage( Object* object )
{
	mBundle.addMessage( object->createSetMessage() );
}

void Server::startObjectBundle()
{
	mBundle.clear();
	osc::Message message;
	message.setAddress("/tuio/2Dobj");
	message.addStringArg("alive");
	{
		mutex::scoped_lock lock(mObjectListMutex);
		for(std::list<Object*>::iterator iterCursor = objectList.begin(); iterCursor != objectList.end(); ++iterCursor)	{
			message.addIntArg( (*iterCursor)->getSessionId() );
		}	
	}
	mBundle.addMessage(message);
}

void Server::sendObjectBundle( long fseq )
{
	sendBundle("/tuio/2Dobj", fseq);
}

void Server::sendEmptyBundle( const char* bundleType )
{	
	mBundle.clear();
	osc::Message message;
	message.setAddress( bundleType );
	message.addStringArg("alive");
	mBundle.addMessage(message);
	message.clear();
	message.setAddress( bundleType );
	message.addStringArg("fseq");
	message.addIntArg(-1);
	mBundle.addMessage(message);
	mSender.sendBundle(mBundle);
	mBundle.clear();
}

void Server::sendBundle( const char* bundleType, long fseq )
{	
	osc::Message message;
	message.setAddress( bundleType );
	message.addStringArg( "fseq" );
	message.addIntArg( fseq );
	mBundle.addMessage(message);
	mSender.sendBundle(mBundle);
}

void Server::removeObject( Object* object )
{
	if(object == NULL) return;
	{
		mutex::scoped_lock lock(mObjectListMutex);
		objectList.remove( object );
	}
	delete object;
	bUpdateObject = true;
}

void Server::updateObject( Object* object, float x, float y, float a )
{
	if(object == NULL) return;
	if( x < 0 || x > 1 || y < 0 || y > 1 ) return;
	{
		mutex::scoped_lock lock(mObjectListMutex);
		object->update( ci::Vec2f(x,y), a) ;
	}
	bUpdateObject = true;
}

void Server::addExternalCursor( Cursor* cursor )
{
    if( cursor == NULL) return;
    cursorList.push_back(cursor);
    bUpdateCursor = true;
}

void Server::removeExternalCursor( Cursor* cursor )
{
    if( cursor == NULL) return;
    cursorList.remove(cursor);
    bUpdateCursor = true;
}

void Server::updateExternalCursor( Cursor* cursor )
{
    if( cursor == NULL) return;
    bUpdateCursor = true;
}

void Server::addExternalObject( Object* object )
{
    if( object == NULL) return;
    objectList.push_back(object);
    bUpdateObject = true;
}

void Server::removeExternalObject( Object* object )
{
    if( object == NULL) return;
    bUpdateObject = true;
}

void Server::updateExternalObject( Object* object )
{
    if( object == NULL) return;
    bUpdateObject = true;
}


} } //namespace cinder::tuio


