// EventEmitterImpl.h
//
// Method implementations for EventEmitter. This should be included from the *.cpp
// file of any classes that are derived from EventEmitter. 
//
// See EventEmitter.h file for details.
//
// Copyright (C) 2011 by Hans de Ruiter, all rights reserved

#ifndef __EVENTEMITTER_IMPL_H__
#define __EVENTEMITTER_IMPL_H__

#include "EventEmitter.h"
#include "EventListener.h"

template<class EventEmitterType, class EventListenerType>
EventEmitter<EventEmitterType, EventListenerType>::EventEmitter()
{
	// Nothing to do here
}

template<class EventEmitterType, class EventListenerType>
EventEmitter<EventEmitterType, EventListenerType>::~EventEmitter()
{
	// Let all of the event listeners know that this listener is gone
	EVENTEMITTER_EMPTYCHECK
	EVENTEMITTER_REVERSELOOPSTART(EventListenerType)
		(*currListener)->removeSource(this);
	EVENTEMITTER_LOOPEND
}

template<class EventEmitterType, class EventListenerType>
void EventEmitter<EventEmitterType, EventListenerType>::addListener(EventListenerType *listener)
{
	listeners.push_back(listener);
	listener->registerSource(this);
}

template<class EventEmitterType, class EventListenerType>
bool EventEmitter<EventEmitterType, EventListenerType>::removeListener(EventListenerType *listener)
{
	// Use a reverse order list, because frequently the last inserted listener 
	// will be the first removed (e.g., temporary listeners)
	EVENTEMITTER_EMPTYCHECK_RET(false)
	EVENTEMITTER_REVERSELOOPSTART(EventListenerType)
		if((*currListener) == listener)
		{
			(*currListener)->removeSource(this);
			listeners.erase(--(currListener.base()));
			return true;
		}
	EVENTEMITTER_LOOPEND

	// If we get to here, then the listener was not in the list
	return false;
}

template<class EventEmitterType, class EventListenerType>
bool EventEmitter<EventEmitterType, EventListenerType>::removeListener(
				EventListener<EventEmitterType, EventListenerType> *listener)
{
	return removeListener(reinterpret_cast<EventListenerType*>(listener));
}

#endif