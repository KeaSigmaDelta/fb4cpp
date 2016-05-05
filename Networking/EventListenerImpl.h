// EventListenerImpl.h
//
// Method implementations for EventListener. This should be included from the *.cpp
// file of any classes that are derived from EventListener. 
//
// See EventListener.h file for details.
//
// Copyright (C) 2011 by Hans de Ruiter, all rights reserved

#ifndef __EVENTLISTENER_IMPL_H__
#define __EVENTLISTENER_IMPL_H__

#include "EventEmitter.h"
#include "EventListener.h"

template<class EventEmitterType, class EventListenerType>
EventListener<EventEmitterType, EventListenerType>::EventListener()
{
	// Nothing to do here
}

template<class EventEmitterType, class EventListenerType>
EventListener<EventEmitterType, EventListenerType>::~EventListener()
{
	// Remove this listener from all sources
	while(!sources.empty())
	{
		(*sources.begin())->removeListener(this);
	}
}

template<class EventEmitterType, class EventListenerType>
void EventListener<EventEmitterType, EventListenerType>::registerSource(
					EventEmitter<EventEmitterType, EventListenerType> *source)
{
	sources.push_back(source);
}

template<class EventEmitterType, class EventListenerType>
bool EventListener<EventEmitterType, EventListenerType>::removeSource(
					EventEmitter<EventEmitterType, EventListenerType> *source)
{
	// Use a reverse order list, because frequently the last inserted source
	// will be the first removed
	for(typename std::list<EventEmitter<EventEmitterType, EventListenerType>*>::reverse_iterator
								currSource = sources.rbegin();
		currSource != sources.rend(); ++currSource)
	{
		if((*currSource) == source)
		{
			sources.erase(--(currSource.base()));
			// Since this is called by the even emitter, no need to 
			// call source->removeListener();
			return true;
		}
	}

	// If we get to here, then it wasn't found
	return false;
}

#endif