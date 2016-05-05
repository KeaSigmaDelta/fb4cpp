// EventEmitter.h

#ifndef __EVENTEMITTER_H__
#define __EVENTEMITTER_H__

#include <list>

template<class EventEmitterType, class EventListenerType>
class EventListener;

/** Template class for event emitters. 
 * This takes care of the low-level handling of adding and removing event listeners.
 * MACROS make it easy to create event dispatch methods in child-classes.
 *
 * To use, include this file from the derived class' header file, and include
 * EventEmitterImpl.h from the derived class' *.cpp file.
 *
 * @author Hans de Ruiter
 * 
 * @copyright 2011 by Hans de Ruiter, all rights reserved (See license.txt for 
 * licensing information)
 *
 * @version 0.1 -- 2011/05/11 - Initial version
 */
template<class EventEmitterType, class EventListenerType>
class EventEmitter
{
public:
	EventEmitter();

	virtual ~EventEmitter();

	/** Adds an event listener to this object's listener list.
	 *
	 * @param listener pointer to the event listener
	 */
	virtual void addListener(EventListenerType *listener);

	/** Removes the given update listener from this object.
	 * 
	 * @param updateListener the update listener to remove
	 *
	 * @return bool true if successful, false if not found
	 */
	virtual bool removeListener(EventListenerType *listener);

	/** Removes the given update listener from this object.
	 * 
	 * @param updateListener the update listener to remove
	 *
	 * @return bool true if successful, false if not found
	 */
	virtual bool removeListener(
		EventListener<EventEmitterType, EventListenerType> *listener);

protected:
	/** The list of event listeners.
	 */
	std::list<EventListenerType*> listeners;
};

// ----- HELPER MACROS -----

// MACRO for the beginning of an emit-event loop. 
//
// Example use:
// \begincode
// void Item::emitItemChangedEvent(int counter)
// {
//     EVENTEMITTER_EMPTYCHECK
//     EVENTEMITTER_LOOPSTART(ItemListener)
//     (*currListener)->itemChanged(this, counter);
//     EVENTEMITTER_LOOPEND
// }
// \endcode
// 
// @param EventListenerType the event listener's class name

#if _MSC_VER && !__INTEL_COMPILER
#define EVENTEMITTER_LOOPSTART(EventListenerType) \
	for(std::list<EventListenerType*>::iterator currListener = listeners.begin(); \
						currListener != listeners.end(); ++currListener) \
	{
#else
#define EVENTEMITTER_LOOPSTART(EventListenerType) \
	for(typename std::list<EventListenerType*>::iterator currListener = listeners.begin(); \
						currListener != listeners.end(); ++currListener) \
	{
#endif

// MACRO for the beginning of an emit-event loop which starts at the end of the list
// instead of the beginning. 
//
// Example use:
// \begincode
// void Item::emitItemChangedEvent(int counter)
// {
//     EVENTEMITTER_EMPTYCHECK
//     EVENTEMITTER_REVERSELOOPSTART(ItemListener)
//     (*currListener)->itemChanged(this, counter);
//     EVENTEMITTER_LOOPEND
// }
// \endcode
// 
// @param EventListenerType the event listener's class name
#if _MSC_VER && !__INTEL_COMPILER
#define EVENTEMITTER_REVERSELOOPSTART(EventListenerType) \
	for( std::list<EventListenerType*>::reverse_iterator currListener = listeners.rbegin(); \
						currListener != listeners.rend(); ++currListener) \
	{
#else
#define EVENTEMITTER_REVERSELOOPSTART(EventListenerType) \
	for(typename std::list<EventListenerType*>::reverse_iterator currListener = listeners.rbegin(); \
						currListener != listeners.rend(); ++currListener) \
	{
#endif

// MACRO for the end of an emit-event loop (both forward or reverse). 
#define EVENTEMITTER_LOOPEND }

// A MACRO that checks if the listeners list is empty, and returns
#define EVENTEMITTER_EMPTYCHECK \
	if(listeners.empty()) \
    { \
		return; \
	}

// A MACRO that checks if the listeners list is empty, and returns
#define EVENTEMITTER_EMPTYCHECK_RET(retVal) \
	if(listeners.empty()) \
    { \
		return retVal; \
	}

// The same as EVENTEMITTER_EMPTYCHECK, but returns the given value if
// the listeners list is empty
#define EVENTEMITTER_EMPTYCHECK_RET(retVal) \
	if(listeners.empty()) \
    { \
		return retVal; \
	}

#endif
