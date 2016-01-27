#ifndef _ANIMATORHOLDER_H_
#define _ANIMATORHOLDER_H_

#pragma once

#include <list>
#include <algorithm>
#include "Animator.h"
#include <iostream>

typedef std::list<Animator*> AnimatorList;

class AnimatorHolder {
	static AnimatorList running, suspended;
	class ProgressFunctor : public std::unary_function<Animator*, void> {
			timestamp_t t;
		public:
			void operator()(Animator* a) const { a->Progress(t); }
			ProgressFunctor(timestamp_t _t) : t(_t) {}
	};
	class PauseFunctor : public std::unary_function<Animator*, void> {
		timestamp_t t;
	public:
		void operator()(Animator* a) const { a->TimeSet(t); }
		PauseFunctor(timestamp_t _t) : t(_t) {}
	};
public:
	static void Register(Animator* a) { suspended.push_back(a); }
	static void Cancel(Animator* a) { suspended.remove(a); }
	static void MarkAsRunning(Animator* a)
	{
		suspended.remove(a); running.push_back(a);
	}
	static void MarkAsSuspended(Animator* a)
	{
		running.remove(a); suspended.push_back(a);
	}
	static void Progress(timestamp_t currTime) {
		std::for_each(
			running.begin(), running.end(), ProgressFunctor(currTime)
			);
	}
	static void Pause(timestamp_t currTime) {
		std::for_each(
			running.begin(), running.end(), ProgressFunctor(currTime)
			);
	}
};

#endif