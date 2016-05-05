// EventListener.h

#ifndef __EVENTLISTENER_H__
#define __EVENTLISTENER_H__

#include <list>

template<class EventEmitterType, class EventListenerType>
class EventEmitter;

/** Template class for event listeners (see the EventEmitter template class
 * for more). 
 *
 * This class' main purpose is to ensure that there are no dangling pointers to
 * deleted event listeners. Child-classes should define the actual event handler
 * methods themselves (e.g., create an ItemListeners to listen to events from the
 * Item class).
 *
 *
 * To use, include this file from the derived class' header file, and include
 * EventListenerImpl.h from the derived class' *.cpp file.
 *
 * @author Hans de Ruiter
 * 
 * @copyright 2011 by Hans de Ruiter, all rights reserved (See license.txt for 
 * licensing information)
 *
 * @version 0.1 -- 2011/05/11 - Initial version
 */
template<class EventEmitterType, class EventListenerType>
class EventListener
{
public:
	EventListener();

	virtual ~EventListener();

	/** Adds an event emitter as source.
	 * This is called by EventEmitter::addListener() to let this object know that it 
	 * has been added (important for cleanup).
	 *
	 * @param source pointer to the event emitter
	 */
	virtual void registerSource(EventEmitter<EventEmitterType, EventListenerType> *source);

	/** Removes an event emitter.
	 * This is called by EventEmitter::removeListener() to let this object know that it 
	 * has been added (important for cleanup).
	 *
	 * @param source pointer to the event emitter to remove
	 *
	 * @return bool true if successful, false if not found
	 */
	virtual bool removeSource(EventEmitter<EventEmitterType, EventListenerType> *source);

private:
	/** The list of event emitters that have registered as being connected to this
	 * listener.
	 */
	std::list<EventEmitter<EventEmitterType, EventListenerType>*> sources;
};

#endif
